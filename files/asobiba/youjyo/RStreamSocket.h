// RStreamSocket.h: RStreamSocket クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAMSOCKET_H__EC408D8D_17F4_4EEF_8710_5A9BE7C7B28B__INCLUDED_)
#define AFX_RSTREAMSOCKET_H__EC408D8D_17F4_4EEF_8710_5A9BE7C7B28B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RStream.h"
#include "RSocketForWinSock2.h"

class RStreamSocket  : public RStream
{
public:
	RStreamSocket(RSocketForWinSock2 * inSocket);
	virtual ~RStreamSocket();

	virtual int Write(const char *inBuffer ,int inBufferSize) ;
	virtual int Read(char *outBuffer ,int inBufferSize) ;
private:
	RSocketForWinSock2*	Socket;
	AsyncParam		Use;
};

#endif // !defined(AFX_RSTREAMSOCKET_H__EC408D8D_17F4_4EEF_8710_5A9BE7C7B28B__INCLUDED_)
