// Parser.cpp: Parser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "RecursiveDescentParsing.h"
#include "XLException.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RecursiveDescentParsing::RecursiveDescentParsing()
{

}

RecursiveDescentParsing::~RecursiveDescentParsing()
{

}

//実行
double RecursiveDescentParsing::Run(TokenParser * poolToken)
{
	this->PoolToken = poolToken;
	double ans = this->Expression();
	if ( this->PoolToken->GetToken() != "" )
	{
		throw XLException(format("不明な値(%s)が見つかりました" , this->PoolToken->GetToken().c_str()) );
	}
	return ans;
}

//式
double RecursiveDescentParsing::Expression()
{
	double ans = this->Term();
	while(1)
	{
		if ( this->PoolToken->GetToken() == "+" )
		{//足し算
			this->PoolToken->NextToken();
			ans = ans + this->Term();
		}
		else if ( this->PoolToken->GetToken() == "-" )
		{//引き算
			this->PoolToken->NextToken();
			ans = ans - this->Term();
		}
		else
		{
			break;
		}
	}
	return ans;
}

//項
double RecursiveDescentParsing::Term()
{
	double ans = this->Factor();
	while(1)
	{
		if(this->PoolToken->GetToken() == "*")
		{//掛け算
			this->PoolToken->NextToken();
			ans = ans * this->Factor();
		}
		else if(this->PoolToken->GetToken() == "/")
		{//割り算
			this->PoolToken->NextToken();
			ans = ans / this->CheckZeroDiv( this->Factor() );
		}
		else if(this->PoolToken->GetToken() == "^")
		{//べき数
			this->PoolToken->NextToken();
			ans = pow(ans,this->Factor());
		}
		else
		{
			break;
		}
	}
	return ans;

}

//因子
double RecursiveDescentParsing::Factor()
{
    double  ans;

    if(this->PoolToken->GetToken() == "(")
	{//括弧
        this->PoolToken->NextToken();
        ans = this->Expression();
        if(this->PoolToken->GetToken() != ")")
		{
            throw XLException("閉じ括弧がありません");
        }
        this->PoolToken->NextToken();
    }
    else if(this->PoolToken->GetToken() == "+" )
	{//+符号付
		this->PoolToken->NextToken();
		ans = this->Factor();
    }
    else if(this->PoolToken->GetToken() == "-" )
	{//-符号付
		this->PoolToken->NextToken();
		ans = - this->Factor();
    }
	else
	{//数字または関数
        ans = this->Data();
	}

	return ans;
}

//データ
double RecursiveDescentParsing::Data()
{
	if ( IsNumber(this->PoolToken->GetToken()) )
	{//数字
		double ans = atof( this->PoolToken->GetToken().c_str() );
		this->PoolToken->NextToken();
		return ans;
	}
	//以下関数
	else if (this->PoolToken->GetToken() == "sin")
	{
		this->PoolToken->NextToken();
		return sin(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "cos")
	{
		this->PoolToken->NextToken();
		return cos(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "tan")
	{
		this->PoolToken->NextToken();
		return tan(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "sqrt")
	{
		this->PoolToken->NextToken();
		return sqrt(this->Factor());
	}
	else if (this->PoolToken->GetToken() == "pi")
	{
		this->PoolToken->NextToken();
		return 3.14;
	}
	else if (this->PoolToken->GetToken() == "piyutori")
	{
		this->PoolToken->NextToken();
		return 3;	//およそ3
	}

	throw XLException( format("不明な関数(%s)が呼び出されました", this->PoolToken->GetToken().c_str() ) );
}

//Zeroだったら例外で落とす.
double RecursiveDescentParsing::CheckZeroDiv(double inValue) const
{
	if ( inValue == 0 )
	{
		throw XLException("ゼロで除算しようとしました");
	}
	return inValue;
}

//数字のみで構成されているか？
bool RecursiveDescentParsing::IsNumber(const string & inString) const
{
	int len = inString.size();
	for(int i = 0 ; i < len ; i ++)
	{
		if (! (::isdigit( inString[i] )  ||  inString[i] == '.') )
		{
			return false;
		}
	}
	return true;
}

//テスト
void RecursiveDescentParsing::test()
{
	RecursiveDescentParsing rdp;
	TokenParser tp;
	double rr;
	ASSERT( tp.Set("1 + 2 + 3") );
	ASSERT( (rr = rdp.Run(&tp) ) == 6 );

	ASSERT( tp.Set("1 + (2 - 3)") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("sin(0)") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("-1 + 1") );
	ASSERT( (rr = rdp.Run(&tp) ) == 0 );

	ASSERT( tp.Set("-1 + 2 * 5") );
	ASSERT( (rr = rdp.Run(&tp) ) == 9 );

	ASSERT( tp.Set("-(2+3)") );
	ASSERT( (rr = rdp.Run(&tp) ) == -5 );

	ASSERT( tp.Set("(-1)+2*3+(4+(5-2))") );
	ASSERT( (rr = rdp.Run(&tp) ) == 12 );

	ASSERT( tp.Set("1.5 + 1.5") );
	ASSERT( (rr = rdp.Run(&tp) ) == 3 );
}
