// YRequestPath.cpp: YRequestPath クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RToken.h"
#include "YRequestPath.h"

#define TOP_PATH  "/"
#define TEST_PATH  "test"
#define READCGI_PATH "read.cgi"
#define DAT_PATH  "dat"
#define SUBJECT_PATH "subject.txt"
#define INDEX_PATH  "index.html"
#define SUBBACK_PATH  "subback.html"
#define BBSCGI_PATH  "bbs.cgi"

const int BOARD_NAME_MAX = 255;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YRequestPath::YRequestPath()
{
	RequestType	= REQUESTTYPE_ERROR;
}

YRequestPath::~YRequestPath()
{

}

bool YRequestPath::PathParse(const string &inPath)
{

	//TOP_PATH を含んでいる?
	if ( strncmp( inPath.c_str() , TOP_PATH , sizeof(TOP_PATH)-1 ) != 0)	return false;
	RToken Token;
	Token.setToken( inPath.c_str()  + sizeof(TOP_PATH)-1 , '/');

	const char* s;
	if ( ! (s = Token.nextToken() ) )	return false;

	if ( strcmp( s , TEST_PATH ) == 0)
	{//http://pc.2ch.net/test/read.cgi/php/1008344964/l50
		if ( ! (s = Token.nextToken() ) )	return false;
		if ( strcmp( s , READCGI_PATH ) != 0)
		{
			if ( strcmp( s , BBSCGI_PATH ) == 0)
			{
				RequestType = REQUESTTYPE_BBSCGI;
				return true;
			}
			return false;
		}

		//板名ゲット
		if ( ! (s = Token.nextToken() ) )	return false;
		this->BoardName =  s;

		//DAT番号ゲット.
		if ( ! (s = Token.nextToken() ) )	return false;
		if ( ! (this->DatNumber = atol(s) ) ) return false;

		//取得する書き込みナンバー
		if ( ! (s = Token.nextToken() ) )
		{
			RequestType = REQUESTTYPE_CGIALL;
			return true;
		}
		else
		{
			if (s[0] == 'l')
			{//http://pc.2ch.net/test/read.cgi/php/1008344964/l50
				wParam = atoi(s + 1);
				RequestType = REQUESTTYPE_CGILAST;
				return true;
			}
			
			const char * haifun = strchr(s , '-');
			if ( haifun == NULL)
			{//http://pc.2ch.net/test/read.cgi/php/1000797355/231
				wParam = atoi(s);
				RequestType = REQUESTTYPE_CGIONE;
				return true;
			}
			else
			{//http://pc.2ch.net/test/read.cgi/php/1000797355/201-300
				if ( *(haifun + 1) == '\0')
				{//http://pc.2ch.net/test/read.cgi/php/1000797355/201-
					wParam = atoi(s);
					RequestType = REQUESTTYPE_CGIFORM;
					return true;
				}
				if (s == haifun)
				{//http://pc.2ch.net/test/read.cgi/php/1000797355/-300
					wParam = atoi(haifun + 1);
					RequestType = REQUESTTYPE_CGITO;
					return true;
				}
				wParam = atoi(s);
				lParam = atoi(haifun + 1);
				RequestType = REQUESTTYPE_CGIFORMTO;
				return true;
			}
		}
	}
	else	
	{//http://pc.2ch.net/php/dat/1036889287.dat
		this->BoardName =  s;

		if ( ! (s = Token.nextToken() ) )
		{
			if (  inPath[ inPath.size() - 1] == '/' )
			{
				RequestType = REQUESTTYPE_INDEX;
				return true;
			}
			return false;
		}
		if ( strcmp( s , INDEX_PATH ) == 0  )
		{
			RequestType = REQUESTTYPE_INDEX;
			return true;
		}
		if ( strcmp( s , SUBBACK_PATH ) == 0  )
		{
			RequestType = REQUESTTYPE_SUBBACK;
			return true;
		}

		//DATらしい.
		if ( strcmp( s , DAT_PATH ) == 0)
		{
			if ( ! (s = Token.nextToken() ) )	return false;

			if ( ! (this->DatNumber = atol(s) ) ) return false;
			RequestType = REQUESTTYPE_DAT;
		}
		else if ( strcmp( s , SUBJECT_PATH ) == 0 )
		{//http://pc.2ch.net/php/subject.txt
			RequestType = REQUESTTYPE_SUBJECT;
		}
		else
		{
			return false;
		}
		return true;
	}
}

void YRequestPath::test()
{
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/php/subback.html") );
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_SUBBACK);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/php/") );
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_INDEX);
	}
	{
		YRequestPath	yrp;
		ASSERT( !yrp.PathParse("/ba.jpg") );
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355/-300") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGITO);
		ASSERT( yrp.getwParam() == 300);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1008344964/l50") );
		ASSERT( yrp.getDatNumber() == 1008344964);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGILAST);
		ASSERT( yrp.getwParam() == 50);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355/231") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGIONE);
		ASSERT( yrp.getwParam() == 231);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355/201-300") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGIFORMTO);
		ASSERT( yrp.getwParam() == 201);
		ASSERT( yrp.getlParam() == 300);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355/201-") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGIFORM);
		ASSERT( yrp.getwParam() == 201);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355/") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGIALL);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/read.cgi/php/1000797355") );
		ASSERT( yrp.getDatNumber() == 1000797355);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_CGIALL);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/php/dat/1036889287.dat") );
		ASSERT( yrp.getDatNumber() == 1036889287);
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_DAT);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/php/subject.txt") );
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_SUBJECT);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/php/index.html") );
		ASSERT( yrp.getBoardName() == "php");
		ASSERT( yrp.getRequestType() == REQUESTTYPE_INDEX);
	}
	{
		YRequestPath	yrp;
		ASSERT( yrp.PathParse("/test/bbs.cgi") );
		ASSERT( yrp.getRequestType() == REQUESTTYPE_BBSCGI);
	}
	puts("YRequestPath::test OK");
}

