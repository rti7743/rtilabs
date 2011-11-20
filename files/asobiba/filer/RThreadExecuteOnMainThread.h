// RThreadExecuteOnMainThread.h: RThreadExecuteOnMainThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_)
#define AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RThreadExecuteOnMainThread ;
#include "comm.h"
#include "REvent.h"

//イベント割り込み. 10/02 は　シスプリ repure 放送開始記念
#define RTI_WM_INTERRUPT	(WM_USER + 1002)

typedef void (RThreadExecuteOnMainThread::*MAIN_THREAD_CALLBACK)(void);

//メインスレッドに処理を委託します。
class RThreadExecuteOnMainThread  
{
protected:
	RThreadExecuteOnMainThread();
	virtual ~RThreadExecuteOnMainThread();

	//継承して処理を書くのですよー
	virtual unsigned int Run() = 0;

public:
	//メインに処理を委託します。
	void	Start(HWND inMainWindow);

	//停止を命じる
	//処理はすぐに帰ってきます。
	//停止フラグを立てます。
	void	HoldUp();

	//スレッドが停止するまで待つ。
	//なお、自分自身がこれを呼ぶとデッドロックします(w
	void	Join();

	//スレッドが停止するまでか、時間が経過するまで待つ。
	//なお、自分自身がこれを呼ぶとデッドロックします(w
	//時間が経過して抜けた場合は false .
	bool	Join(DWORD inWaitSecondTime);

	//Wait状態からの脱出.
	//スレッド以外が呼び出します。
	void	Notify();

	//このスレッドが有効かどうか調べる.
	bool	IsAlive();

	//Wait 状態かどうか調べる.
	bool	IsWait();

	//HoldUp 状態かどうか調べる.
	bool	IsHoldUp();
protected:
	//Notifyが呼ばれるまで待つ.
	//スレッド自身が呼び出します。
	void	Wait();

public:
	//コールバック.
	//自前で呼ぶな
	void	_Start();

private:
	//すべてのイベントをクリアします。
	void	AllResetEvent();

	//有効かどうか.
	REvent		Alive;

	//Waitに使うイベントオブジェクト.
	REvent	WaitEvent;

	//HoldUpに使うイベントオブジェクト.
	REvent	HoldUpEvent;

};

#endif // !defined(AFX_RTHREADEXECUTEONMAINTHREAD_H__C0D65388_5A4B_421B_AEF8_7AB670421CDE__INCLUDED_)
