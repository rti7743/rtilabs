// RTaskManager.cpp: RTaskManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTaskManager.h"
#include "RTaskLS.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTaskManager::RTaskManager()
{
}

RTaskManager::~RTaskManager()
{

}

RTaskManager* RTaskManager::getInstance()
{
	static RTaskManager rtm;
	return &rtm;
}


//RTaskが処理が終わったときに呼び出す 
void RTaskManager::CallEndOfTask(RTask * inTask,const Window* inTaskWindow)
{
	if (inTask == NULL) return ;	//不正.

	//タスクリストより検索.
	TASK_MANAGER::iterator it = find( Task.begin() , Task.end() , inTask);
	if (it != Task.end())
	{
		//有効なタスク なので Windowに反映したいと思う。

		//そのまえに有効なウィンドウであるか調査
		if (Filer::getFiler()->IsValidWindow(inTaskWindow) )
		{//有効なウィンドウなので更新処理.
			inTask->WindowUpdate();
		}
		else
		{//無効なウィンドウなので、無効なタスクとして処理.
			TRACE("warning: Task %p is valid , but window is invalid!!\n",inTask);
			inTask->Invalid();
		}
		Task.erase( it );
	}
	else
	{
		//無効なタスクです.
		TRACE("warning: Task %p is invalid!!\n",inTask);

		inTask->Invalid();
	}
	delete inTask;
}

//タスクをセット.
void RTaskManager::putTask(RTask* inTask)
{
	//タスク管理に登録.
	Task.push_back(inTask);
	//実行してねと登録 ワーカースレッドパターン.
	TaskChannel.putRequest(inTask);
}

//タスクを停止.
void RTaskManager::stopTask(const RTask* inTask)
{
	ASSERT(inTask != NULL);

	//タスクリストより検索.
	TASK_MANAGER::iterator it = find( Task.begin() , Task.end() , inTask);
	if (it != Task.end())
	{
		//停止を促す(タスクは急には止まらない)
		(*it)->Stop();
		//管理キューから除外します。
		Task.erase( it );
	}
	//このときタスク自体は生きているので
	//delete してはだめです。
}


//このWindowに関連付けられているタスクをすべて削除.
void RTaskManager::stopTasks(const Window* inWin)
{
	ASSERT(inWin != NULL);

	TASK_MANAGER::iterator it;
	for(it = Task.begin() ; it != Task.end() ;)
	{
		if (inWin == (*it)->getWindow() )
		{
			//停止を促す(タスクは急には止まらない)
			(*it)->Stop();
			//管理キューから除外します。
			it = Task.erase( it );
		}
		else
		{
			++it;
		}
	}

}
