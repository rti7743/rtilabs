// RNotify.cpp: RNotify クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RNotify.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNotify::RNotify()
{
	NotifyHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,   NULL,    0,   0);
}
RNotify::~RNotify()
{
	CloseHandle(NotifyHandle);
}

//通知する.
void RNotify::Post(void * inConnectPack)
{
	PostQueuedCompletionStatus(NotifyHandle, 0,(DWORD)inConnectPack , NULL);
}

//終了を伝える.
void RNotify::Shutdown()
{
	PostQueuedCompletionStatus(NotifyHandle, 0, NULL, NULL);
}

//通知までまて.
bool RNotify::Wait(void ** outConnectPack)
{
	DWORD			dummy;
	OVERLAPPED *	dummy2 ;
	return Btob(GetQueuedCompletionStatus(NotifyHandle , &dummy , (DWORD*)outConnectPack , &dummy2 , INFINITE));
}
