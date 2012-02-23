#include "JuliusPlus.h"
#define issjiskanji(c)	((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))

template<typename _INT> static const std::string num2str(_INT i)
{
	std::stringstream out;
	out << i;
	return out.str();
}


void JuliusPlus::OnStatusRecready(Recog *recog)
{
	if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
		fprintf(stderr,"マイクで喋ってください。エンターキーを押すと、プログラムを終了します。\r\n");
		fprintf(stderr,"----コマンド----\r\n");
		fprintf(stderr,"コンピュータ、電気つけて\r\n");
		fprintf(stderr,"コンピュータ、電気けして\r\n");
		fprintf(stderr,"コンピュータ、エアコンつけて\r\n");
		fprintf(stderr,"コンピュータ、エアコンけして\r\n");
		fprintf(stderr,"コンピュータ、どいつむら\r\n");
		fprintf(stderr,"それっぽいのだと マッチ!! ってでます。\r\n");

		fprintf(stderr, "<<< please speak >>>");

		//認識したwaveファイルを消去します。
		this->WaveFileData.clear();
	}
}
void JuliusPlus::OnStatusRecstart(Recog *recog)
{
	if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
		fprintf(stderr, "\r                    \r");
	}
}

//認識結果の一部を waveで保存する.
void JuliusPlus::WaveCutter(Recog *recog,int all_frame,int startfream,int endfream,const std::string& filename) const
{
	if (startfream == 0 && endfream == 0)
	{//all
		endfream = this->WaveFileData.size();
	}
	else
	{
		if (endfream < startfream)
		{
			return ;
		}
	}
	int allsize = this->WaveFileData.size();
	int of_fream = allsize / all_frame;
	startfream *= of_fream;
	endfream *= of_fream;
	//recog->jconf->input.framesize;

	FILE *recfile_fp = wrwav_open( (char*) filename.c_str(), recog->jconf->input.sfreq);
	if(recfile_fp)
	{
		wrwav_data(recfile_fp,(SP16*) ((&this->WaveFileData[0]) + startfream), endfream - startfream);
		wrwav_close(recfile_fp);
	}
}

//ディクテーションフィルターで呼びかけ文字列が入っているか確認する.
bool JuliusPlus::checkYobikake(const std::string & dictationString) const
{
	auto it = this->YobikakeListArray.begin();
	for( ; this->YobikakeListArray.end() != it ; ++it)
	{
		//ディクテーションフィルターで絞る
		if ( dictationString.find( *it ) != std::string::npos )
		{
			break;
		}
	}
	return (this->YobikakeListArray.end() != it);
}

//呼びかけがあれば、waveで保存し、もう一つのjuliusインスタンスで再チェックする.
bool JuliusPlus::checkDictation(Recog *recog,const OncSentence* sentence) const
{
	if (!this->UseDictationFilter)
	{//チェックをしないのなら常に true 正解を返す.
		return true;
	}
	auto it3 = sentence->nodes.begin();
	if (it3 == sentence->nodes.end() )
	{
		return false;
	}
	if (! checkYobikake( (*it3)->word ) )
	{
		return false;
	}
	//認識した結果の最初の呼びかけの部分を保存.
	WaveCutter(recog,sentence->all_frame,(*it3)->begin_frame-1,(*it3)->end_frame,"__DictationCheck_temp.wav" );

	//もう一つの julius インスタンスで wave ファイルからの認識をやります.
	//これで過剰マッチを完全に叩き落とします。
	//結果は、this->DictationCheckString に保存されます。
	int ret = j_open_stream(this->recogFile, "__DictationCheck_temp.wav");
	if(ret < 0)
	{
		return false;
	}
	j_recognize_stream(this->recogFile);

	//内容のチェック
	if (! checkYobikake( this->DictationCheckString ) )
	{
		std::cout << "#" << this->DictationCheckString << std::endl;
		return false;
	}

	return true;
}

void JuliusPlus::OnOutputResult(Recog *recog)
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
		std::stringstream dictNumber;
		std::stringstream cmscore;
		std::stringstream avgscore;
		std::stringstream begin_frame;
		std::stringstream end_frame;
		std::stringstream plus_node_score;

		for( auto it2 = (*it)->nodes.begin() ; it2 != (*it)->nodes.end() ; ++it2)
		{
			word << (*it2)->word << " ";
			yomi << (*it2)->yomi << " ";
			dictNumber << (*it2)->dictNumber << " ";
			cmscore << (*it2)->cmscore << " ";
			avgscore << (*it2)->avgscore << " ";
			begin_frame << (*it2)->begin_frame << " ";
			end_frame << (*it2)->end_frame << " ";
			plus_node_score << (*it2)->plus_node_score << " ";
		}
		//検出した呼びかけをもう一度再検証する。
		bool dictationCheck = checkDictation(recog,*it);
		std::cout << i << ". " << "plus_sentence_score:" << (*it)->plus_sentence_score << " score:" << (*it)->score << " dictcheck:" << dictationCheck << std::endl;
		if (dictationCheck && (*it)->plus_sentence_score >= 0.7)
		{
			std::cout << " マッチ!!!!!!!!!!!!!" << std::endl;
		}

		std::cout << " word       :" << word.str() << std::endl;
		std::cout << " yomi       :" << yomi.str() << std::endl;
		std::cout << " dictNumber :" << dictNumber.str() << std::endl;
		std::cout << " cmsscore   :" << cmscore.str() << std::endl;
		std::cout << " avgscore   :" << avgscore.str() << std::endl;
		std::cout << " begin_frame:" << begin_frame.str() << std::endl;
		std::cout << " end_frame  :" << end_frame.str() << std::endl;
		std::cout << " plus_node_s:" << plus_node_score.str() << std::endl;
		std::cout << std::endl;

		if (dictationCheck && (*it)->plus_sentence_score >= this->BasicRuleConfidenceFilter )
		{//マッチしているのならコールバックを読んであげよう。
			const CallbackDataStruct* callback = SearchCallbackWhereDFA(*it);
			if (callback == NULL)
			{
				std::cout << " コールバックが登録されていません" << std::endl;
			}
			else
			{
				if (! callback->Enable )
				{
					std::cout << " 有効なコールバックではありません" << std::endl;
				}
				else
				{
					std::cout << " コールバックを呼び出します" << std::endl;
					callback->callbackFunctionalObject();
				}
			}
		}

		if (i >= 1)
		{
			break;
		}
	}
	std::cout << std::flush;

	FreeAllSentenceList(&allSentence);
}

//juliusの認識結果と、plusルールに指定されたコールバックを見比べて、
//正しいであろう、コールバック構造体を返します。
const CallbackDataStruct* JuliusPlus::SearchCallbackWhereDFA(const OncSentence* juliusPlusResult) const
{
	if ( juliusPlusResult->nodes.size() <= 1 )
	{//呼びかけ + コマンドと2つのものを見るので、それ以下の物には興味ない
		return NULL;
	}
	auto resultIT = juliusPlusResult->nodes.begin();
	resultIT ++; //最初は呼びかけなので無視する。

	int dict = (*resultIT)->dictNumber;
	auto dicIT = CallbackDictionary.find(dict);
	if (dicIT == CallbackDictionary.end() )
	{//マッチせず
		return NULL;
	}
	return dicIT->second;
}


//認識waveファイルの断片が入ります。
void JuliusPlus::OnRecordAdinTrigger(Recog *recog, SP16 *speech, int samplenum)
{
	this->WaveFileData.insert(WaveFileData.end() , speech , speech + samplenum);
}
void JuliusPlus::OnSpeechStop(Recog *recog)
{
}
void JuliusPlus::OnSpeechPause(Recog *recog)
{
}

void JuliusPlus::OnSpeechResume(Recog *recog)
{
}

void JuliusPlus::OnSpeechPauseFunction(Recog *recog)
{
	adin_end(recog->adin);

	//pause

	adin_begin(recog->adin,NULL);
	j_request_resume(recog);
}

//結果リストの開放
void JuliusPlus::FreeAllSentenceList(std::list<OncSentence*> *allSentence) const
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

//plusのスコアを計算します。
float JuliusPlus::computePlusScore(const std::list<OneNode*>& nodes,float score,int sentnum,float mseclen) const
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

//recog から plus 形式に変換します。
//メモリを余計に消費しますが、こちらのほうが見やすいと思います。
void JuliusPlus::convertResult(const Recog *recog, std::list<OncSentence*> *allSentence) const
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

			//開始が取れないものは破棄.
			if ( strcmp(winfo->woutput[seq[0]]  , "<s>") != 0 )
			{
				continue;
			}

			//一文
			OncSentence * oneSentence = new OncSentence;
			// output word sequence like Julius 
			//0 , [1 , 2, 3, 4], 5, と先頭と最後を除いている、開始終端、記号を抜くため
			for(int i=1;i<seqnum-1;i++)
			{
				//1単語 --> 単語の集合が文になるよ
				int i_seq = seq[i];
				auto oneNode = new OneNode();
				oneNode->word = winfo->woutput[i_seq];
				//dict 対応している番号(dfaファイルの左側1行目の数字)
				oneNode->dictNumber = atoi(winfo->wname[i_seq]);

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
			//「が」、へんてこな文章を入れても、スコアが高くなってしまうし、長い文章を入れるとスコアが絶望的に低くなる
			//ので、そのままだと使えない。
			oneSentence->score = s->score;
			oneSentence->all_frame = r->result.num_frame;
			//多少使えるスコアを計算します。
			oneSentence->plus_sentence_score = computePlusScore(oneSentence->nodes,s->score,r->result.sentnum,mseclen);

			allSentence->push_back(oneSentence);
		}
	}
}


JuliusPlus::JuliusPlus()
{
	this->jconf = NULL;
	this->recog = NULL;
	this->Thread = NULL;
	this->recogFile = NULL;
	this->jconfFile = NULL;

	this->LocalCaptureRuleNodeCount = 0;
	this->GlobalRuleNodeCount = 0;

	this->Grammer = NULL;
}

JuliusPlus::~JuliusPlus()
{
	JuliusStop();

	delete this->Grammer;
	this->Grammer = NULL;
}

bool JuliusPlus::MakeJuliusRule(const JuliusPlusRule* toprule,bool no_nest , std::ostream* dfa , std::ostream* dict  )
{
	struct reverse_automaton
	{
		//出力する dfaハンドル
		std::ostream* dfa;
		//出力する dictハンドル
		std::ostream* dict;
		//callbackとdfaマッチングテーブル
		std::map<int,const CallbackDataStruct*>* callbackMatch;

		//次の dfa 番号
		int dfaNumber ;
		//次の dict 番号
		int dictNumber ;

		//ゴミルールのdict
		int dictGomiNumer ;

	
		void fire(const JuliusPlusRule* toprule,bool no_nest)
		{
			this->dfaNumber = 3;
			this->dictNumber = 3;
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

			//ゴミを入れることで juliusの確率計算がかたよるバグを回避する.
			//あ-ん までの一文字のごみを作成します。
			*dfa << firstRuleNumber << " " << this->dictGomiNumer << " " << 1 << " " << 0 << " " << 0 << std::endl;
			unsigned char gomi[4]; strcpy((char*)gomi, "あ");
			while( 1 )
			{
				if (strcmp( (char*)gomi,"ゃ") == 0 || strcmp( (char*)gomi,"ゅ") == 0 || strcmp( (char*)gomi,"ょ") == 0 || strcmp( (char*)gomi,"ぃ") == 0 )
				{//julius的にこれらの文字単体だとエラーになるらしい。
					//nop
				}
				else
				{
					std::string word = (char*) gomi;
					std::string yomi = JuliusPlusRule::convertYomi(word);
					*dict << this->dictGomiNumer	<< "\t" << "[gomi]" << "\t" << yomi << std::endl;
				}

				if (gomi[1] == 0xff) gomi[0] ++; //繰り上がり
				gomi[1] ++;

				if (strcmp( (char*)gomi,"ん") == 0)
				{
					break;
				}
			}

			if (!no_nest)
			{//通常のネストするルーチン
				func(toprule,1,0 , firstRuleNumber);
			}
			else
			{//ネストをしないルーチン 単語だけ処理
				nonest_func(toprule,1,0 , firstRuleNumber);
			}
		}

		//rule   処理するルール
		//number ルールに割り振られるべき番号 dfaNumber 番号。
		//backNumber このルールの親ルールの番号
		//termNumber このルールがどこにも遷移しない終端ルールだった時に進む番号
		void func(const JuliusPlusRule* currentrule, int number,int backNumber,int termNumber)
		{
			assert(currentrule);
			auto words = currentrule->getWords();
			auto nestrules = currentrule->getNestRules();
			auto termRule = currentrule->getTermRule();

			if (words->size() <= 0 )
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
				InsertCallback(currentrule,dictNumber);
			}

			//term ruleがあれば先に実行する。
			if (termRule == NULL)
			{//終端ノードなし
				if (nestrules->size() <= 0 && words->size() >= 1)
				{//単語だけのノードの場合、終端になる。
					*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
					if (currentrule->getName() == "YobikakeRule")
					{
						*dfa << termNumber << " " << this->dictGomiNumer << " " << number << " " << 0 << " " << 0 << std::endl;
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

		//コールバックとdfaを一緒にして記録する。
		void InsertCallback(const JuliusPlusRule* currentrule,int dictNumber)
		{
			const CallbackDataStruct* callback = currentrule->getCallback();
			if ( callback->Enable )
			{
				if( this->callbackMatch->find(dictNumber) == this->callbackMatch->end() )
				{
					this->callbackMatch->insert( std::pair<int,const CallbackDataStruct*>(dictNumber,callback) );
				}
			}
		}

		//rule   処理するルール
		//number ルールに割り振られるべき番号 dfaNumber 番号。
		//backNumber このルールの親ルールの番号
		//termNumber このルールがどこにも遷移しない終端ルールだった時に進む番号
		void nonest_func(const JuliusPlusRule* currentrule, int number,int backNumber,int termNumber)
		{
			assert(currentrule);
			auto words = currentrule->getWords();
			auto nestrules = currentrule->getNestRules();
			auto termRule = currentrule->getTermRule();

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

			*dfa << termNumber << " " << dictNumber << " " << number << " " << 0 << " " << 0 << std::endl;
			if (currentrule->getName() == "YobikakeRule")
			{
				*dfa << termNumber << " " << this->dictGomiNumer << " " << number << " " << 0 << " " << 0 << std::endl;
			}
		}
	} maton;
	maton.callbackMatch = &this->CallbackDictionary;
	maton.dfa = dfa;
	maton.dict = dict;
	maton.fire( toprule ,no_nest);

	return true;
}

//音声認識ルールを構築します。 正規表現にも対応しています。
xreturn::r<bool> JuliusPlus::AddRegexp(const CallbackDataStruct & callback,const std::string & str ,JuliusPlusRule* stateHandle ) 
{
//unixへの移植を考えて wchar_tはやめることにした。
//unix で UTF8 を使っている限りはマルチバイトに悩むことはないでしょう。 EUCとかレガシーなシステムは知らん。
//	_USE_WINDOWS_ENCODING;

	//一番最初だけ正規表現の構文変換をかける.
	// .+ --> (:?.*)
	// (まる|さんかく)? --> (まる|さんかく|)   正しい正規表現としてはエラーだが、このエンジンの場合容認する.
	// なのは? --> なの(は|)
	std::string optstr;
	optstr.reserve(str.size());

	//処理を簡単にするために、UTF-16でループします。
	for( const char * p = str.c_str()  ; *p ; ++p )
	{
		if ( issjiskanji(*p) )
		{
			optstr += *p;
			optstr += *(p+1);
			p ++;
			continue;
		}

		if ( *p == '.' && *(p+1) == '+')
		{ // .+ --> (:?.*)
			optstr += "(?:.+)";
			++p;
		}
		else if (*p == '(' && *(p+1) == '?' && *(p+2) == ':' )
		{
			optstr += "(?:";
			p+=2;
		}
		else if (*(p+1) == '?')
		{
			if (*p == ')')
			{// (まる|さんかく)? --> (まる|さんかく|)
				optstr += "|)";
			}
			else 
			{// なのは? --> なの(は|)
				optstr += std::string("(?:") + *p + "|)";
			}
			++p;
		}
		else if (*p == '*' || *p == '+' || *p == '.' || *p == '[' || *p == ']')
		{
			//            throw exception(std::string("") + "現在は、メタ文字 " + p + " は利用できません。利用可能なメタ文字 () | .+ ?");
			return xreturn::error(std::string("") + "現在は、メタ文字 " + p + " は利用できません。利用可能なメタ文字 () | .+ ?");
		}
		else
		{
			optstr += *p;
		}
	}


	this->LocalCaptureRuleNodeCount = 1;
	JuliusPlusRule* firstNestRule = new JuliusPlusRule(this->GlobalRuleNodeCount++,callback);
	stateHandle->AddNestRule(firstNestRule);
	return AddRegexpImpl(callback,optstr, firstNestRule);
}

//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
xreturn::r<bool> JuliusPlus::AddRegexpImpl(const CallbackDataStruct & callback,const std::string & str, JuliusPlusRule*  stateHandle)
{
//	_USE_WINDOWS_ENCODING;
	std::string matchString;

	//正規表現をパースしながら回す.
	const char * p = str.c_str();
	const char * splitPos = p;
	JuliusPlusRule*  currentRule = stateHandle;
	for(  ; *p ; ++p )
	{
		if ( issjiskanji(*p) )
		{
			p ++;
			continue;
		}

		if (*p == '(')
		{
			//閉じ括弧まで飛ばす. )
			int nest = 1;
			const char* n = p + 1;
			for( ; *n  ; ++n )
			{
				if ( issjiskanji(*n) )
				{
					n ++;
					continue;
				}
				if (*n == '(')
				{
					++nest ;
				}
				else if (*n == ')')
				{
					--nest ;
					if (nest <= 0)
					{
						break;
					}
				}
			}

			//ネストする前の部分
			matchString = std::string(splitPos, 0 ,p - splitPos);


			//ネストしている部分を格納するルールを作る.
			JuliusPlusRule*  nestRule;

			//キャプチャー？
			std::string captureNodeName;
			if (*(p+1) == '?' && *(p+2) == ':')
			{
				p += 2;
			}
			else
			{
				captureNodeName = num2str(this->LocalCaptureRuleNodeCount++) + ":" + num2str(this->GlobalRuleNodeCount) ;
			}
			//お目当ての()ネストを格納する部分を作る
			if (captureNodeName.length() <= 0)
			{
				nestRule = new JuliusPlusRule(this->GlobalRuleNodeCount ++ , callback);
			}
			else
			{
				nestRule = new JuliusPlusRule( captureNodeName , callback);
			}

			if (matchString.empty())
			{//先行文字列が空
				currentRule->AddNestRule(nestRule);
			}
			else
			{//文字列だけを入れると OR扱いになってしまうので、ネストを格納するネストを作らないとダメ。
				JuliusPlusRule* firstNestRule = new JuliusPlusRule(this->GlobalRuleNodeCount ++,callback);
				firstNestRule->AddWord(matchString);

				firstNestRule->AddNestRule(nestRule);
				currentRule->AddNestRule(firstNestRule);
			}

			//かっこの後にも構文が連続する場合、そのツリーを作成する.
			if (*n == '\0'  || *(n+1) == '\0' || *(n+1) == '|')
			{//閉じかっこで構文がとまる場合はそこで終端
				//nop
			}
			else
			{//さらに構文が続いている場合
				JuliusPlusRule * afterRule = new JuliusPlusRule( this->GlobalRuleNodeCount++,callback);
				nestRule->SetCombineRule(afterRule);

				currentRule = afterRule;
			}

			//ネストしているルールを再帰して実行.
			matchString = std::string(p+1 , 0 , (int) (n - p - 1) );
			auto r = this->AddRegexpImpl(callback,matchString, nestRule);
			if(!r)
			{
				return xreturn::errnoError(r);
			}

			p = n ;
			splitPos = n + 1;  //+1は最後の ) を飛ばす. iは forの ++i で i == splitPos となる。(わかりにくい)
		}
		else if (*p == '|') 
		{
			matchString = std::string(splitPos,0 , (int) (p - splitPos));
			if (matchString.length() >= 1)
			{
				currentRule->AddWord(matchString);
			}
			//空分岐 (A|) のような場合、空ノードを入れる.
			if (matchString.length() <= 0 || *(p+1) == '\0' )
			{
				assert(0); //not implement! 未実装!
				currentRule->AddWord("");
			}

			splitPos = p + 1;
			currentRule = stateHandle;
		}
		else if (*p == '.' && *(p+1) == '+') 
		{
			assert(0); //not implement! 未実装!
//			currentRule->AddWord("＊");

			p += 1;
			splitPos = p + 1;
		}
	}

	//最後の残り
	matchString = std::string(splitPos , 0 , (int) (p - splitPos) );
	if ( matchString.length() >= 1 &&str.length() >= 1 && *(p-1) != ')')
	{
		currentRule->AddWord(matchString );
	}
	return true;
}



xreturn::r<bool> JuliusPlus::Create(MainWindow* poolMainWindow)
{
	assert(this->Grammer == NULL);
	//ルール構文
	//grammertop
	//	AddNestRule -- 
	//                 YobikakeRule
	//                       AddNestRule --- CommandRule
	//  AddNestRule -- TemporaryRule
	//
	this->Grammer = new JuliusPlusRule("topgrammer",this->DummyCallback);
	this->YobikakeRuleHandle = new JuliusPlusRule("YobikakeRule",this->DummyCallback);
	this->CommandRuleHandle = new JuliusPlusRule("CommandRule",this->DummyCallback);

	this->TemporaryRuleHandle = new JuliusPlusRule("TemporaryRule",this->DummyCallback);

	//全部 Grammer に関連付けます。
	//Grammer が delete されれば、残りもすべてデリートされます。
	this->Grammer->AddNestRule(this->YobikakeRuleHandle);
	this->Grammer->AddNestRule(this->TemporaryRuleHandle);
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
		this->YobikakeRuleHandle->AddWord(*it );
	}
	return true;
}

//認識結果で不完全なものを捨てる基準値を設定します。
//xreturn::r<bool> JuliusPlus::SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter,bool useDictationFilter)
xreturn::r<bool> JuliusPlus::SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter,bool useDictationFilter)
{
//	this->TemporaryRuleConfidenceFilter = temporaryRuleConfidenceFilter;
	this->TemporaryRuleConfidenceFilter = 0; //今回のデモでは使わない
	this->YobikakeRuleConfidenceFilter = yobikakeRuleConfidenceFilter;
	this->BasicRuleConfidenceFilter = basicRuleConfidenceFilter;
	this->UseDictationFilter = useDictationFilter;

	return true;
}

//コマンドに反応する音声認識ルールを構築します
xreturn::r<bool> JuliusPlus::AddCommandRegexp(const std::string & str,const CallbackDataStruct & callback)
{
	return AddRegexp(callback,str , this->CommandRuleHandle);
}

//テンポラリルールに反応する音声認識ルールを構築します
xreturn::r<bool> JuliusPlus::AddTemporaryRegexp(const std::string & str,const CallbackDataStruct & callback)
{
	this->TemporaryRuleCount ++;
	return AddRegexp(callback,str, this->TemporaryRuleHandle);
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

	//マイクから入力用
	std::ofstream dfa("regexp_test.dfa");
	std::ofstream dict("regexp_test.dict");
	this->MakeJuliusRule(this->Grammer,false, &dfa,&dict);

	//ディクテーションフィルター用
	std::ofstream dfaFile("regexp_test_file.dfa");
	std::ofstream dictFile("regexp_test_file.dict");
	this->MakeJuliusRule(this->YobikakeRuleHandle,true,&dfaFile,&dictFile);

	this->JuliusStop();
	this->JuliusStart();
	this->JuliusFileStart();

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
			//これで開放すると、 j_recog_free で落ちる・・・
			//			j_jconf_free(this->jconf);
						this->jconf = NULL;
		}
		//メモリ開放
		j_recog_free(this->recog);
		this->recog = NULL;
	}

	if (this->recogFile)
	{
		if (this->jconfFile)
		{
			//これで開放すると、 j_recog_free で落ちる・・・
			//			j_jconf_free(this->jconfFile);
						this->jconfFile = NULL;
		}

		j_recog_free(this->recogFile);
		this->recogFile = NULL;
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
	struct _ref{
		static void status_recready(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnStatusRecready(recog);
		}
		static void status_recstart(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnStatusRecstart(recog);
		}
		static void output_result(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnOutputResult(recog);
		}
		static void record_adin_trigger(Recog *recog, SP16 *speech, int samplenum, void *_this)
		{
			((JuliusPlus*)_this)->OnRecordAdinTrigger(recog,speech,samplenum);
		}
		static void speech_stop(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnSpeechStop(recog);
		}
		static void speech_pause(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnSpeechPause(recog);
		}
		static void speech_resume(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnSpeechResume(recog);
		}
		static void speech_pause_function(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnSpeechPauseFunction(recog);
		}
	};
	callback_add(this->recog, CALLBACK_EVENT_SPEECH_READY, _ref::status_recready, this);
	callback_add(this->recog, CALLBACK_EVENT_SPEECH_START, _ref::status_recstart, this);
	callback_add(this->recog, CALLBACK_RESULT, _ref::output_result, this);
	callback_add_adin(this->recog, CALLBACK_ADIN_TRIGGERED, _ref::record_adin_trigger, this);
	callback_add(this->recog, CALLBACK_EVENT_SPEECH_STOP, _ref::speech_stop, this);
	callback_add(this->recog, CALLBACK_EVENT_PAUSE, _ref::speech_pause, this);
	callback_add(this->recog, CALLBACK_EVENT_RESUME, _ref::speech_resume, this);
	callback_add(this->recog, CALLBACK_PAUSE_FUNCTION,  _ref::speech_pause_function, this);

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

//ディクテーションフィルター利用時の認識
void JuliusPlus::OnOutputResultFile(Recog *recog)
{
	this->DictationCheckString = "";
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

			//呼びかけを取得します。
			for(int i = 0 ; i < seqnum ; i ++)
			{
				//ゴミデータは無視します。
				if ( strcmp(winfo->woutput[seq[i]]  , "<s>") == 0 || 
					 strcmp(winfo->woutput[seq[i]]  , "</s>") == 0 || 
					 strcmp(winfo->woutput[seq[i]]  , "gomi") == 0 
					)
				{
					continue;
				}
				//たぶんこれが呼びかけ。
				this->DictationCheckString = winfo->woutput[seq[i]];
			}
			return ;
		}
	}
	return ;
}


xreturn::r<bool> JuliusPlus::JuliusFileStart()
{
	assert(this->recogFile == NULL);
	assert(this->jconfFile == NULL);
	char* argv[]={
		"juliusplus"
		,"-C"
		,"testfile.jconf"
	};
	int argc = 3;

	this->jconfFile = j_config_load_args_new(argc, argv);
	/* else, you can load configurations from a jconf file */
	//jconf = j_config_load_file_new(jconf_filename);
	if (this->jconfFile == NULL) 
	{
		return xreturn::error("Try `-help' for more information.\n");
	}

	/* 2. create recognition instance according to the jconf */
	/* it loads models, setup final parameters, build lexicon
	and set up work area for recognition */
	this->recogFile = j_create_instance_from_jconf(this->jconfFile);
	if (this->recogFile == NULL)
	{
		return xreturn::error("Error in startup(j_create_instance_from_jconf)\n");
	}
	struct _ref{
		static void output_result(Recog *recog, void *_this)
		{
			((JuliusPlus*)_this)->OnOutputResultFile(recog);
		}
	};
	callback_add(this->recogFile, CALLBACK_RESULT, _ref::output_result, this);

	// Initialize audio input
	if (j_adin_init(this->recogFile) == FALSE) 
	{
		return xreturn::error("Error in startup(j_adin_init)\n");
	}
	//以上、準備だけしておいて、
	//認識ルーチンは、後から呼びます。
/*
	int ret = j_open_stream(recogFile, "nano.wav");
	if(ret < 0)
	{
		return xreturn::error("Error in startup(j_open_stream)\n");
	}

	j_recognize_stream(recogFile);

	ret = j_open_stream(recogFile, "nano.wav");
	if(ret < 0)
	{
		return xreturn::error("Error in startup(j_open_stream)\n");
	}

	j_recognize_stream(recogFile);
*/
	return true;
}

