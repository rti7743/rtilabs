// RNormalWindow.h: RNormalWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_)
#define AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"

class RNormalWindow : public RWindow  
{
public:
	RNormalWindow();
	virtual ~RNormalWindow();
protected:
	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw();
protected:
	//ウィンドウプロシージャ(none static)
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
	{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		RNormalWindow* p = (RNormalWindow*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

};

#endif // !defined(AFX_RNORMALWINDOW_H__03886D53_66DB_46A8_A775_C8CB1E9F8FC0__INCLUDED_)
