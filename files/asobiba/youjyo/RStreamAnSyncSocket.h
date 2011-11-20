// RStreamAnSyncSocket.h: RStreamAnSyncSocket クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAMANSYNCSOCKET_H__286C0D2E_BEE3_48B0_A80F_84F4BD66963B__INCLUDED_)
#define AFX_RSTREAMANSYNCSOCKET_H__286C0D2E_BEE3_48B0_A80F_84F4BD66963B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSocketForWinSock2.h"
#include "RStreamAsync.h"

class RStreamAsyncSocket : public RStreamAsync
{
public:
	RStreamAsyncSocket(RSocketForWinSock2* inSocket);
	virtual ~RStreamAsyncSocket();

	virtual void Write(const char *inBuffer ,int inBufferSize) ;
	virtual void Read(char *outBuffer ,int inBufferSize) ;

	virtual int ReadWaitForSignal() throw(RIOException);
	virtual int ReadWaitForComplate() throw(RIOException);

	virtual int WriteWaitForSignal() throw(RIOException);
	virtual int WriteWaitForComplate() throw(RIOException);
private:
	RSocketForWinSock2*	Socket;
	AsyncParam		ReadUse;
	AsyncParam		WriteUse;
};

#endif // !defined(AFX_RSTREAMANSYNCSOCKET_H__286C0D2E_BEE3_48B0_A80F_84F4BD66963B__INCLUDED_)
