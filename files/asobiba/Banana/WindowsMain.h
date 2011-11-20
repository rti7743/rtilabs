// WindowsMain.h: WindowsMain クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
#define AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "BBanana.h"
#include "RMessageBox.h"

class WindowsMain  
{
private:
	//singleton のため外部より作成できなくようにする
	WindowsMain();
public:
	virtual ~WindowsMain();
public:
	//メインシステムの取得 singleton
	static WindowsMain*	getMainSystem();
	//画面の更新.
	static void Update();
	//実行.
	int   Run()  throw(RException);

	//メインウィンドウハンドルの取得
	HWND getWindowHandle() const 
	{
		return BananaDlg.getWindowHandle();
	}
	//メインインスタンスハンドルの取得
	HINSTANCE getInstance() const 
	{
		return Instance;
	}
	
	//適当にIDを振り出す(w
	static int getID() ;
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		return getMainSystem()->WndProc(hwnd,msg,wparam,lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


private:
	BBanana		BananaDlg;
	//インスタンスハンドル.
	HINSTANCE	Instance;
};

#endif // !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
