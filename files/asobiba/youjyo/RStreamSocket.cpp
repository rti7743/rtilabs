// RStreamSocket.cpp: RStreamSocket クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RStreamSocket.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStreamSocket::RStreamSocket(RSocketForWinSock2 * inSocket)
{
	Socket = inSocket;
}

RStreamSocket::~RStreamSocket()
{
}

int RStreamSocket::Write(const char *inBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->Use.Overlapped.hEvent );
	this->Use.Buffer.buf = (char*)inBuffer;
	this->Use.Buffer.len = inBufferSize;
	this->Use.Size = 0;
	this->Use.Flag = 0;

	Socket->Send(&this->Use);
	//どーよ、読めた?
	Socket->WaitForSignal( &this->Use,10000);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->Use);
	//こんだけ読めたよん.
	return this->Use.Size;

}
int RStreamSocket::Read(char *outBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->Use.Overlapped.hEvent );
	this->Use.Buffer.buf = (char*)outBuffer;
	this->Use.Buffer.len = inBufferSize;
	this->Use.Size = 0;
	this->Use.Flag = 0;

	Socket->Reserve(&this->Use);
	//どーよ、読めた?
	Socket->WaitForSignal( &this->Use,10000);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->Use);
	//こんだけ読めたよん.
	return this->Use.Size;
}


