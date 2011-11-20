// RTextLib.cpp: RTextLib クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTextLib.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTextLib::RTextLib()
{

}

RTextLib::~RTextLib()
{

}


const string RTextLib::Skip(const string & inTarget , const string & inFind)
{
	const char * p = strstr ( inTarget.c_str() , inFind.c_str() );
	if (p == NULL) return "";

	p += inFind.size();
	return p;
}

const string RTextLib::SkipBefor(const string & inTarget , const string & inFind)
{
	const char * p = strstr ( inTarget.c_str() , inFind.c_str() );
	if (p == NULL) return "";

	return p;
}


const string RTextLib::Cut(const string & inTarget , const string & inStart , const string & inEnd)
{
	const char * p = strstr ( inTarget.c_str() , inStart.c_str() );
	if (p == NULL) return "";

	p += inStart.size();

	const char * e = strstr( p ,inEnd.c_str() );
	if (e == NULL) return "";

	return string(p , 0 , e - p );
}