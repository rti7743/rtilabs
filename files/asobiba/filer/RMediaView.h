// RMediaView.h: RMediaView クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_)
#define AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"
#include "RImageButton.h"
#include "RProgressBar.h"
#include "RMedia.h"
#include "RTimer.h"
#include "RSetupLoad.h"

class RMediaView : public RWindow  
{
public:
	RMediaView();
	virtual ~RMediaView();
public:
	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//ロード
	bool Load(const string inFilename, HINSTANCE inINstance);
	//タイトルを取得する
	string getTitle() const ;

private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		RMediaView* p = (RMediaView*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//ボタン.
	RImageButton	Start;
	RImageButton	Stop;
	RImageButton	FastForward;
	RImageButton	FastRewind;
	RImageButton	Forward;
	RImageButton	Rewind;

	RProgressBar	Seen;
	RProgressBar	Volum;
	//再生するやつ.
	RMedia			Media;

	RTimer			Timer;

	//設定のロード
	RSetupLoad		SetupLoad;
};

#endif // !defined(AFX_RMEDIAVIEW_H__702B6E56_775E_4830_B5B7_F9FD78EF6CF9__INCLUDED_)
