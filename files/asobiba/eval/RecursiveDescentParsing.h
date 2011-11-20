// Parser.h: Parser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_)
#define AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <list>
//トークンをパースする.
#include "TokenParser.h"

// 再帰下降構文解析
//参考: http://www005.upp.so-net.ne.jp/h-masuda/cl/CAlgo/eval01.html
//      http://d.hatena.ne.jp/h_sakurai/20060119
class RecursiveDescentParsing  
{
public:
	RecursiveDescentParsing();
	virtual ~RecursiveDescentParsing();

	//実行
	double Run(TokenParser * poolToken);

	//テスト
	static void test();

private:
	//式
	double Expression();

	//項
	double Term();

	//因子
	double Factor();

	//データ
	double Data();

	//Zeroだったら例外で落とす.
	double CheckZeroDiv(double inValue) const;

	//数字のみで構成されているか？
	bool IsNumber(const string & inString) const;

private:
	//式をトークンで分割したもの.
	TokenParser* PoolToken;
};

#endif // !defined(AFX_PARSER_H__7B2FEA54_36CA_442B_84C9_031BFB37C256__INCLUDED_)
