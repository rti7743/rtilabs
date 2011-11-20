// WindowsMain.h: WindowsMain クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
#define AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



#include "Filer.h"
#include "DisplayWin32.h"
#include "RDoNotInitaliseException.h"
#include "RSpliter.h"
#include "BasePanel.h"

//デザイン
#define TOOL_BAR_START	0
#define TOOL_BAR_SIZE	28
#define ADDRESS_BAR_START	(TOOL_BAR_SIZE)
#define ADDRESS_BAR_SIZE	20
#define TAB_BAR_START	(ADDRESS_BAR_START + ADDRESS_BAR_SIZE)
#define TAB_BAR_SIZE	20
#define STATUS_BAR_SIZE	20
#define ITEM_PANEL_SIZE	(TOOL_BAR_SIZE + ADDRESS_BAR_SIZE + TAB_BAR_SIZE)

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
	//実行.
	int   Run() throw();

	//メインウィンドウハンドルの取得
	HWND getWindowHandle() const 
	{
		return WindowHandle;
	}
	//メインインスタンスハンドルの取得
	HINSTANCE getInstance() const 
	{
		return Instance;
	}
	
	//BaseWindowハンドルの取得.
	HWND getBaseWindow(bool isLeft) const
	{
		if (isLeft)	return Panel[0].getWindowHandle();
		else		return Panel[1].getWindowHandle();
	}

	//WndProcの処理の横流しに登録する.
	void registeCallbackWndProc(bool isLeft ,WndProcCallable * inProc)
	{
		if (isLeft)		Panel[0].registeCallbackWndProc(inProc);
		else			Panel[1].registeCallbackWndProc(inProc);
	}
	//WndProcの処理の横流しの解除
	void UnregisteCallbackWndProc(bool isLeft ,WndProcCallable * inProc)
	{
		if (isLeft)		Panel[0].UnregisteCallbackWndProc(inProc);
		else			Panel[1].UnregisteCallbackWndProc(inProc);
	}

	//適当にIDを振り出す(w
	static int getID() ;

private:
	//サイズ変更
	void OnSize();

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
	//メインウィンドウのハンドル.
    HWND		WindowHandle;
	//インスタンスハンドル.
	HINSTANCE	Instance;
	//ベースパネル 左右に一枚づつ作ります。
	//この上にアドレスバーやリストビューを配置します。
	BasePanel	Panel[2];

	//左右分割
	RSpliter	Spliter;
};

#endif // !defined(AFX_WINDOWSMAIN_H__24E24427_3464_411D_B86B_CC3C742F2664__INCLUDED_)
