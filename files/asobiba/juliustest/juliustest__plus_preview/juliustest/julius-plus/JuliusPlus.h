#pragma once;
#include <Winsock2.h>
#include <mswsock.h>

#include <windows.h>

#ifdef _MSC_VER // This check can be removed if you only build for Windows
#pragma warning (push)
#pragma warning (disable : 4005)
#include <intsafe.h>
#include <stdint.h>
#pragma warning (push)
#pragma warning (default : 4005)
#endif

#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include "rule.h"
#include "xreturn.h"
#include "windows_encoding.h"
#include <boost/thread.hpp>


extern "C"{
	#include <julius/juliuslib.h>
};

struct MainWindow{
};
struct CallbackDataStruct{
	int a;
	CallbackDataStruct(int aaa):a(aaa) {};
};


class JuliusPlus
{

public:
	JuliusPlus();
	virtual ~JuliusPlus();

	rule Grammer;

	bool JuliusPlus::MakeJuliusRule(std::ostream* dfa , std::ostream* dict  );
	//音声認識ルールを構築します。 正規表現にも対応しています。
	xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) ;
	//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
	xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle);

	virtual xreturn::r<bool> Create(MainWindow* poolMainWindow) ;
	//呼びかけを設定します。
	//設定したあと、 CommitRule() てしてね。
	virtual xreturn::r<bool> SetYobikake(const std::list<std::string> & yobikakeList) ;
	//認識結果で不感染なものを捨てる基準値を設定します。
	virtual xreturn::r<bool> SetRecognitionFilter(double temporaryRuleConfidenceFilter,double yobikakeRuleConfidenceFilter,double basicRuleConfidenceFilter) ;
	//コマンドに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> AddCommandRegexp(CallbackDataStruct & callback,const std::string & str) ;
	//テンポラリルールに反応する音声認識ルールを構築します
	virtual xreturn::r<bool> AddTemporaryRegexp(CallbackDataStruct & callback,const std::string & str) ;
	//テンポラリルールをすべてクリアします
	virtual xreturn::r<bool> ClearTemporary() ;
	//構築したルールを音声認識エンジンにコミットします。
	virtual xreturn::r<bool> CommitRule() ;

private:
	void JuliusPlus::JuliusStop();
	xreturn::r<bool> JuliusPlus::JuliusStart();

	int LocalCaptureRuleNodeCount;
	int GlobalRuleNodeCount;

	double TemporaryRuleConfidenceFilter;
	double YobikakeRuleConfidenceFilter;
	double BasicRuleConfidenceFilter;
	std::list<std::string> YobikakeListArray;
	std::vector<CallbackDataStruct> CallbackDictionary;

	rule* TemporaryRuleHandle;
	rule* YobikakeRuleHandle;
	rule* CommandRuleHandle;
	int TemporaryRuleCount ;
	bool IsNeedUpdateRule;

	Jconf *jconf;
	Recog *recog;
	boost::thread *Thread;
};
