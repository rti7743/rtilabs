// RIPFilter.cpp: RIPFilter クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RIPFilter.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RIPFilter::RIPFilter()
{

}

RIPFilter::~RIPFilter()
{
	Clear();
}

void RIPFilter::Clear()
{
	for(IPLIST::iterator i = IPList.begin() ; i != IPList.end() ; ++i)
	{
		delete (*i);
	}
}

void RIPFilter::Load(const string inFilename)
{
	Clear();

	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}

	char buf[MAX_PATH];

	int line = 0;
	while( !feof(fp) )
	{
		if ( fgets(buf , MAX_PATH , fp) == NULL ) break;

		line++;
		if ( buf[0] == '\0' || buf[0] == '#' ) continue;

		try
		{
			AppendIP( RConv::chop(buf) );
		}
		catch(RParseException e)
		{
			printf("IPフィルターリストを読み込み中にエラーが発生しました(%d行目):%s\r\n" , line , e.getMessage() );
		}	
	}

	fclose(fp);
}

void RIPFilter::AppendIP(const string inLine) throw(RParseException)
{
	const char* line = inLine.c_str();

	//x.x.x.x-y.y.y.y 形式?
	const char * m = strchr(line , '-');
	if ( m != NULL )
	{	//x.x.x.x-y.y.y.y 形式.
		string from = string( line , 0 , m - line );
		string to = string( line + (m - line + 1) );

		in_addr fromIP ;
		in_addr toIP ;
		fromIP.s_addr = inet_addr( from.c_str() );
		toIP.s_addr = inet_addr( to.c_str() );

		if (fromIP.s_addr == INADDR_NONE)
		{
			throw RParseException(EXCEPTIONTRACE + "(" + from + ")有効なIPではありません.");
		}
		if (toIP.s_addr == INADDR_NONE)
		{
			throw RParseException(EXCEPTIONTRACE + "(" + to + ")有効なIPではありません.");
		}
		IPList.push_back( new RIPFilterData(fromIP , toIP) );
		return ;
	}
	//x.x.x.x/y 形式?
	m = strchr(line , '/');
	if ( m != NULL )
	{	//x.x.x.x/y 形式.
		string from = string( line , 0 , m - line );
		string mask = string( line + (m - line + 1)  );

		in_addr fromIP ;
		fromIP.s_addr = inet_addr( from.c_str() );
		int IPmask = atoi( mask.c_str() ); 

		if (fromIP.s_addr == INADDR_NONE)
		{
			throw RParseException(EXCEPTIONTRACE + "(" + from + ")有効なIPではありません.");
		}
		if (IPmask < 0 || IPmask > 32)
		{
			throw RParseException(EXCEPTIONTRACE + "(" + mask + ")マスクする範囲が正しくありません(0～32までにしてください).");
		}
		IPList.push_back( new RIPFilterData(fromIP , IPmask) );
		return ;
	}
	throw RParseException(EXCEPTIONTRACE + "文法エラーです。だだしく記述してください" + inLine );
}

void RIPFilterData::test()
{
	in_addr a,b,c;
	{
		a.s_addr = inet_addr("192.168.1.10");
		b.s_addr = inet_addr("192.168.1.20");

		RIPFilterData p( a , b );
		c.s_addr = htonl(inet_addr("192.168.1.10"));
		ASSERT( p.IsBan( c ) == true );

		c.s_addr = htonl(inet_addr("192.168.1.20"));
		ASSERT( p.IsBan( c ) == true );

		c.s_addr = htonl(inet_addr("192.168.1.15"));
		ASSERT( p.IsBan( c ) == true );
	}
	{
		a.s_addr = inet_addr("192.168.1.10");
		b.s_addr = inet_addr("192.168.1.20");

		RIPFilterData p( a , b );
		c.s_addr = htonl(inet_addr("10.1.168.192"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("192.1.20.10"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("1.1.1.1"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("0.0.0.0"));
		ASSERT( p.IsBan( c ) == false );
	}
	{
		a.s_addr = inet_addr("192.168.1.10");
		
		RIPFilterData p( a , 24 );
		c.s_addr = htonl(inet_addr("192.168.1.10"));
		ASSERT( p.IsBan( c ) == true );

		c.s_addr = htonl(inet_addr("192.168.1.20"));
		ASSERT( p.IsBan( c ) == true );

		c.s_addr = htonl(inet_addr("192.168.1.15"));
		ASSERT( p.IsBan( c ) == true );
	}
	{
		a.s_addr = inet_addr("192.168.1.10");

		RIPFilterData p( a , 24 );
		c.s_addr = htonl(inet_addr("10.1.168.192"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("192.1.20.10"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("1.1.1.1"));
		ASSERT( p.IsBan( c ) == false );

		c.s_addr = htonl(inet_addr("0.0.0.0"));
		ASSERT( p.IsBan( c ) == false );
	}
}

void RIPFilter::test()
{
	RIPFilter p;
	p.Load("testdata/ripfilter.txt");

	in_addr a;
	a.s_addr = htonl(inet_addr("192.168.1.10"));
	ASSERT( p.IsBan(a) == false );

}
