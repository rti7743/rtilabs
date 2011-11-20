// RWindow.h: RWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_)
#define AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RWindow  ;

#include "comm.h"
#include "RDoNotInitaliseException.h"

class RWindow  
{
public:
	RWindow();
	virtual ~RWindow();

	void MountWindow(HWND inWindow)
	{
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

	//ClientRect の取得.
	void GetClientRect(RECT * outRC) const;
	//WindowRect の取得.
	void GetWindowRect(RECT * outRC) const;


protected:
	//作成
	void Create(HINSTANCE inINstance, const char* inClassName,
					HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw();

private:
	HWND			WindowHandle;
	int				ID;
};

#endif // !defined(AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_)
