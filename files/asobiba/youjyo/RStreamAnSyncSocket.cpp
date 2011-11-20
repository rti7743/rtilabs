// RStreamAnSyncSocket.cpp: RStreamAnSyncSocket クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RStreamAnSyncSocket.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStreamAsyncSocket::RStreamAsyncSocket(RSocketForWinSock2* inSocket)
{
	Socket = inSocket;
}

RStreamAsyncSocket::~RStreamAsyncSocket()
{
}

void RStreamAsyncSocket::Write(const char *inBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->ReadUse.Overlapped.hEvent );
	this->ReadUse.Buffer.buf = (char*)inBuffer;
	this->ReadUse.Buffer.len = inBufferSize;
	this->ReadUse.Size = 0;
	this->ReadUse.Flag = 0;
	Socket->Reserve(&this->ReadUse);
}

void RStreamAsyncSocket::Read(char *outBuffer ,int inBufferSize) 
{
	WSAResetEvent( this->ReadUse.Overlapped.hEvent );
	this->ReadUse.Buffer.buf = outBuffer;
	this->ReadUse.Buffer.len = inBufferSize;
	this->ReadUse.Size = 0;
	this->ReadUse.Flag = 0;
	Socket->Reserve(&this->ReadUse);
}

int RStreamAsyncSocket::ReadWaitForSignal() throw(RIOException)
{
	//どーよ、読めた?
	Socket->WaitForSignal( &this->ReadUse,60);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->ReadUse);
	//こんだけ読めたよん.
	return this->ReadUse.Size;
}

int RStreamAsyncSocket::ReadWaitForComplate() throw(RIOException)
{
	//どーよ、読めた?
	Socket->WaitForSignal(&this->ReadUse,WSA_INFINITE);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->ReadUse);
	//こんだけ読めたよん.
	return this->ReadUse.Size;
}

int RStreamAsyncSocket::WriteWaitForSignal() throw(RIOException)
{
	//どーよ、かけた?
	Socket->WaitForSignal( &this->WriteUse,60);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->WriteUse);
	//こんだけかけたよん
	return this->WriteUse.Size;
}

int RStreamAsyncSocket::WriteWaitForComplate() throw(RIOException)
{
	//どーよ、かけた?
	Socket->WaitForSignal( &this->WriteUse,60);	//タイムアウトは例外.
	//そんぢゃ、結果プリーズ.
	Socket->WSAGetOverlappedResult(&this->WriteUse);
	//こんだけかけたよん
	return this->WriteUse.Size;
}

