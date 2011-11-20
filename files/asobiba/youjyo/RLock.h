// RLock.h: RLock クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_)
#define AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RLock;

#include "comm.h"



//このクラスはロックだ!
//そして、これを読んでいるキミ達もロックだ!
//by キユ(w
class RLock  
{
private:
	CRITICAL_SECTION CriticalSection;
public:
	RLock()
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~RLock()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	//クリティカルセクションに入る
	void Lock()
	{
		EnterCriticalSection(&CriticalSection);
	}

	//クリティカルセクションから抜ける
	void UnLock()
	{
		 LeaveCriticalSection(&CriticalSection);
	}
};

//自動ロック.
//このクラスは自動ロックだ!
//そして、これを読んでいるキミ達もロックだ!
//by キユ(w もう、ええって。。
class RAutoLock
{
public:
	RAutoLock(RLock * l)
	{
		Lock = l;
		Lock->Lock();
	}
	~RAutoLock()
	{
		Lock->UnLock();
	}
private:
	RLock*	Lock;
};


#endif // !defined(AFX_RLOCK_H__B3FCF45D_5BED_402E_A143_F7338FAE3A8E__INCLUDED_)
