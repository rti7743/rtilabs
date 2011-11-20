// RTask.cpp: RTask クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNotify.h"
#include "RTask.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTask::RTask(Rfiles* inNewFiles, Display* inDisp , Window* inCallback) : Request()
{

	//処理を依頼した window
	Win = inCallback;
	//処理した Rfiles*
	Files = inNewFiles;
	//処理した Display*
	Disp = inDisp;
}

RTask::~RTask()
{

}

//現在ターゲットになっているウィンドウを取得します。
const Window*	RTask::getWindow()
{
	return Win;
}

//停止を促します。
void RTask::Stop()
{
	// そのうち実装します(w.
}


//コールバック実行.
void	RTask::CallBack()
{
	//更新必要っす。
	RNotify<RTask,Window>	Nofity;
	Nofity.CallBack(this,Win);
}
//エラーコールバック実行.
void	RTask::ErrorCallBack()
{
	//更新必要っす。
	RNotify<RTask,Window>	Nofity;
	Nofity.CallBack(this,NULL);
}

//タスク終了後、メインスレッドから呼び出される
//無効なタスクであるため破棄せよ
void RTask::Invalid()
{
	if (Files) Files->Dec(); // delete Files;

//	delete Disp;

	Files = NULL;
	Disp = NULL;
}

//タスク終了後、メインスレッドから呼び出される
//有効であるので更新せよ
void RTask::WindowUpdate()
{
	if (Win)	Win->UpdateWindow(Files,Disp);
}

