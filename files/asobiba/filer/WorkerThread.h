// WorkerThread.h: WorkerThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKERTHREAD_H__268AE137_BCC7_430C_82FF_4C8CADEF3A5A__INCLUDED_)
#define AFX_WORKERTHREAD_H__268AE137_BCC7_430C_82FF_4C8CADEF3A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class WorkerThread;

#include "RThread.h"
#include "Channel.h"

class WorkerThread  : public RThread
{
public:
	WorkerThread(Channel* inChannel);
	virtual ~WorkerThread();

	virtual unsigned int Run();
private:
	Channel*	OwnerChannel;
};

#endif // !defined(AFX_WORKERTHREAD_H__268AE137_BCC7_430C_82FF_4C8CADEF3A5A__INCLUDED_)
