// RThread.h: RThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
#define AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RThread  ;

#include "comm.h"

#include "RDoNotInitaliseException.h"

//スレッドクラス
//java の スレッドの劣化コピーです(w
class RThread  
{
protected:
	RThread();
	virtual ~RThread();

	//継承してスレッドの処理を書くのですよー
	virtual unsigned int Run() = 0;

public:
	//スレッドをスタートします。
	//例外
	//RDoNotInitaliseException	スレッドが作成できない
	void	Start() throw();

	//スレッドを強制停止させます。
	//危険、素人にはお勧めできない。
	//まぁ、お前ら素人は HoldUp で Join してなさいってこと。
	void	Stop();

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

private:
	//スレッドコールバック.
	static	unsigned int __stdcall  _Start(void* inThis);
	//すべてのイベントをクリアします。
	void	AllResetEvent();
private:
	//スレッドのハンドル.
	HANDLE	ThreadHandle;
	//スレッドID.
	unsigned int	ThreadNo;

	//Waitに使うイベントオブジェクト.
	HANDLE	WaitEvent;

	//HoldUpに使うイベントオブジェクト.
	HANDLE	HoldUpEvent;
};

#endif // !defined(AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
