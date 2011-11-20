// RListViewWithMouseGesture.h: RListViewWithMouseGesture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_)
#define AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RListView.h"
#include "RMouseGestureMouseCapture.h"	//マウスジェスチャー マウスの動作を観測、つまりマウスの動作をチェキするのデス!

class RListViewWithMouseGesture : public RListView  
{
public:
	RListViewWithMouseGesture();
	virtual ~RListViewWithMouseGesture();

	//作成.	オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		RListViewWithMouseGesture* p = (RListViewWithMouseGesture*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//サブクラス化.
	WNDPROC			OrignalWindowProc;
private:
	//マウスジェスチャー マウスの動作を観測、つまりマウスの動作をチェキするのデス!
	RMouseGestureMouseCapture	MouseGestureMouseCapture;
};

#endif // !defined(AFX_RLISTVIEWWITHMOUSEGESTURE_H__A106AD96_78CD_4F0A_AE1A_4B77C885F5BE__INCLUDED_)
