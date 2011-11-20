// RZlib.cpp: RZlib クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RZlib.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RZlib::RZlib()
{

}

RZlib::~RZlib()
{

}

void RZlib::Create() throw(RException)
{
#ifndef _WIN32_WCE
	Zlib.Create("zlib.dll");
#else
	Zlib.Create("zlibce.dll");
#endif
	compress2 = (compress2Def) Zlib.GetProcAddress("compress2");
	uncompress = (uncompressDef) Zlib.GetProcAddress("uncompress");
}


//エラーコードを例外にして飛ばす.
void RZlib::ZLibThrow(int inRet) const throw(RConferException)
{
	switch(inRet)
	{
	case Z_ERRNO:
		throw RConferException(inRet , "zlib::Z_ERRNO");
	case Z_STREAM_ERROR:
		throw RConferException(inRet , "zlib::不正な引数");
	case Z_DATA_ERROR:
		throw RConferException(inRet , "zlib::データが壊れている");
	case Z_MEM_ERROR:
		throw RConferException(inRet , "zlib::メモリが足りない");
	case Z_BUF_ERROR:
		throw RConferException(inRet , "zlib::出力バッファが足りない");
	case Z_VERSION_ERROR:
		throw RConferException(inRet , "zlib::Z_VERSION_ERROR");
	default:
		throw RConferException(inRet , "zlib::未知のエラー");
	}
}


void RZlib::test()
{
	RZlib z;
	try
	{
		string src = "this is test!!";
		char dest[MAX_PATH];
		unsigned long destNum =  MAX_PATH;

		z.Create();

		z.Compress2((unsigned char*) dest , &destNum , (const unsigned char*) src.c_str() , src.size() + 1, BEST_COMPRESSION );

		char dest2[MAX_PATH];
		unsigned long destNum2 =  MAX_PATH;
		z.UnCompress((unsigned char*)dest2 , &destNum2 , (const unsigned char*)dest , destNum );

		ASSERT(src == dest2);
	}
	catch(RException e)
	{
		TRACE( e.getMessage() );
		ASSERT(0);
	}
}

