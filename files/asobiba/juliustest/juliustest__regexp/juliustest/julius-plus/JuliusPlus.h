#pragma once;
#include <fstream>
#include <iostream>
#include <sstream>
#include "rule.h"
#include "xreturn.h"
#include "windows_encoding.h"


class JuliusPlus
{

public:
	JuliusPlus();
	virtual ~JuliusPlus();

	rule Grammer;

	bool JuliusPlus::RuleCommit(std::ostream* dfa , std::ostream* dict  );
	//音声認識ルールを構築します。 正規表現にも対応しています。
	xreturn::r<bool> JuliusPlus::AddRegexp(unsigned int id,const std::string & str ,rule* stateHandle ) ;
	//音声認識ルールを登録する部分の詳細な実行です。正規表現のネストがあるので再起してます。
	xreturn::r<bool> JuliusPlus::AddRegexpImpl(unsigned int id,const std::wstring & str, rule*  stateHandle);

private:
	int LocalCaptureRuleNodeCount;
	int GlobalRuleNodeCount;
};
