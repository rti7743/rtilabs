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
#include "RWindowBase.h"
#include "RWin32Exception.h"

class RWindow  : public RWindowBase
{
public:
	RWindow();
	virtual ~RWindow();

	//作成
	void Create(HINSTANCE inINstance, const char* inClassName,
					HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw(RWin32Exception);
	virtual LRESULT WinProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam) = 0;
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		RWindow* This;
		if (msg == WM_CREATE)
		{
			This = (RWindow*)lparam;

			//class にコールバックするためにサブクラス化
			This->MountWindow(hwnd);
			::SetWindowLong( hwnd , GWL_USERDATA,  (long)This);
		}
		else
		{
			This = (RWindow*)GetWindowLong( hwnd , GWL_USERDATA);
		}
		if (This != NULL ) return This->WinProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
};

#endif // !defined(AFX_RWINDOW_H__8C77A027_74D2_4D0C_B171_4272C174A0A3__INCLUDED_)
