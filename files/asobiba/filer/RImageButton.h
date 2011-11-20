// RImageButton.h: RImageButton クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_)
#define AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"
#include "RImage.h"

class RImageButton : public RWindow  
{
public:
	RImageButton();
	virtual ~RImageButton();
public:
	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//イメージのセット
	bool CreateImage(const string inFilename , bool isRegion);
	//絵の大きさにウィンドウを合わせる.
	void FitPictureTo();
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		RImageButton* p = (RImageButton*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//サブクラス化.
	WNDPROC			OrignalWindowProc;

	RImage			LoadImage;
};

#endif // !defined(AFX_RIMAGEBUTTON_H__0EEC9DA0_0B01_4D27_897F_FAEE184C7865__INCLUDED_)
