// RZlib.h: RZlib クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RZLIB_H__77B0C168_CA53_424F_9EF5_9099A0791471__INCLUDED_)
#define AFX_RZLIB_H__77B0C168_CA53_424F_9EF5_9099A0791471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
//#include "zlib.h"
#include "RAutoLibraryWithException.h"
#include "RConferException.h"

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)

class RZlib  
{
public:
	//圧縮レベル.
	enum 
	{
		NO_COMPRESSION		=	0,
		BEST_SPEED			=	1,
		BEST_COMPRESSION	=	9,
		DEAULT_COMPRESSION	=	-1,
	};


	RZlib();
	virtual ~RZlib();

	void Create() throw(RException);

	void Compress2(unsigned char *dest, unsigned long *destLen,  const unsigned char *source, unsigned long sourceLen,   int level = DEAULT_COMPRESSION) const throw (RConferException)
	{
		ASSERT(compress2 != NULL);
		int ret = compress2(dest , destLen , source , sourceLen  , level);
		if (ret == Z_OK) return ;

		ZLibThrow(ret);	//中で throw する.
	}
	void UnCompress(unsigned char *dest, unsigned long *destLen,  const unsigned char *source, unsigned long sourceLen) const throw (RConferException)
	{
		ASSERT(uncompress != NULL);
		int ret = uncompress(dest , destLen , source , sourceLen  );
		if (ret == Z_OK) return ;

		ZLibThrow(ret);	//中で throw する.
	}

	static void test();

private:
	//エラーコードを例外にして飛ばす.
	void ZLibThrow(int inRet) const throw(RConferException);


	typedef int (WINAPI *compress2Def)(unsigned char *dest, unsigned long *destLen,  const unsigned char *source, unsigned long sourceLen,   int level);
	typedef int (WINAPI *uncompressDef)(unsigned char *dest, unsigned long *destLen,  const unsigned char *source, unsigned long sourceLen);

	compress2Def	compress2;
	uncompressDef	uncompress;

	RAutoLibraryWithException	Zlib;
};

#endif // !defined(AFX_RZLIB_H__77B0C168_CA53_424F_9EF5_9099A0791471__INCLUDED_)
