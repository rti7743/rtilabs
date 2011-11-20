// MVStaticZLib.cpp: MVStaticZLib クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVStaticZLib.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////



MVStaticZLib* MVStaticZLib::getInstance()
{
	static MVStaticZLib a;
	return &a;
}

void MVStaticZLib::Create()
{
#ifndef _WIN32_WCE
	this->Zlib.Create("zlib.dll");
#else
	this->Zlib.Create("zlibce.dll");
#endif
	this->inflateEnd = (inflateEndDef) Zlib.GetProcAddress("inflateEnd");
	this->inflate = (inflateDef) Zlib.GetProcAddress("inflate");
	this->inflateInit2_ = (inflateInit2_Def) Zlib.GetProcAddress("inflateInit2_");
}

void MVStaticZLib::UnCompressed(unsigned char *outDest , int inDestSize , const unsigned char* inSrc , int inSrcSize) throw(RConferException)
{
	z_stream d_stream = {0}; /* decompression stream */

	d_stream.next_in  = (unsigned char*) inSrc;
	d_stream.next_out = (unsigned char*) outDest;
	d_stream.avail_in = inSrcSize;
	d_stream.avail_out = inDestSize;

	//注意: inflateInit(&d_stream) だと、 inflate がエラーになります。
	//      なぜに? inflateInit2 二日悩んだ。
	int err = this->inflateInit2_(&d_stream , -MAX_WBITS , ZLIB_VERSION, sizeof(z_stream));
	if (err != Z_OK )
	{
		ZLibThrow(err, "inflateInit");
	}

	err = this->inflate(&d_stream, Z_NO_FLUSH);
	if (err != Z_STREAM_END)
	{
		ZLibThrow(err, "inflate");
	}

	err = this->inflateEnd(&d_stream);
	if (err != Z_OK )
	{
		ZLibThrow(err, "inflateEnd");
	}
}

//エラーコードを例外にして飛ばす.
void MVStaticZLib::ZLibThrow(int inRet , const string & inMethodName) const throw(RConferException)
{
	switch(inRet)
	{
	case Z_OK:
		break;
	case Z_ERRNO:
		throw RConferException(inRet , inMethodName +"zlib::Z_ERRNO");
	case Z_STREAM_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::不正な引数");
	case Z_DATA_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::データが壊れている");
	case Z_MEM_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::メモリが足りない");
	case Z_BUF_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::出力バッファが足りない");
	case Z_VERSION_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::Z_VERSION_ERROR");
	default:
		throw RConferException(inRet , inMethodName +"zlib::未知のエラー");
	}
}