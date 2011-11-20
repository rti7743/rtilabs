// RWorkerThread.h: RWorkerThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_)
#define AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RWorkerThread;

#include "RThread.h"
#include "RThreadPool.h"
#include "RRequest.h"

class RWorkerThread : public RThread  
{
public:
	RWorkerThread(RThreadPool<RWorkerThread>* inPool);
	virtual ~RWorkerThread();

	
	/**
	 * Run:		実行
	 *
	 * @return virtual unsigned int			常に 0
	 */
	virtual unsigned int Run();
	/**
	 * Bind:		仕事を登録する.
	 *
	 * @param inReq		新しい仕事
	 * @return void 
	 */
	void	Bind(RRequest* inReq)
	{
		Request = inReq;
	}

private:
	//スレッドプール本体.
	RThreadPool<RWorkerThread>*	OwnerPool;
	//リクエスト.
	RRequest*		Request;
};

#endif // !defined(AFX_RWORKERTHREAD_H__17331539_AA0E_497B_9E13_C55FB99EF56A__INCLUDED_)
