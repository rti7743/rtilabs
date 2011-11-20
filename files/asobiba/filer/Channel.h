// hannel.h: Channel クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_)
#define AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Channel  ;

#include "Request.h"
#include "RLock.h"
#include "WorkerThread.h"


//スレッドの最大数
const int MAX_WORKER_THREAD	=	5;

//ワーカースレッドパターンで仕事(Request)を管理するクラス.
class Channel  
{
public:
	Channel();
	virtual ~Channel();

	//タスクをセットする
	void putRequest(Request* inRequest);
	//タスクを取得する
	Request* takeRequest();
private:
	//enum hack

	typedef deque<Request*>		REQUEST;
	REQUEST	RequestManage;

	//スレッドたち.
	WorkerThread*			Thread[MAX_WORKER_THREAD];
	//ロック.
	RLock	Lock;
};

#endif // !defined(AFX_CHANNEL_H__4BEE9CB9_5B87_49E6_BAF6_DCFE2647AEE9__INCLUDED_)
