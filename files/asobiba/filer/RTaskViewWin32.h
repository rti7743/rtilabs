// RTaskViewWin32.h: RTaskViewWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_)
#define AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RTaskView.h"
#include "RListView.h"
#include "RLock.h"


class RTaskViewWin32  : public RTaskView
{
public:
	RTaskViewWin32() throw();
	virtual ~RTaskViewWin32();


	//新しいタスクを追加します。
	void InsertTask(unsigned long inID,string inMessage);
	//進捗の更新
	void UpdateGoing(unsigned long inID,int inParsent);
	//タスクの削除
	void DeleteTask(unsigned long inID);
	//サイズが変更されたときに呼ばれる.
	virtual void OnSize() ;
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		RTaskViewWin32* This = (RTaskViewWin32*)GetWindowLong( hwnd , GWL_USERDATA);
		if (This != NULL ) return This->DlgProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ.
	LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//ウィンドウハンドル
	HWND	WindowHandle;
	//タスクを表示するリストビュー
	RListView	TaskList;
	//ロック.
	RLock	Lock;
};

#endif // !defined(AFX_RTASKVIEWWIN32_H__2BAC6EB2_8EB4_42E1_9029_EA94AB36ABEC__INCLUDED_)
