// TokenParser.cpp: TokenParser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "TokenParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TokenParser::TokenParser()
{
	this->Clear();
}

TokenParser::~TokenParser()
{

}
//トークンのパース
bool TokenParser::Set(const string & inString)
{
	this->Clear();

	int length = inString.size();
	for(int i = 0 ; i < length ; )
	{
		if ( inString[i] == ' ' )
		{//スペースは無視
			i ++;
			continue;
		}
		else if ( this->IsOperator( inString[i]  ) )
		{//オペレーター
			this->TokenList.push_back( inString.substr(i , 1 ) );
			i++;
		}
		else if ( this->IsNumber(inString[i]) )
		{//数字
			int start = i;
			for( ; this->IsNumber(inString[i]) ; i ++)
			{
				//nop
			}
			//分岐の条件が数字であることだから、文字列が空になることはありえない.
			ASSERT(i > start);

			this->TokenList.push_back(inString.substr(start , i-start ));
		}
		else if ( ::isalpha(inString[i]) )
		{//英字
			int start = i;
			for( ; ::isalpha(inString[i]) ; i ++)
			{
				//nop
			}
			//分岐の条件が英字であることだから、文字列が空になることはありえない.
			ASSERT(i > start);

			this->TokenList.push_back(inString.substr(start , i-start ));
		}
		else 
		{//不明..
			this->ErrorPosstion = i;
			return false;
		}
	}
	//イテレータに初期値位置をセットする.
	this->ActivePoint = TokenList.begin();
	return true;
}

//オペレータかどうか.
bool TokenParser::IsOperator(char inChar) const
{
	return inChar == '+' || inChar == '-' || inChar == '*' || inChar == '/' || inChar == '(' || inChar == ')' || inChar == '^';
}

//数字かどうか
bool TokenParser::IsNumber(char inChar) const
{
	return (inChar >= '0' && inChar <= '9')   || inChar == '.';
}


//テスト
void TokenParser::test()
{
	{
		//特殊系
		TokenParser tp;
		ASSERT( tp.Set("sin(0)") );
	}
	{
		//正常系
		TokenParser tp;
		string r;
		//パース
		ASSERT( tp.Set("1 + 2 + 3.14") );
		//トークンの取り出し
		ASSERT( (r = tp.GetToken()) == "1");
		//2回呼び出しても結果は一緒
		ASSERT( (r = tp.GetToken()) == "1");
		//次のトークンへ
		ASSERT( tp.NextToken() );
		ASSERT( (r = tp.GetToken()) == "+");
		//次のトークンへ 方法2
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "2");
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "3.14");
		//これ以上ないので空になる.
		ASSERT( (r = tp.GetNextToken()) == "");
		//これ以上ないので空になる.
		ASSERT( (r = tp.GetToken()) == "");
		//移動できないので false
		ASSERT( ! tp.NextToken() );
	}
	{
		//正常系2
		TokenParser tp;
		string r;
		//パース
		ASSERT( tp.Set("1 + (2 - 4)/2") );
		ASSERT( (r = tp.GetNextToken()) == "1");
		ASSERT( (r = tp.GetNextToken()) == "+");
		ASSERT( (r = tp.GetNextToken()) == "(");
		ASSERT( (r = tp.GetNextToken()) == "2");
		ASSERT( (r = tp.GetNextToken()) == "-");
		ASSERT( (r = tp.GetNextToken()) == "4");
		ASSERT( (r = tp.GetNextToken()) == ")");
		ASSERT( (r = tp.GetNextToken()) == "/");
		ASSERT( (r = tp.GetNextToken()) == "2");
	}
	{
		//初期化が正しく行われていることの確認.
		TokenParser tp;
		string r;

		//これ以上ないので空になる.
		ASSERT( (r = tp.GetNextToken()) == "");
		//これ以上ないので空になる.
		ASSERT( (r = tp.GetToken()) == "");
		//移動できないので false
		ASSERT( ! tp.NextToken() );
	}
	{
		//パースエラー
		TokenParser tp;
		int rr;
		//パースエラー
		ASSERT( ! tp.Set("1 + ### 2 + 3.14") );
		//5文字目にエラー
		ASSERT( (rr = tp.getErrorPosstion()) == 5);
	}

	
}

