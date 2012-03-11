#include <Winsock2.h>
#include <mswsock.h>

#include <windows.h>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../liblinear/linear.h"
#include "LoadLibraryHelper.h"

extern "C"{
	#include <julius/juliuslib.h>
};



Jconf* g_jconfFile;
Recog * g_recogFile;

bool g_OKorBAD;
std::ostream* g_TrainFile;

//plusのスコアを計算します。
float computePlusScore(float cmscore,float score,int sentnum,float mseclen) 
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
//	float cmsplus = 0;
//	cmsplus = cmsplus / nodes.size();
//	cmsplus = (1 - cmsplus + 1) * 10; //反転させて10倍してペナルティとする。大きいほど罰則がでかい
//
//	plus_sentence_score *= cmsplus;	//cmscore によるペナルティを入れる.

	float cmsplus = (1 - cmscore + 1) * 10; //反転させて10倍してペナルティとする。大きいほど罰則がでかい
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

//仮説の数によるペナルティ
//仮説が多いと、それだけ判断に困っているということでペナルティを与えます。
//また、ゴミにマッチした場合はより大きなペナルティを与えます。
//しきいちは適当です。機械学習とか入れられればいいんだけど。
int countHypothesisPenalty(const Recog *recog) 
{
	float gomiCount = 0;
	for(RecogProcess* r = recog->process_list; r ; r=r->next) 
	{
		//ゴミを消します。
		if (! r->live || r->result.status < 0 ) 
		{
			continue;
		}
		//同一文章かどうかチェックする
		std::list<int> checkDict;

		auto winfo = r->lm->winfo;
		for(auto n = 0; n < r->result.sentnum; n++) 
		{ // for all sentences
			const auto s = &(r->result.sent[n]);
			const auto seq = s->word;
			const auto seqnum = s->word_num;

			//開始が取れないものはごみ
			if ( strcmp(winfo->woutput[seq[0]]  , "<s>") != 0 )
			{
				gomiCount +=1.0f;
				continue;
			}
			if (seqnum <= 2)
			{//start word end 
				gomiCount +=1.0f;
				continue;
			}
			//ゴミ文章ならペナルティ
			if ( strcmp(winfo->woutput[seq[0]]  , "gomi") == 0 )
			{
				gomiCount +=1.0f;
				continue;
			}
			//ふつーのコマンドは スタート符号 呼びかけ コマンド スタート符号終わり の4以上になるよ。
			//コマンドに エアコン(つけて|けして) のように、正規表現が入ると コマンド1 コマンド2 と分割される。
			int dict;
			if ( seqnum >= 4  )
			{
				//同一文章ならペナルティは少なくする  エアコン(つけて|けして) など同一文章
				dict = atoi(winfo->wname[seq[2]]);
			}
			else
			{
				//デモでは使わないがテンポラリルールの場合には、呼びかけが無いのでその分短くなる。
				dict = atoi(winfo->wname[seq[1]]);
			}
			if (dict <= 3)
			{//ゴミルール
				gomiCount +=1.0f;
				continue;
			}

			//すでに登場したノードかどうかしらべる
			if (std::find(checkDict.begin(),checkDict.end() , dict) != checkDict.end()	)
			{
				gomiCount += 0.2f;
				continue;
			}

			//新規の文章なら多少のペナルティ
			gomiCount += 0.5f;
			checkDict.push_back(dict);
		}
	}
	return (int)(gomiCount + 1.5f);
}

//読みを取得する
std::string ConvertYomi(const WORD_INFO * wordinfo,int index) 
{
	std::string yomi;

	char buf[64];
	for (int j=0;j<wordinfo->wlen[index];j++) 
	{
		yomi += center_name( (wordinfo->wseq[index][j]->name),buf);
	}
	return yomi;
}


//julius認識したデータ
void g_output_result(Recog *recog, void *dummy)
{
	//喋った時間の総数
	const float mseclen = (float)recog->mfcclist->param->samplenum * (float)recog->jconf->input.period * (float)recog->jconf->input.frameshift / 10000.0f;
	//仮説の数によるペナルティ
	const int hypothesisPenalty = countHypothesisPenalty(recog);

	//1:成功 2:失敗 
	if (g_OKorBAD)
	{
		*g_TrainFile << "1";
	}
	else
	{
		*g_TrainFile << "2";
	}
	for(const RecogProcess* r = recog->process_list; r ; r=r->next) 
	{
		//ゴミを消します。
		if (! r->live || r->result.status < 0 ) 
		{
			continue;
		}

		// output results for all the obtained sentences
		const auto winfo = r->lm->winfo;
		for(auto n = 0; n < r->result.sentnum; n++) 
		{ // for all sentences
			const auto s = &(r->result.sent[n]);
			const auto seq = s->word;
			const auto seqnum = s->word_num;

			int i_seq;
			// output word sequence like Julius 
			//0 , [1 , 2, 3, 4], 5, と先頭と最後を除いている、開始終端、記号を抜くため
			int i;
			for(i = 0;i<seqnum;i++)
			{
				//1単語 --> 単語の集合が文になるよ
				i_seq = seq[i];

				//開始と終了を飛ばす
				if (	strcmp(winfo->woutput[i_seq]  , "<s>") == 0 
					||  strcmp(winfo->woutput[i_seq]  , "</s>") == 0 
				){
					continue;
				}
				break;
			}
			if (i >= seqnum) 
			{
				continue;
			}
			//dict から plus側のrule を求める
			int dict = atoi(winfo->wname[i_seq]);

			//マッチよみがなを取得する
			std::string yomi = ConvertYomi(winfo,i_seq);

			//cmscoreの数字(このままでは使えない子状態)
			auto cmscore = s->confidence[i];
			//素性1 cmsscore
			*g_TrainFile << " 1:" << cmscore;

			//julius のスコア 尤度らしい。マイナス値。0に近いほど正しいらしい。
			//「が」、へんてこな文章を入れても、スコアが高くなってしまうし、長い文章を入れるとスコアが絶望的に低くなる
			//ので、そのままだと使えない。
			auto score = s->score;

			//素性2 文章スコア
			*g_TrainFile << " 2:" << score;

			auto all_frame = r->result.num_frame;

			//素性3 フレーム数
			*g_TrainFile << " 3:" << all_frame;

			//素性4 仮説の数によるペナルティ
			*g_TrainFile << " 4:" << hypothesisPenalty;

			//素性5 録音時間
			*g_TrainFile << " 5:" << mseclen;

			//多少使えるスコアを計算します。
			//			oneSentence->plus_sentence_score = computePlusScore(oneSentence->nodes,s->score,r->result.sentnum,mseclen);
			auto plus_sentence_score = computePlusScore(cmscore,s->score,hypothesisPenalty,mseclen);
			//素性6 plusスコア
			*g_TrainFile << " 6:" << plus_sentence_score;

			//素性7 サンプル数？
			*g_TrainFile << " 7:" << r->lm->am->mfcc->param->header.samplenum;
			//素性8 なにこれ
//			*g_TrainFile << " 8:" << r->lm->am->mfcc->param->header.wshift;
			//素性9 なにこれ
//			*g_TrainFile << " 9:" << r->lm->am->mfcc->param->veclen;
			//素性10～ これがきめてになった。
			int feature = 10;
			for(int vecI = 0 ; vecI < r->lm->am->mfcc->param->header.samplenum ;vecI++ )
			{
				for(int vecN = 0 ; vecN < r->lm->am->mfcc->param->veclen ;vecN++ )
				{
					*g_TrainFile << " " << feature++ << ":" << r->lm->am->mfcc->param->parvec[vecI][vecN];
				}
			}
			*g_TrainFile << std::endl;

			return ;
		}
	}

}


//julius 録音開始
bool JuliusReco(const std::string& fullname)
{
	int ret = j_open_stream(g_recogFile,(char*) fullname.c_str() );
	if(ret < 0)
	{
		return false;
	}
	j_recognize_stream(g_recogFile);
	return true;
}

//julius初期化
bool JuliusFileStart()
{
	const char* argv[]={
		"juliusplus"
		,"-C"
		,"testfile.jconf"
	};
	int argc = sizeof(argv)/sizeof(argv[0]);

	//julius は C関数だから、const外して char** にするしかない。
	g_jconfFile = j_config_load_args_new(argc, (char**)argv);
	/* else, you can load configurations from a jconf file */
	//jconf = j_config_load_file_new(jconf_filename);
	if (g_jconfFile == NULL) 
	{
		return false;
	}

	/* 2. create recognition instance according to the jconf */
	/* it loads models, setup final parameters, build lexicon
	and set up work area for recognition */
	g_recogFile = j_create_instance_from_jconf(g_jconfFile);
	if (g_recogFile == NULL)
	{
		return false;
	}

	callback_add(g_recogFile, CALLBACK_RESULT, g_output_result, 0);

	// Initialize audio input
	if (j_adin_init(g_recogFile) == FALSE) 
	{
		return false;
	}
	return true;
}

//成功と失敗データのディレクトリスキャン
bool ScanDir(const std::string & dir)
{
	//ディレクトリに格納されている情報を取得してマッチングしていきます。
	std::string path = dir + "\\*.wav";

	_finddata_t data;
	long handle = _findfirst( path.c_str() , &data );
	if (handle == -1)
	{
		return true;
	}
	do
	{
		const std::string filename = dir + "\\" + data.name;
		if (data.attrib & _A_SUBDIR)
		{
			continue;
		}
		bool r = JuliusReco(filename);
		if (!r)
		{
			//juliusがwave食べさせたらえらった。どうしよう・・・
		}
	}
	while( ! _findnext(handle,&data) );
	_findclose(handle);

	return true;
}

class XLMachineLearningLibliear
{
public:
	XLMachineLearningLibliear()
	{
		lib.Load("liblinear.dll");
		func_train = (def_train) lib.GetProcAddress("train");
		func_cross_validation = (def_cross_validation) lib.GetProcAddress("cross_validation");

		func_predict_values = (def_predict_values) lib.GetProcAddress("predict_values");
		func_predict = (def_predict) lib.GetProcAddress("predict");
		func_predict_probability = (def_predict_probability) lib.GetProcAddress("predict_probability");

		func_save_model = (def_save_model) lib.GetProcAddress("save_model");
		func_load_model = (def_load_model) lib.GetProcAddress("load_model");

		func_get_nr_feature = (def_get_nr_feature) lib.GetProcAddress("get_nr_feature");
		func_get_nr_class = (def_get_nr_class) lib.GetProcAddress("get_nr_class");
		func_get_labels = (def_get_labels) lib.GetProcAddress("get_labels");

		func_free_model_content = (def_free_model_content) lib.GetProcAddress("free_model_content");
		func_free_and_destroy_model = (def_free_and_destroy_model) lib.GetProcAddress("free_and_destroy_model");
		func_destroy_param = (def_destroy_param) lib.GetProcAddress("destroy_param");

		func_check_parameter = (def_check_parameter) lib.GetProcAddress("check_parameter");
		func_check_probability_model = (def_check_probability_model) lib.GetProcAddress("check_probability_model");
		func_set_print_string_function = (def_set_print_string_function) lib.GetProcAddress("set_print_string_function");

		this->Model = NULL;
	}
	virtual ~XLMachineLearningLibliear()
	{
		for(std::vector<feature_node*>::const_iterator it = ProblemX.begin() ; it != ProblemX.end() ; ++it )
		{
			delete [] *it;
		}
		FreeModel();
	}
	//ファイルから学習データを読み込む
	bool LoadTrain(const std::string& filename)
	{
		FILE * fp = fopen(filename.c_str() , "rb");
		if (!fp)
		{
			return false;
		}

		std::vector<char> buffer;
		buffer.resize(65535);
		while (! feof(fp) )
		{
			char * p = &buffer[0];
			fgets(p,buffer.size(), fp);
			if (*p == '#' || *p == '\n' || *p == '\0') continue; //コメントとか

			char * startN = p;

			//何個 : があるかを数えます。
			int featurecount = 0;
			for( ; *startN  ; ++startN ) 
			{
				if (*startN == ':') featurecount ++;
			}

			//クラス番号との区切り線
			startN = p;
			for( ; *p != ' ' && *p != '\t' && *p != '\n' && *p != '\0'; ++p );
			if (*p == '\0' || *p == '\n' ) continue;
			*p = '\0';

			//素性の数だけ確保(直列に確保しないとダメ)
			feature_node* feature_nodes = new feature_node[featurecount + 1]; //終端もあるので +1する

			//クラス番号
			const int classid = atoi(startN);
			ProblemY.push_back(classid);
			ProblemX.push_back(feature_nodes);

			feature_node* featureP = feature_nodes;
			for( ; 1 ; ++featureP )
			{
				//素性番号
				p ++;
				startN = p;
				for( ; *p != ':' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' ) break;
				*p = '\0';
				featureP->index = atoi(startN);

				//素性の値
				p ++;
				startN = p;
				for( ; *p != ' ' && *p != '\t' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' )
				{
					featureP->value = atof(startN);
					break;
				}
				else
				{
					*p = '\0';
					featureP->value = atof(startN);
				}
			}
			//終端マーク
			featureP->index = -1;
			featureP->value = 0;
		}
		fclose(fp);
		return true;
	}
	//教え込んだときと同じファイルフォーマットを使って、認識率を測定します。 デバッグ用
	bool DebugPredict(const std::string& filename,const std::string& logFilename,int* outall,int* outmatch)
	{
		FILE * fp = fopen(filename.c_str() , "rb");
		if (!fp)
		{
			return false;
		}
		FILE * logfp = fopen(logFilename.c_str() , "wb");
		if (!logfp)
		{
			return false;
		}
		int all = 0;
		int match = 0;

		std::vector<char> buffer;
		buffer.resize(65535);
		while (! feof(fp) )
		{
			char * p = &buffer[0];
			fgets(p,buffer.size(), fp);
			if (*p == '#' || *p == '\n' || *p == '\0') continue; //コメントとか

			char * startN = p;

			//何個 : があるかを数えます。
			int featurecount = 0;
			for( ; *startN  ; ++startN ) 
			{
				if (*startN == ':') featurecount ++;
			}

			//クラス番号との区切り線
			startN = p;
			for( ; *p != ' ' && *p != '\t' && *p != '\n' && *p != '\0'; ++p );
			if (*p == '\0' || *p == '\n' ) continue;
			*p = '\0';

			//どんな値だったかを保存しておきます。
			std::string feature_string = p + 1;

			//素性の数だけ確保(直列に確保しないとダメ)
			feature_node* feature_nodes = new feature_node[featurecount + 1]; //終端もあるので +1する

			//クラス番号
			const int classid = atoi(startN);

			feature_node* featureP = feature_nodes;
			for( ; 1 ; ++featureP )
			{
				//素性番号
				p ++;
				startN = p;
				for( ; *p != ':' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' ) break;
				*p = '\0';
				featureP->index = atoi(startN);

				//素性の値
				p ++;
				startN = p;
				for( ; *p != ' ' && *p != '\t' && *p != '\n'  && *p != '\0'; ++p );
				if (*p == '\0' || *p == '\n' )
				{
					featureP->value = atof(startN);
					break;
				}
				else
				{
					*p = '\0';
					featureP->value = atof(startN);
				}
			}
			//終端マーク
			featureP->index = -1;
			featureP->value = 0;


			//識別されてみます。
			int predictClassID = this->Predict(feature_nodes);

			//結果を測定します。
			if (predictClassID == classid)
			{
				match ++;
				fprintf(logfp,"OK %d %s" , predictClassID , feature_string.c_str() );
			}
			else
			{
				fprintf(logfp,"BAD %d %s" , predictClassID , feature_string.c_str() );
			}
			all ++;

			//メモリ解放を忘れずに。
			delete [] feature_nodes;
		}
		fclose(fp);

		fprintf(logfp,"\r\nAccuracy = %lf%% (%d/%d)" , ((double)match) * 100 / all  , match , all);
		*outall = all;
		*outmatch = match;
		return true;
	}
	//学習データの追加 (liblinear 辞退がオンラインできないらしいから仕方ないけども)
	void AppendTrain(int classID,const std::vector<feature_node>& newnodes)
	{
		feature_node* nodes = new feature_node[newnodes.size()+1]; //終端もあるので+1しときます。

		ProblemY.push_back(classID);
		ProblemX.push_back(nodes);

		for(std::vector<feature_node>::const_iterator it = newnodes.begin() ; it != newnodes.end() ; ++it )
		{
			*nodes++ = *it;
		}

		//終端マーク -1 を入れる。
		nodes->index = -1;
		nodes->index = 0;
	}
	//学習します。
	bool Train()
	{
		int maxindex = 0;
		for(std::vector<feature_node*>::const_iterator it = ProblemX.begin() ; it != ProblemX.end() ; ++it )
		{
			for(feature_node* nodes = *it; nodes->index != -1 ; ++nodes )
			{
				maxindex = max(nodes->index,maxindex);
			}
		}

		struct problem prob;
		prob.bias = -1;				//バイアス？初期化するときの引数で与えるらしいが・・・
		prob.l = ProblemY.size();	//行数
		prob.n = maxindex;	//一番でかい素性番号
		prob.y = &ProblemY[0];
		prob.x = &ProblemX[0];

		struct parameter param;
		param.solver_type = L2R_L2LOSS_SVC_DUAL;
		param.C = 1;
		param.eps = INFINITE; // あとで設定し直すよ
		param.nr_weight = 0;
		param.weight_label = NULL;
		param.weight = NULL;

		if(param.eps == INFINITE)
		{
			if(param.solver_type == L2R_LR || param.solver_type == L2R_L2LOSS_SVC)
				param.eps = 0.01;
			else if(param.solver_type == L2R_L2LOSS_SVC_DUAL || param.solver_type == L2R_L1LOSS_SVC_DUAL || param.solver_type == MCSVM_CS || param.solver_type == L2R_LR_DUAL)
				param.eps = 0.1;
			else if(param.solver_type == L1R_L2LOSS_SVC || param.solver_type == L1R_LR)
				param.eps = 0.01;
		}

		FreeModel();
		this->Model = func_train(&prob,&param);
//		this->Model = train(&prob,&param);
		return this->Model != NULL;
	}
	//学習したモデルからクラス番号を取得します。
	int Predict(const std::vector<feature_node>& params)
	{
		if (params.size() <= 0)
		{
			return 0;
		}
		if ( (params.rbegin())->index == -1 )
		{
			return func_predict(this->Model , &params[0]);
		}
		else
		{
			std::vector<feature_node> params2 = params;
			feature_node fin;
			fin.index = -1;
			fin.value = 0;
			params2.push_back(fin);

			return func_predict(this->Model , &params2[0]);
		}
	}
	//学習したモデルからクラス番号を取得します。
	int Predict(const feature_node* params)
	{
		return func_predict(this->Model , params);
	}
	bool SaveModel(const std::string& filename)
	{
		func_save_model(filename.c_str(),this->Model);
//		save_model(filename.c_str(),this->Model);
		return true;
	}
	void FreeModel()
	{
		if (this->Model)
		{
			func_free_model_content(this->Model);
//			free_model_content(this->Model);
			this->Model = NULL;
		}
	}

private:
	std::vector<int> ProblemY;
	std::vector<feature_node*> ProblemX;
	std::vector<feature_node>  RealFeature;
	model* Model;

	LoadLibraryHelper lib;
	typedef struct model* (*def_train)(const struct problem *prob, const struct parameter *param);
	typedef void (*def_cross_validation)(const struct problem *prob, const struct parameter *param, int nr_fold, int *target);

	typedef int (*def_predict_values)(const struct model *model_, const struct feature_node *x, double* dec_values);
	typedef int (*def_predict)(const struct model *model_, const struct feature_node *x);
	typedef int (*def_predict_probability)(const struct model *model_, const struct feature_node *x, double* prob_estimates);

	typedef int (*def_save_model)(const char *model_file_name, const struct model *model_);
	typedef struct model* (*def_load_model)(const char *model_file_name);

	typedef int (*def_get_nr_feature)(const struct model *model_);
	typedef int (*def_get_nr_class)(const struct model *model_);
	typedef void (*def_get_labels)(const struct model *model_, int* label);

	typedef void (*def_free_model_content)(struct model *model_ptr);
	typedef void (*def_free_and_destroy_model)(struct model **model_ptr_ptr);
	typedef void (*def_destroy_param)(struct parameter *param);

	typedef const char* (*def_check_parameter)(const struct problem *prob, const struct parameter *param);
	typedef int (*def_check_probability_model)(const struct model *model);
	typedef void (*def_set_print_string_function)(void (*print_func) (const char*));


	def_train func_train;
	def_cross_validation func_cross_validation;

	def_predict_values func_predict_values;
	def_predict func_predict;
	def_predict_probability func_predict_probability;

	def_save_model func_save_model;
	def_load_model func_load_model;

	def_get_nr_feature func_get_nr_feature;
	def_get_nr_class func_get_nr_class;
	def_get_labels func_get_labels;

	def_free_model_content func_free_model_content;
	def_free_and_destroy_model func_free_and_destroy_model;
	def_destroy_param func_destroy_param;

	def_check_parameter func_check_parameter;
	def_check_probability_model func_check_probability_model;
	def_set_print_string_function func_set_print_string_function;

};


int main()
{
	XLMachineLearningLibliear liblinear;
	{
		g_TrainFile = new std::ofstream("train.txt");

		if ( ! JuliusFileStart() )
		{
			return 1;
		}

		//失敗したデータの学習.
		g_OKorBAD = false;
		ScanDir("bad_wav");

		//成功したデータの学習.
		g_OKorBAD = true;
		ScanDir("ok_wav");

		delete g_TrainFile;
	}
	{
		g_TrainFile = new std::ofstream("test.txt");

		if ( ! JuliusFileStart() )
		{
			return 1;
		}

		//失敗したデータの学習.
		g_OKorBAD = false;
		ScanDir("test_bad_wav");

		//成功したデータの学習.
		g_OKorBAD = true;
		ScanDir("test_ok_wav");

		delete g_TrainFile;
	}
	//ファイルから学習データを読み込む
	liblinear.LoadTrain("train.txt");
	//学習する
	liblinear.Train();

	//テストする
	int all,match;
	liblinear.DebugPredict("train.txt" , "log_train.txt" , &all,&match);
	printf("学習したデータを再テスト: Accuracy = %.3lf%% (%d/%d)   詳細はlog_train.txt\r\n" , ((double)match) * 100 / all  , match , all);
	liblinear.DebugPredict("test.txt" , "log_test.txt" , &all,&match);
	printf("未知のデータでのテスト  : Accuracy = %.3lf%% (%d/%d)   詳細はlog_test.txt \r\n" , ((double)match) * 100 / all  , match , all);


	//学習モデルを保存する
	liblinear.SaveModel("train.dat");
#ifdef _MSC_VER
	printf("エンターキーで終了します。");
	//キー入力待ち
	std::string line;
	getline( std::cin, line );
#endif
	return 0;
}