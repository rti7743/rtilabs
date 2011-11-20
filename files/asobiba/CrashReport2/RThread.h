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
#include "RException.h"

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
	void	Start() throw(RException);

	//スレッドを強制停止させます。
	//危険、素人にはお勧めできない。
	//まぁ、お前ら素人は HoldUp で Join してなさいってこと。
	void	Stop();

	//停止を命じる
	//処理はすぐに帰ってきます。
	//停止フラグを立てます。
	virtual void	HoldUp();

	//スレッドが停止するまで待つ。
	//なお、自分自身がこれを呼ぶとデッドロックします(w
	void	Join();

	//スレッドが停止するまでか、時間が経過するまで待つ。
	//なお、自分自身がこれを呼ぶとデッドロックします(w
	//時間が経過して抜けた場合は false .
	bool	Join(DWORD inWaitSecondTime);

	//このスレッドが有効かどうか調べる.
	bool	IsAlive() const;

	//HoldUp 状態かどうか調べる.
	bool	IsHoldUp() const;

	//スリープ 指定時間たつか、終了が発生するまで sleep する
	void 	Sleep(DWORD inWaitSecondTime);

protected:
	//Notifyが呼ばれるまで待つ.
	//スレッド自身が呼び出します。
	void	Wait();

private:
	#ifndef _WIN32_WCE
		//スレッドコールバック.
		static	unsigned int __stdcall  _Start(void* inThis);
	#else
		//スレッドコールバック.
		static	unsigned long __stdcall  _Start(void* inThis);
	#endif

	//すべてのイベントをクリアします。
	void	AllResetEvent();
private:
	//スレッドのハンドル.
	HANDLE	ThreadHandle;

	#ifndef _WIN32_WCE
		//スレッドID.
		unsigned int	ThreadNo;
	#else
		//スレッドID.
		unsigned long	ThreadNo;
	#endif

	//HoldUpに使うイベントオブジェクト.
	HANDLE	HoldUpEvent;
};

#endif // !defined(AFX_RTHREAD_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
