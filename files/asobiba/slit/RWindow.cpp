// RWindow.cpp: RWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RWindow::RWindow()
{
}

RWindow::~RWindow()
{
}

//作成
void RWindow::Create(HINSTANCE inINstance, const char* inClassName,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw(RWin32Exception)
{
	HWND handle;
#ifndef _WIN32_WCE
	handle = ::CreateWindowEx(inFlgEx, inClassName, "",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, (LPVOID )this);
#else
	MY_USES_CONVERSION;
	handle = ::CreateWindowEx(inFlgEx, _A2W(inClassName), L"",
		inFlg ,	0, 0, 0, 0,
		isParentWindow, (HMENU)inID, inINstance, (LPVOID )this);
#endif
	if (handle == NULL) throw RWin32Exception("初期化に失敗しました");

	MountWindow(handle);
}

