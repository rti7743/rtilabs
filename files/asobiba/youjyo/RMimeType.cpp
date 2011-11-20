// RMimeType.cpp: RMimeType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RMimeType.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMimeType::RMimeType()
{

}

RMimeType::~RMimeType()
{

}

void RMimeType::Load(const string inMimeFilname)
{
	FILE * fp = fopen( inMimeFilname.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}
	char buf[MAX_PATH];

	while( !feof(fp) )
	{
		if ( fgets(buf , MAX_PATH , fp) == NULL ) break;

		if ( buf[0] == '\0' || buf[0] == '#' ) continue;

		const char* next = nextSpace( buf );
		if (next == NULL) continue;

		//これが mime タイプ
		const string type = RConv::chop( string(buf , 0 , next - buf) );

		const char* extStart = nextNonSpace( next );
		if (extStart == NULL) continue;

		while(1)
		{
			const char* extEnd = nextSpace( extStart );
			if (extEnd == NULL)
			{	//ひとつしかない場合
				const string ext = RConv::chop(extStart);
				MimeType[ ext ] = type;
				break;
			}

			//で、これがファイル拡張子です。
			const string ext = RConv::chop( string(extStart , 0 , extEnd - extStart) );
//			MimeType[ RAlphabetHash( ext.c_str() ) ] = type;
			MimeType[ ext ] = type;
			
			extStart = nextNonSpace( extEnd );
			if (extStart == NULL) break;
		}
	}
	fclose(fp);
}

//次の区切りまで読み飛ばし.
const char* RMimeType::nextSpace(const char* inStr) const
{
	while(*inStr != '\0')
	{
		if ( *inStr == ' ' || *inStr == '\t' ) return inStr;
		++inStr;
	}

	return NULL;
}


//次の区切りではない文字まで読み飛ばし.
const char* RMimeType::nextNonSpace(const char* inStr) const
{
	while(*inStr != '\0')
	{
		if ( *inStr != ' ' && *inStr != '\t' ) return inStr;
		++inStr;
	}

	return NULL;
}

void RMimeType::test()
{
	{
		RMimeType	rmt;
		rmt.Load("conftest/mime_small.types");

		string ret;

		ASSERT( (ret = rmt.getMimeType("jpeg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("jpg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("png") ) == "image/png" );
	}
	{
		RMimeType	rmt;
		rmt.Load("conftest/mime.types");

		string ret;

		ASSERT( (ret = rmt.getMimeType("jpeg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("jpg") ) == "image/jpeg" );
		ASSERT( (ret = rmt.getMimeType("png") ) == "image/png" );
	}
}

RMimeType* RMimeType::getInstance()
{
	static RMimeType p;
	return &p;
}

