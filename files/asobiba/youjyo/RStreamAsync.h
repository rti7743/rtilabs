// RStreamAsync.h: RStreamAsync クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAMASYNC_H__E0F41DE0_4F8A_41B0_9C75_F9577A96118E__INCLUDED_)
#define AFX_RSTREAMASYNC_H__E0F41DE0_4F8A_41B0_9C75_F9577A96118E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOException.h"

class RStreamAsync  
{
public:
	RStreamAsync();
	virtual ~RStreamAsync();

	virtual void Write(const char *inBuffer ,int inBufferSize) = 0;
	virtual void Read(char *outBuffer ,int inBufferSize) = 0;

	virtual int ReadWaitForSignal() throw(RIOException)= 0;
	virtual int ReadWaitForComplate() throw(RIOException)= 0;

	virtual int WriteWaitForSignal() throw(RIOException)= 0;
	virtual int WriteWaitForComplate() throw(RIOException)= 0;

};

#endif // !defined(AFX_RSTREAMASYNC_H__E0F41DE0_4F8A_41B0_9C75_F9577A96118E__INCLUDED_)
