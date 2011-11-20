// RToken.h: RToken クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTOKEN_H__DD931B35_E929_4D7A_82C4_AB5D1E765D96__INCLUDED_)
#define AFX_RTOKEN_H__DD931B35_E929_4D7A_82C4_AB5D1E765D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RToken  
{
public:
	RToken()
	{
		Token = NULL;
		LastBuffer = NULL;
		CurrentToken = NULL;
	}
	virtual ~RToken()
	{
		delete [] Token;
		Token = NULL;
	}
	
	void setToken(const string inToken,char inTokenChar)
	{
		delete [] Token;

		int size = inToken.size();
		Token = new char[ size+1 ];

		memcpy(Token , inToken.c_str() , size + 1);

		for(char* p = Token; p = strchr(p , inTokenChar) ; p++)
		{
			*p = '\0';
		}
		LastBuffer = Token + size ;
		CurrentToken = Token;
	}

	const char* nextToken()
	{
		if (CurrentToken >= LastBuffer)	return NULL;

		char * r = CurrentToken;
		for(char * e = CurrentToken ; *e ; e++)	
			;
		CurrentToken = e+1;
		return r;
	}

	static void test();
private:
	//トークンバッファ
	char*	Token;
	//最終バッファ
	char* LastBuffer;
	//カレントトークン
	char*	CurrentToken;

};

#endif // !defined(AFX_RTOKEN_H__DD931B35_E929_4D7A_82C4_AB5D1E765D96__INCLUDED_)
