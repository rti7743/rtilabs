#pragma once;
#ifdef _MSC_VER
#include <Winsock2.h>
#include <mswsock.h>

#include <windows.h>

 // This check can be removed if you only build for Windows
#pragma warning (push)
#pragma warning (disable : 4005)
#include <intsafe.h>
#include <stdint.h>
#pragma warning (push)
#pragma warning (default : 4005)
#endif //_MSC_VER

#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include "JuliusPlusRule.h"
#include "xreturn.h"
#include <boost/thread.hpp>


extern "C"{
	#include <julius/juliuslib.h>
};

struct MainWindow{
};




class JuliusPlus
{
public:
	//1ノード単位 
	//ノードとは、 dict の単語単位、1つの単語単位だと思ってください。
	//単語が集まって、文章ができます。
	struct OneNode
	{
		std::string word;
		std::string yomi;

		int dictNumber;

		float cmscore;	//確率 juliusがいう「ライバルに比べた」単語信頼度。
						//ライバルに比べたというのがミソで、ライバルがほとんどいない単語だと
						//ふつーに 1.0 (100%)になってしまうという不思議仕様の使えない子。ダメダメすぐる。
						//julius plusでは、gomi単語をばらまいてライバルを擬似的に作るという荒業をおこなっている。
		float avgscore;	//-walign で取得できる単語のスコア。 尤度？
		int begin_frame;
		int end_frame;

		float plus_node_score; // avgscore / (end_frame-begin_frame) にしたもの。
	};
	//1文
	struct OncSentence
	{
		std::list<OneNode*>        nodes;

		float score;				//julus公式のスコア
		float plus_sentence_score;	//いろいろ合算して、そこそこ信頼出来る数字にしたもの
		int   all_frame;			//全フレーム数
	};

public:
	JuliusPlus();
	virtual ~JuliusPlus();

	//構築
	virtual xreturn::r<bool> Create(MainWindow* poolMainWindow) ;
	//呼びかけを設定します。
	//設定したあと、 CommitRule() てしてね。
	virtual xreturn::r<bool> SetYobikake(const std::list<std::string> & yobikakeList) ;
	//認識結果で不感染なものを捨てる基準値を設定します。
	virtual xreturn::r<bool> SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter,bool useDictationFilter) ;
	//コマンドに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> AddCommandRegexp(const std::string & str,const CallbackDataStruct & callback) ;
	//テンポラリルールに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> AddTemporaryRegexp(const std::string & str,const CallbackDataStruct & callback) ;
	//テンポラリルールをすべてクリアします
	virtual xreturn::r<bool> ClearTemporary() ;
	//構築したルールを音声認識エンジンにコミットします。
	virtual xreturn::r<bool> CommitRule() ;

private:
	//音声認識ルールを構築します。 正規表現にも対応しています。
	xreturn::r<bool> JuliusPlus::AddRegexp(const CallbackDataStruct & callback,const std::string & str ,JuliusPlusRule* stateHandle ) ;
	//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
	xreturn::r<bool> JuliusPlus::AddRegexpImpl(const CallbackDataStruct & callback,const std::string & str, JuliusPlusRule*  stateHandle);

	//juliusを稼働させます
	void JuliusPlus::JuliusStop();
	//juliusを停止させます
	xreturn::r<bool> JuliusPlus::JuliusStart();
	//juliusのwaveファイルから認識バージョンを起動します。
	xreturn::r<bool> JuliusPlus::JuliusFileStart();

	//juliusに食べさせる dfa と dict を生成します
	bool JuliusPlus::MakeJuliusRule(const JuliusPlusRule* toprule,bool no_nest ,std::ostream* dfa , std::ostream* dict  );

	//結果リストの開放
	void JuliusPlus::FreeAllSentenceList(std::list<OncSentence*> *allSentence) const;
	//plusのスコアを計算します。
	float JuliusPlus::computePlusScore(const std::list<OneNode*>& nodes,float score,int sentnum,float mseclen) const;
	//recog から plus 形式に変換します。
	//メモリを余計に消費しますが、こちらのほうが見やすいと思います。
	void JuliusPlus::convertResult(const Recog *recog, std::list<OncSentence*> *allSentence) const;
	//認識結果の一部を waveで保存する.
	void JuliusPlus::WaveCutter(Recog *recog,int all_frame,int startfream,int endfream,const std::string& filename) const;
	//ディクテーションフィルターで呼びかけ文字列が入っているか確認する.
	bool JuliusPlus::checkYobikake(const std::string & dictationString) const;
	//呼びかけがあれば、waveで保存し、もう一つのjuliusインスタンスで再チェックする.
	bool JuliusPlus::checkDictation(Recog *recog,const OncSentence* sentence) const;
	//juliusの認識結果と、plusルールに指定されたコールバックを見比べて、
	//正しいであろう、コールバック構造体を返します。
	const CallbackDataStruct* JuliusPlus::SearchCallbackWhereDFA(const OncSentence* juliusPlusResult) const;


	void JuliusPlus::OnStatusRecready(Recog *recog);
	void JuliusPlus::OnStatusRecstart(Recog *recog);
	void JuliusPlus::OnOutputResult(Recog *recog);
	void JuliusPlus::OnRecordAdinTrigger(Recog *recog, SP16 *speech, int samplenum);
	void JuliusPlus::OnSpeechStop(Recog *recog);
	void JuliusPlus::OnSpeechPause(Recog *recog);
	void JuliusPlus::OnSpeechResume(Recog *recog);
	void JuliusPlus::OnSpeechPauseFunction(Recog *recog);

	void JuliusPlus::OnOutputResultFile(Recog *recog);

	int LocalCaptureRuleNodeCount;
	int GlobalRuleNodeCount;

	double TemporaryRuleConfidenceFilter;
	double YobikakeRuleConfidenceFilter;
	double BasicRuleConfidenceFilter;
	bool UseDictationFilter;
	std::list<std::string> YobikakeListArray;

	JuliusPlusRule* TemporaryRuleHandle;
	JuliusPlusRule* YobikakeRuleHandle;
	JuliusPlusRule* CommandRuleHandle;
	int TemporaryRuleCount ;
	bool IsNeedUpdateRule;

	std::string DictationCheckString;

	std::map<int,const CallbackDataStruct*> CallbackDictionary;

	//plus専用の文法保持です。
	JuliusPlusRule* Grammer;
	//認識したwaveファイル
	std::vector<SP16> WaveFileData;
	//ダミーのコールバック
	CallbackDataStruct DummyCallback;

	//マイクから読み込んで実行するjulius
	Jconf *jconf;
	Recog *recog;
	//ディクテーションフィルターのためファイルからのjulius
	//正確には、ディクテーションではなくて、たくさんのゴミの中からの再チェック.
	Jconf *jconfFile;
	Recog *recogFile;

	//juliusをスレッドで動作させます。
	boost::thread *Thread;
};
