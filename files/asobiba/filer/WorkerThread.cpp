// WorkerThread.cpp: WorkerThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WorkerThread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WorkerThread::WorkerThread(Channel* inChannel) : RThread()	
{
	this->OwnerChannel = inChannel;
}

WorkerThread::~WorkerThread()
{

}

unsigned int WorkerThread::Run()
{
	while( this->IsHoldUp() )
	{
		//次のタスクプリーズ
		Request* req = OwnerChannel->takeRequest();
		if (req)
		{	//実行
			req->Execute();
		}
		else
		{	//実行するものないので待ち
			this->Wait();
		}
	}
	return 0;
}

