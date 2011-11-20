// RThread.cpp: RThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RThread.h"
#include "RExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RThread::RThread()
{
	//スレッドのハンドル.
	this->ThreadHandle = NULL;
	//スレッドID.
	this->ThreadNo = 0;

	//終了フラグ
	this->HoldUpEvent = CreateEvent(NULL,TRUE,FALSE, NULL);
}

RThread::~RThread()
{
	this->Stop();

	CloseHandle(this->HoldUpEvent);
}

//スレッドをスタートします。
void	RThread::Start() throw(RException)
{
	if ( this->IsAlive() )
	{
		throw RException(EXCEPTIONTRACE + "すでにスレッドは作成されています");
	}

	AllResetEvent();

#ifndef _WIN32_WCE
	this->ThreadHandle = (HANDLE)_beginthreadex(NULL , 0 , _Start , (void*) this ,  0 , &this->ThreadNo);
#else
	this->ThreadHandle = CreateThread(NULL , 0 , _Start , (void*) this ,0 , &this->ThreadNo);
#endif
	if ( this->ThreadHandle == NULL)
	{
		DWORD error = ::GetLastError();
		throw RException(EXCEPTIONTRACE + "すでにスレッドに失敗しました" + RExceptionUtil::WindowsCodeToString(error) );
	}
}

//スレッドを強制停止させます。
//危険、素人にはお勧めできない。
//まぁ、お前ら素人は HoldUp で Join してなさいってこと。
void	RThread::Stop()
{
	if (this->IsAlive() )
	{
#ifndef _WIN32_WCE
		_endthreadex( (unsigned int) this->ThreadHandle );
#else
		TerminateThread(this->ThreadHandle , 0);
#endif
	}
	CloseHandle(this->ThreadHandle);
}


//停止を命じる
//処理はすぐに帰ってきます。
//停止フラグを立てます。
void	RThread::HoldUp()
{
	SetEvent( this->HoldUpEvent  );
}


//スレッドが停止するまで待つ。
//なお、自分自身がこれを呼ぶとデッドロックします(w
void	RThread::Join()
{
	::WaitForSingleObject( this->ThreadHandle  , INFINITE);
}


//スレッドが停止するまでか、時間が経過するまで待つ。
//なお、自分自身がこれを呼ぶとデッドロックします(w
//時間が経過して抜けた場合は false .
bool	RThread::Join(DWORD inWaitSecondTime)
{
	return	::WaitForSingleObject
				( this->ThreadHandle  , inWaitSecondTime) != WAIT_TIMEOUT;
}


//すべてのイベントをクリアします。
void	RThread::AllResetEvent()
{
	ResetEvent( this->HoldUpEvent  );
}


//このスレッドが有効かどうか調べる.
bool	RThread::IsAlive() const
{
	return	::WaitForSingleObject( this->ThreadHandle  , 0) == WAIT_TIMEOUT;	
}

//HoldUp 状態かどうか調べる.
bool	RThread::IsHoldUp() const
{
	return	::WaitForSingleObject( this->HoldUpEvent  , 0) == WAIT_TIMEOUT;	
}

#ifndef _WIN32_WCE
	//スレッドコールバック.
	unsigned int RThread::_Start(void* inThis)
	{
		return ((RThread*)inThis)->Run();
	}
#else
	//スレッドコールバック.
	unsigned long RThread::_Start(void* inThis)
	{
		return ((RThread*)inThis)->Run();
	}
#endif

//スリープ 指定時間たつか、終了が発生するまで sleep する
void RThread::Sleep(DWORD inWaitSecondTime)
{
	::WaitForSingleObject( this->HoldUpEvent  , inWaitSecondTime) ;	
}
