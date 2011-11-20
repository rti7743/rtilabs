// TokenParser.h: TokenParser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_)
#define AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//文字列を最小単位に分割する.
class TokenParser  
{
public:
	TokenParser();
	virtual ~TokenParser();

	//初期化
	void Clear()
	{
		this->ErrorPosstion = 0;
		this->TokenList.clear();
		this->ActivePoint = TokenList.begin();
	}
	//トークンのパース
	bool Set(const string & inString);

	//今のトークンを取得する.
	string GetToken() const
	{
		if (this->ActivePoint == TokenList.end())
		{
			return "";
		}
		return *this->ActivePoint;
	}
	//今のトークンを取得し、トークンを一つ進める
	string GetNextToken() 
	{
		if (this->ActivePoint == TokenList.end())
		{
			return "";
		}
		return *this->ActivePoint++;
	}
	//次のトークンに移動する.
	bool NextToken() 
	{
		if (this->ActivePoint == TokenList.end())
		{
			return false;
		}
		this->ActivePoint++;
		return true;
	}

	//エラーの場所を取得する.
	int getErrorPosstion() const
	{
		return this->ErrorPosstion + 1;	//1行目から開始したい.
	}
	//テスト
	static void test();

private:
	//オペレータかどうか.
	bool IsOperator(char inChar) const;
	//数字かどうか
	bool IsNumber(char inChar) const;

private:
	//トークンを格納する入れ物
	XLStringList TokenList;
	//現在読み込んでいる場所.
	XLStringList::const_iterator ActivePoint;
	//エラーの場所
	int ErrorPosstion;
};

#endif // !defined(AFX_TOKENPARSER_H__6F683998_E219_443C_BCA1_8BDFA99F94E6__INCLUDED_)
