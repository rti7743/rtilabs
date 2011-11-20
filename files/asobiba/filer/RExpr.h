// RExpr.h: RExpr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_)
#define AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"


//数式をトークンで分割して返す.
class RExprToken
{
public:
	void setExpression(string inExpression);
	string getExpression();
	string getToken() throw(RParseException);

	static void test();
private:
	string		Expression;
	int			Pos;
};

//再帰的下向き構文解析による数式解析
//参考 http://www3.justnet.ne.jp/~h-masuda/ProText/CAlgo/eval01.html

//なお、
//小数点、負の数　には対応してませんので注意してください.
class RExpr  
{
public:
	int Expr(string inExpression) throw(RParseException);
	static void test();
private:
	//式
	int expression() throw(RParseException);
	//項
	int term() throw(RParseException);
	//因子
	int factor() throw(RParseException);

	//次のトークンをセット.
	void getToken()
	{
		NextToken = Token.getToken();
	}
	string NextToken;
	RExprToken	Token;
};


#endif // !defined(AFX_REXPR_H__D84A51B1_2495_4EF0_9866_353C0BFBFFE0__INCLUDED_)
