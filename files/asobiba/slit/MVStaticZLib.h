// MVStaticZLib.h: MVStaticZLib クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVSTATICZLIB_H__72D8E843_A09B_425F_A26C_98340AC8A52B__INCLUDED_)
#define AFX_MVSTATICZLIB_H__72D8E843_A09B_425F_A26C_98340AC8A52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zlib.h"
#include "RAutoLibraryWithException.h"

class MVStaticZLib 
{
private:
	MVStaticZLib()
	{
		Create();
	}
	void Create();
public:
	virtual ~MVStaticZLib()
	{
	}
	void MVStaticZLib::UnCompressed(unsigned char *outDest , int inDestSize , const unsigned char* inSrc , int inSrcSize) throw(RConferException);

	static MVStaticZLib* getInstance();
private:
	//エラーコードを例外にして飛ばす.
	void MVStaticZLib::ZLibThrow(int inRet , const string & inMethodName) const throw(RConferException);

	RAutoLibraryWithException Zlib;

	typedef int (WINAPI * inflateEndDef)(z_streamp strm);
	typedef int (WINAPI * inflateDef)(z_streamp strm, int flush);
	typedef int (WINAPI * inflateInit2_Def)(z_streamp strm, int windowBits , const char *version, int stream_size);

	inflateEndDef		inflateEnd;
	inflateDef			inflate;
	inflateInit2_Def	inflateInit2_;

};

#endif // !defined(AFX_MVSTATICZLIB_H__72D8E843_A09B_425F_A26C_98340AC8A52B__INCLUDED_)
