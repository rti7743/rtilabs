// FModCallable.h: FModCallable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMODCALLABLE_H__7B45A71C_0457_43A7_9766_D43940BCA2DD__INCLUDED_)
#define AFX_FMODCALLABLE_H__7B45A71C_0457_43A7_9766_D43940BCA2DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fmod_define.h"

class FModCallable  
{
public:
	FModCallable()
	{
	}
	virtual ~FModCallable()
	{
	}
protected:
	virtual signed char FModCallBack(FSOUND_STREAM *stream, void *buff, int len) = 0;
public:
	//再生中にコールバック.
	static  signed char __cdecl ProgressCallback(FSOUND_STREAM *stream, void *buff, int len, int param)
	{
		return ((FModCallable*)param)->FModCallBack(stream,buff, len);
	}

};

#endif // !defined(AFX_FMODCALLABLE_H__7B45A71C_0457_43A7_9766_D43940BCA2DD__INCLUDED_)
