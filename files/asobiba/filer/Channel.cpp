// hannel.cpp: Channel クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Channel.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


Channel::Channel()
{
	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i] = new WorkerThread(this);
		Thread[i]->Start();
	}
}

Channel::~Channel()
{
	//停止命令発行
	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->HoldUp();
	}
	//停止まで待って削除.
	for(i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->Join();
		delete Thread[i];
	}
}

//タスクをセットする
void Channel::putRequest(Request* inRequest)
{
	RAutoLock al(&Lock);
	RequestManage.push_back( inRequest );

	for(int i=0 ; i < MAX_WORKER_THREAD ; i++)
	{
		Thread[i]->Notify();
	}
}

//タスクを取得する
Request* Channel::takeRequest()
{
	RAutoLock al(&Lock);

	if (RequestManage.size() <= 0) return NULL;

	Request* ret = RequestManage.back();
	RequestManage.pop_back();

	return ret;
}
