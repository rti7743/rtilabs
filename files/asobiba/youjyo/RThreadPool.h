// RThreadPool.h: RThreadPool クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_)
#define AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
template<class _USER_WORKER_THREAD>
class RThreadPool;




//#include "RWorkerThread.h"
//#define _USER_WORKER_THREAD	RWorkerThread
#include "YThreadWithTls.h"

#include "RRequest.h"
#include "RLock.h"

#ifdef _DEBUG
	//プールしておくスレッドの数.
	const int MAX_THREAD	= 10;
#else
	//プールしておくスレッドの数.(えんたーぷらいず)
	const int MAX_THREAD	= 50;
#endif

/**
 * class RThreadPool:	スレッドプール
 *
 * @author		rti
 */
template<class _USER_WORKER_THREAD>
class RThreadPool  
{
public:
	RThreadPool()
	{
			for(int i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i] = new _USER_WORKER_THREAD(this);
				Threads[i]->Start();

				//あいているスレッドということでぶち込みます。
				WaitThreads.push_back(Threads[i]);
			}
	}
	virtual ~RThreadPool()
	{
			//停止命令発行
			for(int i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i]->HoldUp();
			}
			//停止まで待って削除.
			for(i=0 ; i < MAX_THREAD ; i++)
			{
				Threads[i]->Join();
				delete Threads[i];
			}
	}

	/**
	 * putRequest:	リクエストを送る.
	 *
	 * @param inRequest		リクエスト
	 * @return void 
	 */
	void putRequest(RRequest* inRequest)
	{
			//このルーチンはクリティカルセクションで保護されています。
			RAutoLock al(&Lock);

			//あいているスレッドありますか?
			if (WaitThreads.size() == 0)
			{
				//ないぢゃん.
				//リクエストキューに追加
				RequestQeue.push_back(inRequest);
				return ;
			}
			//あいているスレッドきぼんぬ.
			_USER_WORKER_THREAD* thread = WaitThreads.front();

			//仕事を割り振る.
			thread->Bind(inRequest);
			//目覚めよ!
			thread->Notify();

			WaitThreads.pop_front();
			//仕事しているキューに入れる.
//			WorkThreads.push_back(thread);
	}
	/**
	 * ComplateRequest:	リクエスト完了のお知らせ
	 *
	 * @param inThread		仕事をしたスレッド(このスレッド自身がこのルーチンを呼びます)
	 * @return RRequest*	すぐに新しいリクエストがある場合は Request* で仕事がなければ NULL
	 */
	RRequest* complateRequest(_USER_WORKER_THREAD* inThread)
	{
			//このルーチンはクリティカルセクションで保護されています。
			RAutoLock al(&Lock);

			//実は超忙しい?
			if (RequestQeue.size() != 0)
			{
				//待機状態のリクエストがあるので引き続き処理します.
				RRequest*  req = RequestQeue.front();
				RequestQeue.pop_front();
				return req;
			}

			//仕事をしているスレッドより自分を削除.
//			WORKER_THREADS_POINTER::iterator i = find(WorkThreads.begin() , WorkThreads.end() , inThread);
//			ASSERT(i != WorkThreads.end());
//			WorkThreads.erase(i);

			//待機状態のところに入って...
			WaitThreads.push_back(inThread);

			//しばしの眠りにつきます。　ばいぶー.
			return NULL;
	}

	void Dump()
	{
		//このルーチンはクリティカルセクションで保護されています。
		RAutoLock al(&Lock);

		puts("========================== ThreadPool Dump ============================");
		printf("待機状態スレッド %d\r\n" , WaitThreads.size() );
		printf("仕事中のスレッド %d\r\n" , WorkThreads.size() );
		printf("未処理リクエスト %d\r\n" , RequestQeue.size() );
		puts("========================== =============== ============================");
	}

private:

	//クリティカルセクション
	RLock						Lock;

	//スレッドが振るの時にはここにリクエストが蓄えられます。
	typedef deque<RRequest*>	REQUESTQEUE;
	REQUESTQEUE					RequestQeue;

	//これだけのスレッドをプールしておきます。
	_USER_WORKER_THREAD*	Threads[MAX_THREAD];

	typedef deque<_USER_WORKER_THREAD*>	WORKER_THREADS_POINTER;
	//待機状態のスレッド
	WORKER_THREADS_POINTER	WaitThreads;
	//仕事をしているスレッド
	WORKER_THREADS_POINTER	WorkThreads;
};

#endif // !defined(AFX_RTHREADPOOL_H__F7FC1EEE_8DF5_4D12_A07C_83B0AADFE90C__INCLUDED_)
