// RTask.h: RTask クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_)
#define AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTask  ;

#include "comm.h"
#include "window.h"
#include "WorkerThread.h"

class RTask  : public Request
{
public:
	RTask(Rfiles* inNewFiles, Display* inDisp , Window* inCallback);
	virtual ~RTask();
public:
	//進捗報告 開始0 ----> 完全100 
	virtual int Parsent() = 0;
	//タスクビューに表示するメッセージ
	virtual string getMessage() =0;

	//現在ターゲットになっているウィンドウを取得します。
	const Window*	getWindow();

	//停止を促します。
	void Stop();

	//タスク終了後、メインスレッドから呼び出される
	//無効なタスクであるため破棄せよ
	void Invalid();
	//タスク終了後、メインスレッドから呼び出される
	//有効であるので更新せよ
	void WindowUpdate();
protected:
	//コールバック実行.
	void	CallBack();
	//エラーコールバック実行.
	void	ErrorCallBack();

protected:
	//処理を依頼した window
	Window*			Win;
	//処理した Rfiles*
	Rfiles*			Files;
	//処理した Display*
	Display*		Disp;

};

#endif // !defined(AFX_RTASK_H__20607F2C_2AB9_47CD_8BE0_5E4C1AED7FCE__INCLUDED_)
