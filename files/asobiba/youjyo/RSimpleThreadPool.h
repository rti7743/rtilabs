// RSimpleThreadPool.h: RSimpleThreadPool クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_)
#define AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YMain.h"

#ifdef _DEBUG
	//プールしておくスレッドの数.
	const int MAX_CLIENT_THREAD	= 10;
#else
	//プールしておくスレッドの数.(えんたーぷらいず)
	const int MAX_CLIENT_THREAD	= 100;
#endif


class RSimpleThreadPool  
{
public:
	RSimpleThreadPool()
	{
		for(int i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i] = new YMain();
			Threads[i]->Start();
		}
	}

	virtual ~RSimpleThreadPool()
	{
		//停止命令発行
		for(int i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i]->HoldUp();
		}
		//停止まで待って削除.
		for(i=0 ; i < MAX_CLIENT_THREAD ; i++)
		{
			Threads[i]->Join();
			delete Threads[i];
		}
	}
private:
	YMain*	Threads[MAX_CLIENT_THREAD];
};

#endif // !defined(AFX_RSIMPLETHREADPOOL_H__B229AAB5_E8B0_4092_B17C_7F1AFB1AC911__INCLUDED_)
