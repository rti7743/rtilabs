#include "JuliusPlus.h"


/*
//waveに保存する場合・・・
static FILE *recfile_fp = NULL;
static boolean open_error = FALSE;
static int recordlen;
static char rectmpfilename[MAXLINELEN];
static char recordfilename[MAXLINELEN];

static void
	make_tmp_filename(char *buf, int buflen, char *dirname)
{
#if defined(_WIN32) && !defined(__CYGWIN32__)
	snprintf(buf, buflen, "%s\\tmprecord.000.wav", dirname);
#else
	snprintf(buf, buflen, "%s/tmprecord.%d", dirname, getpid());
#endif
}  


static void
	record_sample_open(Recog *recog, void *dummy)
{
	if (recfile_fp != NULL) {
		fprintf(stderr, "Error: record_sample_open: re-opened before closed!\n"); 
		return;
	}

	make_tmp_filename(rectmpfilename, MAXLINELEN, ".\\");
	if ((recfile_fp = wrwav_open(rectmpfilename, recog->jconf->input.sfreq)) == NULL) {
		perror("Error: record_sample_open");
		fprintf(stderr, "failed to open \"%s\" (temporary record file)\n", rectmpfilename);
		open_error = TRUE;
		return;
	}

	recordlen = 0;
}

static void
	record_sample_close(Recog *recog, void *dummy)
{
	open_error = FALSE;
	if (recfile_fp == NULL) {
		fprintf(stderr, "Warning: record_sample_close; file not opened yet!?\n");
		return;
	}

	if (wrwav_close(recfile_fp) == FALSE) {
		perror("Error: record_sample_close");
	}
	recfile_fp = NULL;

	if (recordlen == 0) {
		unlink(rectmpfilename);
		if (verbose_flag) {
			fprintf(stderr, "No input, not recorded\n");
		}
		return;
	}

	// now rename the temporary file to time-stamp filename
	if (rename(rectmpfilename, recordfilename) < 0) {
		perror("Error: record_sample_close");
		fprintf(stderr, "failed to move %s to %s\n", rectmpfilename, recordfilename);
		return;
	}
	if (verbose_flag) {
		fprintf(stderr, "recorded to \"%s\" (%d bytes, %.2f sec.)\n", recordfilename, recordlen * sizeof(SP16), (float)recordlen / (float) recog->jconf->input.sfreq);
	}
}

static void
	record_sample_write(Recog *recog, SP16 *speech, int samplenum, void *dummy)
{
	static char tstr[20];

	if (recfile_fp == NULL) {
		if (! open_error) record_sample_open(recog, dummy);
	}

	if (wrwav_data(recfile_fp, speech, samplenum) == FALSE) {
		perror("Error: record_sample_write");
		fprintf(stderr, "failed to write samples to \"%s\"\n", rectmpfilename);
		return;
	}

	// make timestamp of system time when an input begins 
	// the current temporal recording file will be renamed to this time-stamp filename 
	if (recordlen == 0) {		// beginning of recording 
		//    timestring(tstr, 18);
	}
	//  make_record_filename(recordfilename, MAXLINELEN, tstr, record_dirname);

	recordlen += samplenum;
}
*/


void
	status_recready(Recog *recog, void *dummy)
{
	if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
		fprintf(stderr,"マイクで喋ってください。何かキーを押すと、プログラムを週利用します。\r\n");
		fprintf(stderr,"----コマンド----\r\n");
		fprintf(stderr,"なのー電気つけて\r\n");
		fprintf(stderr,"なのー電気けして\r\n");
		fprintf(stderr,"なのーエアコンつけて\r\n");
		fprintf(stderr,"なのーエアコンけして\r\n");
		fprintf(stderr,"なのーどいつむら\r\n");
		fprintf(stderr,"それっぽいのだと マッチ!! ってでます。\r\n");
		fprintf(stderr,"問題点: juliusは逆方向から検索するので、「どいつむら」だけで高確率マッチしてしまう\r\n");

		fprintf(stderr, "<<< please speak >>>");
	}
}


void
	status_recstart(Recog *recog, void *dummy)
{
	if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
		fprintf(stderr, "\r                    \r");
	}
}


//1ノード単位 
//ノードとは、 dict の単語単位、1つの単語単位だと思ってください。
//単語が集まって、文章ができます。
struct OneNode
{
	std::string word;
	std::string yomi;

	int dfaNumber;

	float cmscore;	//確率 juliusがいう「ライバルに比べた」単語信頼度。
					//ライバルに比べたというのがミソで、ライバルがほとんどいない単語群だと
					//ふつーに 1.0 (100%)になってしまうという不思議仕様の使えない子。ダメダメすぐる。
					//julius plusでは、gomi単語をばらまいてライバルを擬似的に作るという荒業をおこなっている。
	float avgscore;	//-walign で取得できる単語のスコア。 尤度？
	int begin_frame;
	int end_frame;

	float plus_node_score; // avgscore / (end_frame-begin_frame) にしたもの。
};
struct OncSentence
{
	std::list<OneNode*>        nodes;

	float score;				//julus公式のスコア
	float plus_sentence_score;	//いろいろ合算して、そこそこ信頼出来る数字にしたもの
};


static void FreeAllSentenceList(std::list<OncSentence*> *allSentence)
{
	for( auto it = allSentence->begin() ; it != allSentence->end() ; ++it)
	{
		for( auto it2 = (*it)->nodes.begin() ; it2 != (*it)->nodes.end() ; ++it2)
		{
			delete *it2;
		}
		delete *it;
	}
	allSentence->clear();
}

static float computePlusScore(const std::list<OneNode*>& nodes,float score,int sentnum,float mseclen)
{
	//まずは、文のスコアを録音時間で割ります。
	float plus_sentence_score = score / mseclen;

	//次に、仮説数に応じてペナルティを与えます。
	//仮説が多いということは、似たようなものが多くて迷っているわけで、これを反映する。
	switch(sentnum)
	{
	case 0:
	case 1:
		//ペナルティなし
		break;
	case 2:
		plus_sentence_score *= 1.1f;
		break;
	case 3:
		plus_sentence_score *= 1.15f;
		break;
	case 4:
		plus_sentence_score *= 1.5f;
		break;
	case 5:
		plus_sentence_score *= 2.0f;
		break;
	default:
		plus_sentence_score *= 3.0f;
		break;
	}

	//cmscore の平均値
	float cmsplus = 0;
	for ( auto it = nodes.begin() ; it != nodes.end() ; ++ it )
	{
		cmsplus += (*it)->cmscore;
	}
	cmsplus = cmsplus / nodes.size();
	cmsplus = (1 - cmsplus + 1) * 10; //反転させて10倍してペナルティとする。大きいほど罰則がでかい

	plus_sentence_score *= cmsplus;	//cmscore によるペナルティを入れる.

	//で、このスコアで見れば、
	//たいていうまく行くと-20程度、失敗すると-100程度に収束するんで、
	//それを 1 - 0 に丸めると、それっぽく見えるでしょwww
	//
	//正しい単語を言った時に 0.80 ぐらいになるように調整します。
	plus_sentence_score = (110 - (plus_sentence_score * -1)) / 100;
	if (plus_sentence_score >= 1) return 1;
	if (plus_sentence_score <= 0) return 0;

	return plus_sentence_score;
}


static void convertResult(const Recog *recog, std::list<OncSentence*> *allSentence)
{
	//喋った時間の総数
	const float mseclen = (float)recog->mfcclist->param->samplenum * (float)recog->jconf->input.period * (float)recog->jconf->input.frameshift / 10000.0f;

	for(RecogProcess* r = recog->process_list; r ; r=r->next) 
	{
		//ゴミを消します。
		if (! r->live || r->result.status < 0 ) 
		{
			continue;
		}

		// output results for all the obtained sentences
		auto winfo = r->lm->winfo;
		for(auto n = 0; n < r->result.sentnum; n++) 
		{ // for all sentences
			const auto s = &(r->result.sent[n]);
			const auto seq = s->word;
			const auto seqnum = s->word_num;

			//一文
			OncSentence * oneSentence = new OncSentence;

			// output word sequence like Julius 
			//0 , [1 , 2, 3, 4], 5, と先頭と最後を覗いているのは、開始終端、記号を抜くため
			for(int i=1;i<seqnum-1;i++)
			{
				//1単語 --> 単語の集合が文になるよ
				int i_seq = seq[i];
				auto oneNode = new OneNode();
				oneNode->word = winfo->woutput[i_seq];
				//dfa 対応している番号(dfaファイルの左側1行目の数字)
				oneNode->dfaNumber = atoi(winfo->wname[i_seq]);

				//読み(ローマ字っぽい HMM表記)
				oneNode->yomi = "";
				for (int j=0;j<winfo->wlen[i_seq];j++) 
				{
					char buf[32];
					oneNode->yomi += center_name( (winfo->wseq[i_seq][j]->name),buf);
				}

				//cmscoreの数字(このままでは使えない子状態)
				oneNode->cmscore = s->confidence[i];

				//開始フレームとスコアを取得します。
				for (auto align = s->align; align; align = align->next)
				{
					if (align->unittype != PER_WORD)
					{//-walign 以外無視
						continue;
					}
					for(int ii=0;ii<align->num;ii++) 
					{
						if (align->w[ii] != i_seq)
						{//現在単語ノードi と同じものじゃないと見ない
							continue;
						}
						assert(oneNode->word == winfo->woutput[align->w[ii]]);

						//開始フレーム
						oneNode->begin_frame = align->begin_frame[ii];
						//終了フレーム
						oneNode->end_frame = align->end_frame[ii];
						//単語のスコア
						oneNode->avgscore = align->avgscore[ii];
						//単語のスコアを尤度
						oneNode->plus_node_score = align->avgscore[ii] / (align->end_frame[ii] - align->begin_frame[ii]);
					}
				}

				//今の 1文 に1単語を追加.
				oneSentence->nodes.push_back(oneNode);
			}
			//julius のスコア 尤度らしい。マイナス値。0に近いほど正しいらしい。
			//「が」、へんてこな文章を入れても、スコアが高くなってしまし、長い文章を入れるとスコアが絶望的に低くなる
			//ので、そのままだと使えない。
			oneSentence->score = s->score;
			//多少使えるスコアを計算します。
			oneSentence->plus_sentence_score = computePlusScore(oneSentence->nodes,s->score,r->result.sentnum,mseclen);

			allSentence->push_back(oneSentence);
		}
	}
}



/** 
* Callback to output final recognition result.
* This function will be called just after recognition of an input ends
* 
*/
static void
	output_result(Recog *recog, void *dummy)
{
	std::list<OncSentence*> allSentence;
	convertResult(recog, &allSentence);

	std::cout << std::endl;
	std::cout << "==------------------------------------------------------===" << std::endl;

	int i = 1;
	for(auto it = allSentence.begin() ; it != allSentence.end() ; ++it, ++i)
	{
		std::stringstream word;
		std::stringstream yomi;
		std::stringstream dfaNumber;
		std::stringstream cmscore;
		std::stringstream avgscore;
		std::stringstream begin_frame;
		std::stringstream end_frame;
		std::stringstream plus_node_score;
		for( auto it2 = (*it)->nodes.begin() ; it2 != (*it)->nodes.end() ; ++it2)
		{
			word << (*it2)->word << " ";
			yomi << (*it2)->yomi << " ";
			dfaNumber << (*it2)->dfaNumber << " ";
			cmscore << (*it2)->cmscore << " ";
			avgscore << (*it2)->avgscore << " ";
			begin_frame << (*it2)->begin_frame << " ";
			end_frame << (*it2)->end_frame << " ";
			plus_node_score << (*it2)->plus_node_score << " ";
		}
		std::cout << i << ". " << "plus_sentence_score:" << (*it)->plus_sentence_score << " score:" << (*it)->score << std::endl;
		std::cout << " word       :" << word.str() << std::endl;
		std::cout << " yomi       :" << yomi.str() << std::endl;
		std::cout << " dfaNumber  :" << dfaNumber.str() << std::endl;
		std::cout << " cmsscore   :" << cmscore.str() << std::endl;
		std::cout << " avgscore   :" << avgscore.str() << std::endl;
		std::cout << " begin_frame:" << begin_frame.str() << std::endl;
		std::cout << " end_frame  :" << end_frame.str() << std::endl;
		std::cout << " plus_node_s:" << plus_node_score.str() << std::endl;
		std::cout << std::endl;

		if (i >= 4)
		{
			break;
		}
	}
	std::cout << std::flush;

	FreeAllSentenceList(&allSentence);
}

template<typename _INT> static const std::string num2str(_INT i)
{
	std::stringstream out;
	out << i;
	return out.str();
}
template<typename _INT> static const std::wstring num2wstr(_INT i)
{
	std::wstringstream out;
	out << i;
	return out.str();
}

JuliusPlus::JuliusPlus() : Grammer("topgrammer")
{
	this->jconf = NULL;
	this->recog = NULL;
	this->Thread = NULL;

	this->LocalCaptureRuleNodeCount = 0;
	this->GlobalRuleNodeCount = 0;
}

JuliusPlus::~JuliusPlus()
{
	JuliusStop();
}

bool JuliusPlus::MakeJuliusRule(std::ostream* dfa , std::ostream* dict  )
{
	struct reverse_automaton
	{
		//出力する dfaハンドル
		std::ostream* dfa;
		//出力する dictハンドル
		std::ostream* dict;

		//次の dfa 番号
		int dfaNumber ;
		//次の dict 番号
		int dictNumber ;

		//ゴミルールのdict
		int dictGomiNumer ;

		void fire(const rule* toprule,std::ostream* dfa , std::ostream* dict)
		{
			this->dfaNumber = 3;
			this->dictNumber = 3;
			this->dfa = dfa;
			this->dict = dict;
			this->dictGomiNumer = 2;

			int firstRuleNumber = this->dfaNumber++; //3
			assert(firstRuleNumber == 3); //0 silE
			//1 silB
			//2 -1 -1
			//なので4からスタート

			*dfa << 0 << " " << 0 << " " << firstRuleNumber << " " << 0 << " " << 1 << std::endl;
			*dict << 0 << "\t" << "[</s>]" << "\t" << " " << "silE" << std::endl;

			*dfa << 1 << " " << 1 << " " << 2 << " " << 1 << " " << 1 << std::endl;
			*dict << 1 << "\t" << "[<s>]" << "\t" << " " << "silB" << std::endl;

			*dfa << 2 << " " << -1 << " " << -1 << " " << 1 << " " << 0 << std::endl;

			*dfa << firstRuleNumber << " " << this->dictGomiNumer << " " << 1 << " " << 0 << " " << 0 << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_N]" << "\t" << "N" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_a]" << "\t" << "a" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_a:]" << "\t" << "a:" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_b]" << "\t" << "b" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_by]" << "\t" << "by" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_ch]" << "\t" << "ch" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_d]" << "\t" << "d" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_dy]" << "\t" << "dy" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_e]" << "\t" << "e" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_e:]" << "\t" << "e:" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_f]" << "\t" << "f" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_g]" << "\t" << "g" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_gy]" << "\t" << "gy" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_h]" << "\t" << "h" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_hy]" << "\t" << "hy" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_i]" << "\t" << "i" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_i:]" << "\t" << "i:" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_j]" << "\t" << "j" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_k]" << "\t" << "k" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_ky]" << "\t" << "ky" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_m]" << "\t" << "m" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_my]" << "\t" << "my" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_n]" << "\t" << "n" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_ny]" << "\t" << "ny" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_o]" << "\t" << "o" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_o:]" << "\t" << "o:" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_p]" << "\t" << "p" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_py]" << "\t" << "py" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_q]" << "\t" << "q" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_r]" << "\t" << "r" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_ry]" << "\t" << "ry" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_s]" << "\t" << "s" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_sh]" << "\t" << "sh" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_t]" << "\t" << "t" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_ts]" << "\t" << "ts" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_u]" << "\t" << "u" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_u:]" << "\t" << "u:" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_w]" << "\t" << "w" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_y]" << "\t" << "y" << std::endl;
			*dict << this->dictGomiNumer	<< "\t" << "[gomi_z]" << "\t" << "z" << std::endl;

			func(toprule,1,0 , firstRuleNumber);
		}

		//rule   処理するルール
		//number ルールに割り振られるべき番号 dfaNumber 番号。
		//backNumber このルールの親ルールの番号
		//termNumber このルールがどこにも遷移しない終端ルールだった時に進む番号
		void func(const rule* currentrule,int number,int backNumber,int termNumber)
		{
			assert(currentrule);
			auto words = currentrule->getWords();
			auto nestrules = currentrule->getNestRules();
			auto termRule = currentrule->getTermRule();

			if (words->size() <= 0)
			{//単語を伴わないノードだったら
				if (termRule == NULL)
				{//ネストだけのノードだったら自分をスキップする。
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , number ,backNumber , termNumber);
					}
				}
				else
				{//term ruleがあるのでそっちを実効する.
					func( termRule , number ,backNumber,termNumber);

					int nextNumber = this->dfaNumber ++ ;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
				//スキップするのでココでおわり.
				return ;
			}

			//単語辞書 dictを構築する。
			int dictNumber = this->dictNumber++;
			{
				for(auto it = words->begin(); it != words->end() ; ++it )
				{
					if ((*it)->word == "")
					{
						*dict << dictNumber	<< "\t" << "[<sp>]" << "\t " << "sp" << std::endl;
					}
					else
					{
						*dict << dictNumber << "\t" << "[" << (*it)->word << "]" << "\t" << " " << (*it)->yomi << std::endl;
					}
				}
			}

			//term ruleがあれば先に実行する。
			if (termRule == NULL)
			{//終端ノードなし
				if (nestrules->size() <= 0 && words->size() >= 1)
				{//単語だけのノードの場合、終端になる。
					*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					if (currentrule->getName() == "YobikakeRule")
					{
						*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					}
				}
				else
				{//単語とネストが共にある場合。
					//ネストの処理を行う。
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , termNumber);
					}
				}
			}
			else
			{
				int nestTermNumber = this->dfaNumber ++ ;

				if (nestrules->size() <= 0 && words->size() >= 1)
				{//単語だけのノードの場合、終端になる。
					*dfa << nestTermNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
				}
				else
				{//単語とネストが共にある場合。
					//ネストの処理を行う。
					int nextNumber = this->dfaNumber ++ ;
					*dfa << nextNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					for( auto it = nestrules->begin(); it != nestrules->end() ; ++it )
					{
						func( *it , nextNumber ,number , nestTermNumber);
					}
				}
				func( termRule , nestTermNumber ,number,termNumber);
			}
		}
	} maton;
	maton.fire( &this->Grammer ,dfa , dict  );

	return true;
}

//音声認識ルールを構築します。 正規表現にも対応しています。
xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) 
{
	_USE_WINDOWS_ENCODING;

	//一番最初だけ正規表現の構文変換をかける.
	// .+ --> (:?.*)
	// (まる|さんかく)? --> (まる|さんかく|)   正しい正規表現としてはエラーだが、このエンジンの場合容認する.
	// なのは? --> なの(は|)
	std::wstring optstr = L"";

	//処理を簡単にするために、UTF-16でループします。
	for( const wchar_t * p = _A2W( str.c_str() ) ; *p ; ++p )
	{
		if ( *p == L'.' && *(p+1) == L'+')
		{ // .+ --> (:?.*)
			optstr += L"(?:.+)";
			++p;
		}
		else if (*p == L'(' && *(p+1) == L'?' && *(p+2) == L':' )
		{
			optstr += L"(?:";
			p+=2;
		}
		else if (*(p+1) == L'?')
		{
			if (*p == L')')
			{// (まる|さんかく)? --> (まる|さんかく|)
				optstr += L"|)";
			}
			else 
			{// なのは? --> なの(は|)
				optstr += std::wstring(L"") + L"(?:" + *p + L"|)";
			}
			++p;
		}
		else if (*p == L'*' || *p == L'+' || *p == L'.' || *p == L'[' || *p == L']')
		{
			//            throw exception(std::string("") + "現在は、メタ文字 " + p + " は利用できません。利用可能なメタ文字 () | .+ ?");
		}
		else
		{
			optstr += *p;
		}
	}


	this->LocalCaptureRuleNodeCount = 1;
	std::string name = "@@" + num2str(id) + ":" + num2str(this->GlobalRuleNodeCount++);

	rule* firstNestRule = new rule(name);
	stateHandle->AddNestRule(firstNestRule);
	return AddRegexpImpl(id,optstr, firstNestRule);
}

//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle)
{
	_USE_WINDOWS_ENCODING;
	std::wstring matchString;

	//正規表現をパースしながら回す.
	const wchar_t * p = str.c_str();
	const wchar_t * splitPos = p;
	rule*  currentRule = stateHandle;
	for(  ; *p ; ++p )
	{
		if (*p == L'(')
		{
			//閉じ括弧まで飛ばす. )
			int nest = 1;
			const wchar_t* n = p + 1;
			for( ; *n  ; ++n )
			{
				if (*n == L'(')
				{
					++nest ;
				}
				else if (*n == L')')
				{
					--nest ;
					if (nest <= 0)
					{
						break;
					}
				}
			}

			//ネストする前の部分
			matchString = std::wstring(splitPos, 0 ,p - splitPos);


			//ネストしている部分を格納するルールを作る.
			rule*  nestRule;

			//キャプチャー？
			std::wstring captureNodeName;
			if (*(p+1) == L'?' && *(p+2) == L':')
			{
				p += 2;
			}
			else
			{
				captureNodeName = num2wstr(this->LocalCaptureRuleNodeCount++) + L":" + num2wstr(this->GlobalRuleNodeCount) ;
			}
			//お目当ての()ネストを格納する部分を作る
			if (captureNodeName.length() <= 0)
			{
				nestRule = new rule(this->GlobalRuleNodeCount ++);
			}
			else
			{
				nestRule = new rule( _W2A(captureNodeName.c_str()) );
			}

			if (matchString.empty())
			{//先行文字列が空
				currentRule->AddNestRule(nestRule);
			}
			else
			{//文字列だけを入れると OR扱いになってしまうので、ネストを格納するネストを作らないとダメ。
				rule* firstNestRule = new rule(this->GlobalRuleNodeCount ++);
				firstNestRule->AddWord(_W2A(matchString.c_str()));

				firstNestRule->AddNestRule(nestRule);
				currentRule->AddNestRule(firstNestRule);
			}

			//かっこの後にも構文が連続する場合、そのツリーを作成する.
			if (*n == L'\0'  || *(n+1) == L'\0' || *(n+1) == L'|')
			{//閉じかっこで構文がとまる場合はそこで終端
				//nop
			}
			else
			{//さらに構文が続いている場合
				rule * afterRule = new rule( this->GlobalRuleNodeCount++);
				nestRule->SetCombineRule(afterRule);

				currentRule = afterRule;
			}

			//ネストしているルールを再帰して実行.
			matchString = std::wstring(p+1 , 0 , (int) (n - p - 1) );
			auto r = this->AddRegexpImpl(id,matchString, nestRule);
			if(!r)
			{
				return xreturn::errnoError(r);
			}

			p = n ;
			splitPos = n + 1;  //+1は最後の ) を飛ばす. iは forの ++i で i == splitPos となる。(わかりにくい)
		}
		else if (*p == L'|') 
		{
			matchString = std::wstring(splitPos,0 , (int) (p - splitPos));
			if (matchString.length() >= 1)
			{
				currentRule->AddWord(_W2A(matchString.c_str()));
			}
			//空分岐 (A|) のような場合、空ノードを入れる.
			if (matchString.length() <= 0 || *(p+1) == L'\0' )
			{
				currentRule->AddWord("");
			}

			splitPos = p + 1;
			currentRule = stateHandle;
		}
		else if (*p == L'.' && *(p+1) == L'+') 
		{
			currentRule->AddWord("＊");

			p += 1;
			splitPos = p + 1;
		}
	}

	//最後の残り
	matchString = std::wstring(splitPos , 0 , (int) (p - splitPos) );
	if ( matchString.length() >= 1 &&str.length() >= 1 && *(p-1) != L')')
	{
		currentRule->AddWord(_W2A(matchString.c_str()) );
	}
	return true;
}



xreturn::r<bool> JuliusPlus::Create(MainWindow* poolMainWindow)
{
	//ルール構文
	//grammertop
	//	AddNestRule -- 
	//                 YobikakeRule
	//                       AddNestRule --- CommandRule
	//  AddNestRule -- TemporaryRule
	//
	this->YobikakeRuleHandle = new rule("YobikakeRule");
	this->CommandRuleHandle = new rule("CommandRule");

	this->TemporaryRuleHandle = new rule("TemporaryRule");

	this->Grammer.AddNestRule(this->YobikakeRuleHandle);
	this->Grammer.AddNestRule(this->TemporaryRuleHandle);
	this->YobikakeRuleHandle->AddNestRule(this->CommandRuleHandle);
	return true;
}

//呼びかけを設定します。
//設定したあと、 CommitRule() てしてね。
xreturn::r<bool> JuliusPlus::SetYobikake(const std::list<std::string> & yobikakeList)
{
	this->YobikakeListArray = yobikakeList;
	for(auto it = this->YobikakeListArray.begin();  this->YobikakeListArray.end() != it ; ++it)
	{
		this->YobikakeRuleHandle->AddWord(*it);
	}
	return true;
}

//認識結果で不感染なものを捨てる基準値を設定します。
xreturn::r<bool> JuliusPlus::SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter)
{
	this->TemporaryRuleConfidenceFilter = temporaryRuleConfidenceFilter;
	this->YobikakeRuleConfidenceFilter = yobikakeRuleConfidenceFilter;
	this->BasicRuleConfidenceFilter = basicRuleConfidenceFilter;

	return true;
}

//コマンドに反応する音声認識ルールを構築します
xreturn::r<bool> JuliusPlus::AddCommandRegexp(CallbackDataStruct & callback,const std::string & str)
{
	this->CallbackDictionary.push_back(callback);
	assert(this->CallbackDictionary.size() >= 1);
	return AddRegexp(CallbackDictionary.size() - 1,str , this->CommandRuleHandle);
}

//テンポラリルールに反応する音声認識ルールを構築します
xreturn::r<bool> JuliusPlus::AddTemporaryRegexp(CallbackDataStruct & callback,const std::string & str)
{
	this->CallbackDictionary.push_back(callback);
	assert(this->CallbackDictionary.size() >= 1);

	this->TemporaryRuleCount ++;
	return AddRegexp(CallbackDictionary.size() - 1,str, this->TemporaryRuleHandle);
}

//テンポラリルールをすべてクリアします
xreturn::r<bool> JuliusPlus::ClearTemporary()
{
	if (this->TemporaryRuleCount <= 0)
	{//現在テンポラリルールにルールが入っていないので、クリアをスキップします。
		return true;
	}

	//テンポラリールールを白紙に
	this->TemporaryRuleHandle->Clear();

	//クリアしたので、テンポラリルールの数はゼロになります。
	this->TemporaryRuleCount = 0;
	//ルールに変更が加わったのでコミットしないといけません。
	this->IsNeedUpdateRule = true;

	//コミット発動
	auto r = this->CommitRule();
	if (!r) return xreturn::error(r.getError());

	return true;
}

//構築したルールを音声認識エンジンにコミットします。
xreturn::r<bool> JuliusPlus::CommitRule()
{
	if (! this->IsNeedUpdateRule )
	{//アップデートする必要なし
		return true;
	}

	std::ofstream dfa("regexp_test.dfa");
	std::ofstream dict("regexp_test.dict");
	this->MakeJuliusRule(&dfa,&dict);

	this->JuliusStop();
	this->JuliusStart();

	//アップデートが終わったので再びルールに変更が加わるまではアップデートしない。
	this->IsNeedUpdateRule = false;

	return true;
}


void JuliusPlus::JuliusStop()
{
	if (this->recog)
	{
		//ストリームを閉じる
		j_close_stream(this->recog);

		//スレッド停止までまつ
		this->Thread->join();
		delete this->Thread;
		this->Thread = NULL;

		if (this->jconf)
		{
			//			j_jconf_free(this->jconf);
			//			this->jconf = NULL;
		}
		//メモリ開放
		//		j_recog_free(this->recog);
		this->recog = NULL;
	}
}

xreturn::r<bool> JuliusPlus::JuliusStart()
{
	assert(this->recog == NULL);
	assert(this->jconf == NULL);
	assert(this->Thread == NULL);
	char* argv[]={
		"juliusplus"
		,"-C"
		,"testmic.jconf"
	};
	int argc = 3;
	int ret;

	this->jconf = j_config_load_args_new(argc, argv);
	/* else, you can load configurations from a jconf file */
	//jconf = j_config_load_file_new(jconf_filename);
	if (this->jconf == NULL) 
	{
		return xreturn::error("Try `-help' for more information.\n");
	}

	/* 2. create recognition instance according to the jconf */
	/* it loads models, setup final parameters, build lexicon
	and set up work area for recognition */
	this->recog = j_create_instance_from_jconf(this->jconf);
	if (this->recog == NULL)
	{
		return xreturn::error("Error in startup(j_create_instance_from_jconf)\n");
	}
	callback_add(this->recog, CALLBACK_EVENT_SPEECH_READY, status_recready, NULL);
	callback_add(this->recog, CALLBACK_EVENT_SPEECH_START, status_recstart, NULL);
	callback_add(this->recog, CALLBACK_RESULT, output_result, NULL);
//	callback_add_adin(this->recog, CALLBACK_ADIN_TRIGGERED, record_sample_write, NULL);
//	callback_add(this->recog, CALLBACK_EVENT_SPEECH_STOP, record_sample_close, NULL);

	// Initialize audio input
	if (j_adin_init(this->recog) == FALSE) 
	{
		return xreturn::error("Error in startup(j_adin_init)\n");
	}

	//output system information to log
	//j_recog_info(this->recog);
	ret = j_open_stream(recog, NULL);
	if(ret < 0)
	{
		return xreturn::error("Error in startup(j_open_stream)\n");
	}

	this->Thread = new boost::thread( [&]()
	{
		j_recognize_stream(recog);
	} );
	return true;
}


