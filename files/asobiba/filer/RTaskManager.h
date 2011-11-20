// RTaskManager.h: RTaskManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_)
#define AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4786) 
#pragma warning (disable: 4503) 

#include "comm.h"

#include "RTask.h"
#include "RLock.h"
#include "Rfiles.h"
#include "Window.h"
#include "RNotify.h"
#include "Channel.h"

typedef deque<RTask*>	TASK_MANAGER;

//RTaskを管理するクラス。
//signleton である。
//また、 RTask からは Observer として処理が終わったら呼び出される。
class RTaskManager  
{
private:
	//タスク管理
	TASK_MANAGER		Task;

	Channel				TaskChannel;
public:
	//singleton
	static RTaskManager* getInstance();

	//タスクをセット.
	void putTask(RTask* inTask);

	//タスクを停止.
	void stopTask(const RTask* inTask);
	///このWindowに関連付けられているタスクをすべて削除.
	void stopTasks(const Window* inWin);

	//RTaskが処理が終わったときに呼び出す 
	void CallEndOfTask(RTask * inTask,const Window* inTaskWindow);
public:
	RTaskManager();
	virtual ~RTaskManager();



};

#endif // !defined(AFX_RTASKMANAGER_H__A95B695F_DBE3_488D_838D_8329490DCDB5__INCLUDED_)
