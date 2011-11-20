// RHTTPPOSTParser.cpp: RHTTPPOSTParser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RHTTPPOSTParser.h"
#include "RConv.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RHTTPPOSTParser::RHTTPPOSTParser()
{

}

RHTTPPOSTParser::~RHTTPPOSTParser()
{

}

void RHTTPPOSTParser::Parse(const char* inBuffer , int inSize )
{
	int columStart = 0;
	for(int i = 0; i < inSize ; i ++)	//A
	{
		// = までサーチ.
		if ( inBuffer[i] == '=' )
		{
			int valueStart = i;
			for( i=i+1 ; i < inSize ; i ++)	//B
			{
				//区切りの & をサーチ.
				if ( inBuffer[i] == '&' )
				{
					PostData[ RConv::chop(string(inBuffer , columStart , valueStart - columStart )) ] = 
							RConv::UrlEncode( string(inBuffer , valueStart+1 , i -(valueStart+1) ) );
					++i;
					columStart = i;
					break;	//Bのループを抜ける.
				}
			}
			if (i >= inSize)
			{
				PostData[ RConv::chop(string(inBuffer , columStart , valueStart - columStart )) ] = 
					RConv::UrlEncode( string(inBuffer , valueStart+1 , i -(valueStart+1) ) );
			}
		}
	}
}

void RHTTPPOSTParser::Dump()
{
	puts("========================== POST Dump ============================");
	//すべて解放.
	for(POST::iterator i = PostData.begin() ; i != PostData.end() ; ++i)
	{
		printf("%s -> %s\n",(*i).first.c_str(),(*i).second.c_str());
	}
	puts("========================== ========= ============================");
}

 
void RHTTPPOSTParser::test()
{
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "submit=%8F%91%82%AB%8D%9E%82%DE&key=1043426551&FROM=&mail=&MESSAGE=%82%D2%82%B1%81K%82%D2%82%B1&bbs=rti&time=1043440535";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["bbs"]) == "rti");
	}
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "aaa=bbb";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["aaa"]) == "bbb");
	}
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "aaa=bbb&ccc=dddd";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["aaa"]) == "bbb");
		ASSERT( (a = rhpp.PostData["ccc"]) == "dddd");
	}
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "aaa=bbb&";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["aaa"]) == "bbb");
	}
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "aaa=bbb&bbb=%aaaa&&";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["aaa"]) == "bbb");
		ASSERT( (a = rhpp.PostData["bbb"]) == "%aaaa");
	}
	{
		RHTTPPOSTParser	rhpp;
		string a;
		const char* test = "bbs=rti&key=1041563575&submit=%8F%91%82%AB%8D%9E%82%DE&FROM=&mail=&MESSAGE=aaaa";
		rhpp.Parse( test , strlen(test) );

		ASSERT( (a = rhpp.PostData["bbs"]) == "rti");
		ASSERT( (a = rhpp.PostData["key"]) == "1041563575");
		ASSERT( (a = rhpp.PostData["FROM"]) == "");
	}
}


