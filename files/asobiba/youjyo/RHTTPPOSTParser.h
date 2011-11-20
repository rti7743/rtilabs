// RHTTPPOSTParser.h: RHTTPPOSTParser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPPOSTPARSER_H__43BEB778_948F_46D6_84CC_14871E9C116F__INCLUDED_)
#define AFX_RHTTPPOSTPARSER_H__43BEB778_948F_46D6_84CC_14871E9C116F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

typedef map<string , string>	POST;

class RHTTPPOSTParser  
{
public:
	RHTTPPOSTParser();
	virtual ~RHTTPPOSTParser();

	void Parse(const char* inBuffer , int inSize );
	void Dump();

	POST	PostData;

	static void test();
};

#endif // !defined(AFX_RHTTPPOSTPARSER_H__43BEB778_948F_46D6_84CC_14871E9C116F__INCLUDED_)
