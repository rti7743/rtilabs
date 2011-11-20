// RWindow.cpp: RWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RWindow::RWindow()
{
	WindowHandle = NULL;
}

RWindow::~RWindow()
{
	if (this->WindowHandle)
		::DestroyWindow( this->WindowHandle );
}

//作成
void RWindow::Create(HINSTANCE inINstance, const char* inClassName,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw()
{
	this->ID = inID;

	this->WindowHandle = ::CreateWindowEx(inFlgEx, inClassName, "",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, NULL);
	if (this->WindowHandle == NULL) throw RDoNotInitaliseException("初期化に失敗しました");
}
/*
//作成
void RWindow::CreateActiveX(HINSTANCE inINstance, 
					   HWND isParentWindow,int inID,DWORD inFlg,DWORD inFlgEx) throw()
{
	this->ID = inID;

	this->WindowHandle = ::CreateWindowEx(inFlgEx, inClassName, "aaa",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU) inINstance, NULL);
	if (this->WindowHandle == NULL) throw RDoNotInitaliseException("初期化に失敗しました");
}
*/

//移動.
void RWindow::MoveWindow(int x,int y,int width,int height) const
{
	::MoveWindow( this->WindowHandle , x , y , width , height ,TRUE);
}

//位置だけ移動.
void RWindow::MoveWindow(int x,int y) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , x , y, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
}

//幅と高さだけ移動.
void RWindow::ResizeWindow(int width,int height) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , 0 , 0, width , height , SWP_NOMOVE | SWP_NOZORDER);
}


//最大表示
void RWindow::FullWindow(HWND inParentWidowHandle) const
{
	RECT rc;
	::GetClientRect(inParentWidowHandle , &rc);

	this->MoveWindow(0,0,rc.right,rc.bottom);
}


//表示する
void RWindow::Show() const
{
    ::ShowWindow(this->WindowHandle,SW_SHOW);
    ::UpdateWindow(this->WindowHandle);
}

//非表示
void RWindow::Hide() const
{
    ::ShowWindow(this->WindowHandle,SW_HIDE);
}


//スタイルのセット.
void RWindow::setStyle(DWORD inStyle)
{
	SetWindowLong(this->getWindowHandle(), GWL_STYLE, (LONG)inStyle);
}

//スタイルの取得.
DWORD RWindow::getStyle()
{
	return GetWindowLong(this->getWindowHandle(), GWL_STYLE);
}


//ClientRect の取得.
void RWindow::GetClientRect(RECT * outRC) const
{
	::GetClientRect( this->getWindowHandle() , outRC);
}

//WindowRect の取得.
void RWindow::GetWindowRect(RECT * outRC) const
{
	::GetWindowRect( this->getWindowHandle() , outRC);
}
