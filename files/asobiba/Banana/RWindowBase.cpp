// RWindowBase.cpp: RWindowBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RWindowBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RWindowBase::RWindowBase()
{
	WindowHandle = NULL;
}

RWindowBase::~RWindowBase()
{
	if (this->WindowHandle)
		::DestroyWindow( this->WindowHandle );
}


//移動.
void RWindowBase::MoveWindow(int x,int y,int width,int height) const
{
	::MoveWindow( this->WindowHandle , x , y , width , height ,TRUE);
}

//位置だけ移動.
void RWindowBase::MoveWindow(int x,int y) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , x , y, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
}

//幅と高さだけ移動.
void RWindowBase::ResizeWindow(int width,int height) const
{
	SetWindowPos(	this->getWindowHandle() , NULL , 0 , 0, width , height , SWP_NOMOVE | SWP_NOZORDER);
}


//最大表示
void RWindowBase::FullWindow(HWND inParentWidowHandle) const
{
	RECT rc;
	::GetClientRect(inParentWidowHandle , &rc);

	this->MoveWindow(0,0,rc.right,rc.bottom);
}


//表示する
void RWindowBase::Show() const
{
    ::ShowWindow(this->WindowHandle,SW_SHOW);
    ::UpdateWindow(this->WindowHandle);
}

//非表示
void RWindowBase::Hide() const
{
    ::ShowWindow(this->WindowHandle,SW_HIDE);
}


//スタイルのセット.
void RWindowBase::setStyle(DWORD inStyle)
{
	SetWindowLong(this->getWindowHandle(), GWL_STYLE, (LONG)inStyle);
}

//スタイルの取得.
DWORD RWindowBase::getStyle()
{
	return GetWindowLong(this->getWindowHandle(), GWL_STYLE);
}


//ClientRect の取得.
void RWindowBase::GetClientRect(RECT * outRC) const
{
	::GetClientRect( this->getWindowHandle() , outRC);
}

//WindowRect の取得.
void RWindowBase::GetWindowRect(RECT * outRC) const
{
	::GetWindowRect( this->getWindowHandle() , outRC);
}

string RWindowBase::getText() const
{
#ifndef _WIN32_WCE
	char buf[MAX_PATH];
	GetWindowText(this->getWindowHandle() , buf , MAX_PATH);

	return string(buf);
#else
	MY_USES_CONVERSION;
	TCHAR buf[MAX_PATH];
	GetWindowText(this->getWindowHandle() , buf , MAX_PATH);

	return string(_W2A(buf));
#endif
}

void RWindowBase::setText(const string & inStr)
{
#ifndef _WIN32_WCE
	SetWindowText(this->getWindowHandle() , inStr.c_str() );
#else
	MY_USES_CONVERSION;
	SetWindowText(this->getWindowHandle() , _A2W(inStr.c_str()) );
#endif
}

//SendMessage
LRESULT RWindowBase::SendMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::SendMessage(this->getWindowHandle() , inMsg , wParam , lParam );
}
//PostMessage
LRESULT RWindowBase::PostMessage(UINT inMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::PostMessage(this->getWindowHandle() , inMsg , wParam , lParam );
}

void RWindowBase::EnableWindow(bool inEnable)
{
	::EnableWindow(this->getWindowHandle() , btoB( inEnable ) );
}


//指定したコントロールが 自分のウィンドウ座標上のどこにあるか?
void RWindowBase::getControlClientRect(HWND inTarget , RECT * outRC) const
{
	::GetWindowRect( inTarget , outRC );

	POINT pt;
	pt.x = outRC->left;
	pt.y = outRC->top;
	ScreenToClient( this->getWindowHandle() , &pt);
	outRC->left = pt.x;
	outRC->top = pt.y;

	pt.x = outRC->right;
	pt.y = outRC->bottom;
	ScreenToClient( this->getWindowHandle() , &pt);
	outRC->right = pt.x;
	outRC->bottom = pt.y;
}

