// RSocketAsyncTransmitFile.h: RSocketAsyncTransmitFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_)
#define AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSocketForWinSock2.h"
#include "RNotFoundException.h"

//非同期ファイル転送.
class RSocketAsyncTransmitFile  
{
public:
	RSocketAsyncTransmitFile();
	virtual ~RSocketAsyncTransmitFile();

	void Mount(RSocketForWinSock2*		inSocket)
	{
		this->Socket = inSocket;
	}
//ファイル転送.
void RSocketAsyncTransmitFile::Transmit(const string inFilename  , int inStart,int inEnd , TRANSMIT_FILE_BUFFERS* inAppendBuffer);
//ファイルを伴わない転送.
void RSocketAsyncTransmitFile::TransmitWithout(TRANSMIT_FILE_BUFFERS* inAppendBuffer);

	int RSocketAsyncTransmitFile::WaitForComplate() throw(RIOException);
private:
	HANDLE					FileHandle;
	RSocketForWinSock2*		Socket;
	AsyncParam				TransmitUse;
	bool					IsPending;
};

#endif // !defined(AFX_RSOCKETASYNCTRANSMITFILE_H__58DA5B4E_02BC_4826_937C_B8EE89A963DA__INCLUDED_)
