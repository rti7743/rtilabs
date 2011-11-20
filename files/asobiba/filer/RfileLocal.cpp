// RfileLocal1.cpp: RfileLocal クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "comm.h"
#include "RfileLocal.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RfileLocal::RfileLocal()
{
	Handle = NULL;
}

RfileLocal::~RfileLocal()
{

}

//オープン
//@inPath	パス(Rfileはファイル名しか持っていない)
//@inMode	開くモード
//#開けなかった場合は RIOCannotOpenException を投げる
// サポートされていない動作は RNotSupportException を投げる
void RfileLocal::Open(string inPath ,__RFILEOPENTYPE inMode) throw()
{
	string fullpath = inPath + getVars(__FILECOLUMN_TYPE_NAME);
	if (Handle != NULL) throw RIOCannotOpenException("すでにファイル %s は開かれています!", fullpath.c_str() );

	switch(inMode)
	{
	case __RFILEOPENTYPE_READ:
		Handle = CreateFile( fullpath.c_str() , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN  | FILE_FLAG_OVERLAPPED, NULL);
		break;
	case __RFILEOPENTYPE_WRITE:
		Handle = CreateFile( fullpath.c_str() , GENERIC_WRITE , FILE_SHARE_READ , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_OVERLAPPED, NULL);
		break;
	default:
		throw RNotSupportException("ファイル %s は指定した操作を理解できません", fullpath.c_str() );
	}
	if (Handle == NULL) throw RIOCannotOpenException("ファイル %s のオープンに失敗しました OS ErrorCode %ld ", fullpath.c_str() , GetLastError() );


	OverLapped.hEvent = CreateEvent(NULL,FALSE,FALSE, fullpath.c_str() );
	OverLapped.Internal = 0;
	OverLapped.InternalHigh = 0;
	OverLapped.Offset = 0;
	OverLapped.OffsetHigh = 0;

	return;
}

//クローズ
//#閉じれなかった場合は RIOCannotCloseException を投げる
// サポートされていない動作は RNotSupportException を投げる
void RfileLocal::Close() throw()
{
	if (Handle == NULL) return;

	CloseHandle( Handle );
	CloseHandle(OverLapped.hEvent);

	Handle = NULL;
}

//リード
//@outBuf		バッファ
//@inBufferSize	バッファ長
//#読み込み失敗は RIOCannotReadException を返す
// サポートされていない動作は RNotSupportException を投げる
bool RfileLocal::Read(char* outBuf,int inBufferSize) throw()
{
	ASSERT(inBufferSize > 0);
//	printf("read %d\n" , inBufferSize);

	while(1)
	{
		if ( ReadFile( Handle , outBuf, inBufferSize , NULL, &OverLapped  ) )
			break;
		switch(GetLastError())
		{
		case ERROR_HANDLE_EOF:	//最後まで読んだー
			return false;
		case ERROR_IO_PENDING:	//現在処理中だよもん
			return true;
		case ERROR_WORKING_SET_QUOTA:
			puts("read ERROR_WORKING_SET_QUOTA");
			::Sleep(0);
			break;				//リトライ
		default:
			throw RIOCannotReadException("読み込み失敗 (%ld)" , GetLastError() );
		}
	}
	return true;
}

//ライト
//@outBuf		バッファ
//@inBufferSize	バッファ長
//?	>= 0 書き込んだバイト数
//  <  0 Eof
//#書き込み失敗は RIOCannotReadException を返す
// サポートされていない動作は RNotSupportException を投げる
bool RfileLocal::Write(char* outBuf,int inBufferSize) throw()
{
	ASSERT(inBufferSize > 0);

//	printf("write %d\n" , inBufferSize);

	while(1)
	{
		if ( WriteFile( Handle , outBuf, inBufferSize , NULL, &OverLapped)  )
			break;
		switch(GetLastError())
		{
		case ERROR_IO_PENDING:	//現在処理中だよもん
			return true;
		case ERROR_WORKING_SET_QUOTA:
			puts("write ERROR_WORKING_SET_QUOTA");
			::Sleep(0);
			break;				//リトライ
		default:
			throw RIOCannotReadException("書き込み失敗 (%ld)" , GetLastError() );
		}
	}
	return true;
}


//読み込み終了まで待つ.
//#書き込み失敗は RIOException を返す
void RfileLocal::WaitFor() throw()
{
	DWORD ret = ::WaitForSingleObject( OverLapped.hEvent , INFINITE);
	if (ret  != WAIT_OBJECT_0)
		throw RIOException("WaitForSingleObject が %ld を返しました",ret);
}

//終わったかどうか調べる
//? true	終わったWaitFor() よんでちょ
//? false	まだ
//#書き込み失敗は RIOException を返す
bool RfileLocal::Poll() throw()
{
	DWORD ret = ::WaitForSingleObject( OverLapped.hEvent , 0);
	if (ret  == WAIT_TIMEOUT)		return false;
	else if (ret == WAIT_OBJECT_0)	return true;

	throw RIOException("WaitForSingleObject が %ld を返しました",ret);
}

//処理の結果をもらいます。
//Poll が true / または、WaitFor のリターン後に使ってください.
//#書き込み失敗は RIOException を返す
unsigned long RfileLocal::GetReport() throw()
{
	DWORD tranceByte;
	BOOL ret = GetOverlappedResult( Handle , &OverLapped , &tranceByte , FALSE );
	if (ret == FALSE)
		throw RIOException("GetOverlappedResult が失敗しました(%ld)", GetLastError() );

//	printf("ファイル %s comp %ld  %ld\n" , Name.c_str() , tranceByte , OverLapped.InternalHigh );

	OverLapped.Offset += tranceByte;
	OverLapped.OffsetHigh = 0;

	ResetEvent( OverLapped.hEvent );

	return tranceByte;
}
