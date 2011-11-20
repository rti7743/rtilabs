// RSocketAsyncTransmitFile.cpp: RSocketAsyncTransmitFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RSocketAsyncTransmitFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RSocketAsyncTransmitFile::RSocketAsyncTransmitFile()
{
	this->FileHandle = NULL;
	this->Socket = NULL;
}

RSocketAsyncTransmitFile::~RSocketAsyncTransmitFile()
{
	if ( FileHandle )
	{
		::CloseHandle( FileHandle );
		FileHandle = NULL;
	}
}

//ファイル転送.
void RSocketAsyncTransmitFile::Transmit(const string inFilename  , int inStart,int inEnd , TRANSMIT_FILE_BUFFERS* inAppendBuffer)
{
	if (inStart > inEnd) throw RNotFoundException("Rangeの範囲がおかしいです.");

	this->FileHandle = CreateFile( inFilename.c_str() ,
								GENERIC_READ ,
								FILE_SHARE_READ,NULL,OPEN_EXISTING,
								FILE_FLAG_SEQUENTIAL_SCAN,NULL );
	if (this->FileHandle == INVALID_HANDLE_VALUE)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "ファイル" + inFilename + "が開けません");
	}
 
	this->TransmitUse.Buffer.buf = NULL;	// not use.
	this->TransmitUse.Buffer.len = 0;
	this->TransmitUse.Size = inEnd - inStart;
	this->TransmitUse.Flag = 0;

	this->TransmitUse.Overlapped.Offset = inStart;
	this->TransmitUse.Overlapped.OffsetHigh = 0;

	WSAResetEvent( this->TransmitUse.Overlapped.hEvent );
	this->IsPending = Socket->TransmitFile( this->FileHandle , inEnd - inStart , 0 , inAppendBuffer , &TransmitUse);
}

//ファイルを伴わない転送.
void RSocketAsyncTransmitFile::TransmitWithout(TRANSMIT_FILE_BUFFERS* inAppendBuffer)
{
	this->FileHandle = NULL;

	this->TransmitUse.Buffer.buf = NULL;	// not use.
	this->TransmitUse.Buffer.len = 0;
	this->TransmitUse.Size = 0;
	this->TransmitUse.Flag = 0;

	WSAResetEvent( this->TransmitUse.Overlapped.hEvent );
	this->IsPending = Socket->TransmitFile( NULL , 0 , 0 , inAppendBuffer , &TransmitUse);
}

//レジュームありの転送
/*
void RSocketAsyncTransmitFile::TransmitRange(const string inFilename  , int inStart,int inEnd , TRANSMIT_FILE_BUFFERS* inAppendBuffer)
{
	this->FileHandle = CreateFile( inFilename.c_str() ,
								GENERIC_READ ,
								FILE_SHARE_READ,NULL,OPEN_EXISTING,
								FILE_FLAG_SEQUENTIAL_SCAN,NULL );
	if (this->FileHandle == INVALID_HANDLE_VALUE)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "ファイル" + inFilename + "が開けません");
	}
 
	this->TransmitUse.Buffer.buf = NULL;	// not use.
	this->TransmitUse.Buffer.len = 0;
	this->TransmitUse.Size = GetFileSize(this->FileHandle , NULL);
	this->TransmitUse.Flag = 0;

	WSAResetEvent( this->TransmitUse.Overlapped.hEvent );
	this->IsPending = Socket->TransmitFile( this->FileHandle , GetFileSize(this->FileHandle , NULL) , inEnd , inAppendBuffer , &TransmitUse);
}
*/

int RSocketAsyncTransmitFile::WaitForComplate() throw(RIOException)
{
	if ( ! this->IsPending )
	{
		//どーよ、書けた?
		Socket->WaitForSignal(&this->TransmitUse,WSA_INFINITE);	//タイムアウトは例外.
		//そんぢゃ、結果プリーズ.
		Socket->WSAGetOverlappedResult(&this->TransmitUse);
	}

	::CloseHandle( FileHandle );
	FileHandle = NULL;

	//こんだけ読めたよん.
	return this->TransmitUse.Size;
}
