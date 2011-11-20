// RWindowBase.h: RWindowBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_)
#define AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RWindowBase  
{
public:
	RWindowBase();
	virtual ~RWindowBase();

	void MountWindow(HWND inWindow)
	{
		ASSERT(inWindow != NULL);
		WindowHandle = inWindow;
	}

	//ウィンドウハンドルの取得.
	HWND		getWindowHandle() const 
	{
		return WindowHandle;
	}
	//IDの取得.
	int			getID()
	{
		return ID;
	}

	//位置だけ移動.
	void MoveWindow(int x,int y) const;
	//幅と高さだけ移動.
	void ResizeWindow(int width,int height) const;
	//移動.
	void MoveWindow(int x,int y,int width,int height) const;
	//最大表示
	void FullWindow(HWND inParentWidowHandle) const;
	//表示する
	virtual void Show() const;
	//非表示
	virtual void Hide() const;
	//スタイルのセット.
	void setStyle(DWORD inStyle);
	//スタイルの取得.
	DWORD getStyle();

	void EnableWindow(bool inEnable);

	//ClientRect の取得.
	void GetClientRect(RECT * outRC) const;
	//WindowRect の取得.
	void GetWindowRect(RECT * outRC) const;

	//指定したコントロールが 自分のウィンドウ座標上のどこにあるか?
	void getControlClientRect(HWND inTarget , RECT * outRC) const;

	//SendMessage
	LRESULT SendMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const;
	//PostMessage
	LRESULT PostMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const;


	string getText() const;
	void setText(const string & inStr);

private:
	HWND			WindowHandle;
	int				ID;
};

#endif // !defined(AFX_RWINDOWBASE_H__6BD4E592_ACCB_4842_99AA_8CFB750F6B65__INCLUDED_)
