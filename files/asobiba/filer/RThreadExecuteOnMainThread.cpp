// RThreadExecuteOnMainThread.cpp: RThreadExecuteOnMainThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RThreadExecuteOnMainThread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RThreadExecuteOnMainThread::RThreadExecuteOnMainThread()
{
	Alive.ResetEvent();
	AllResetEvent();
}

RThreadExecuteOnMainThread::~RThreadExecuteOnMainThread()
{
	Join();
}

//メインに処理を委託します。
void	RThreadExecuteOnMainThread::Start(HWND inMainWindow)
{
	ASSERT(inMainWindow != NULL);

	if ( Alive.Pooling() )
	{
		return ;	//すでに起動しています。
	}
	Alive.SetEvent();
	AllResetEvent();

	::PostMessage(inMainWindow , RTI_WM_INTERRUPT , (WPARAM)this , (LPARAM)0);
}

//コールバック.
void  RThreadExecuteOnMainThread::_Start()
{
	ASSERT(	Alive.Pooling() );

	this->Run();

	Alive.ResetEvent();
}


//停止を命じる
//処理はすぐに帰ってきます。
//停止フラグを立てます。
void	RThreadExecuteOnMainThread::HoldUp()
{
	HoldUpEvent.SetEvent();
	Notify();
}

//スレッドが停止するまで待つ。
//なお、自分自身がこれを呼ぶとデッドロックします(w
void	RThreadExecuteOnMainThread::Join()
{
	Alive.WaitForSignal(INFINITE);
}

//スレッドが停止するまでか、時間が経過するまで待つ。
//なお、自分自身がこれを呼ぶとデッドロックします(w
//時間が経過して抜けた場合は false .
bool	RThreadExecuteOnMainThread::Join(DWORD inWaitSecondTime)
{
	return Alive.WaitForSignal(inWaitSecondTime);
}

//Notifyが呼ばれるまで待つ.
//スレッド自身が呼び出します。
void	RThreadExecuteOnMainThread::Wait()
{
	WaitEvent.ResetEvent();
	WaitEvent.WaitForSignal(INFINITE);
}

//Wait状態からの脱出.
//スレッド以外が呼び出します。
void	RThreadExecuteOnMainThread::Notify()
{
	WaitEvent.SetEvent();
}

//すべてのイベントをクリアします。
void	RThreadExecuteOnMainThread::AllResetEvent()
{
	WaitEvent.ResetEvent();
	HoldUpEvent.ResetEvent();
}


//このスレッドが有効かどうか調べる.
bool	RThreadExecuteOnMainThread::IsAlive()
{
	return Alive.Pooling();
}

//Wait 状態かどうか調べる.
bool	RThreadExecuteOnMainThread::IsWait()
{
	return WaitEvent.Pooling();
}

//HoldUp 状態かどうか調べる.
bool	RThreadExecuteOnMainThread::IsHoldUp()
{
	return HoldUpEvent.Pooling();
}

