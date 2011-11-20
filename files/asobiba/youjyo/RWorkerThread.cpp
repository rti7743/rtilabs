// RWorkerThread.cpp: RWorkerThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWorkerThread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RWorkerThread::RWorkerThread(RThreadPool<RWorkerThread>* inPool)
{
	OwnerPool = inPool;
	Request = NULL;
}

RWorkerThread::~RWorkerThread()
{

}

/**
 * Run:		実行
 *
 * @return virtual unsigned int			常に 0
 */
unsigned int RWorkerThread::Run()
{
	while( this->IsHoldUp() )
	{
		if (Request)
		{	//実行
			Request->Execute(this);
			//実行し終わったので破棄.
			delete Request;
			//一応終わったけど、新しい仕事ある?
			Request = OwnerPool->complateRequest(this);
		}
		else
		{	//実行するものないので待ち
			this->Wait();
		}
	}
	return 0;
}

