// RNormalWindow.cpp: RNormalWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNormalWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNormalWindow::RNormalWindow()
{

}

RNormalWindow::~RNormalWindow() 
{
	//サブクラス解除
	SetWindowLong(this->getWindowHandle() , GWL_USERDATA , NULL);
}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RNormalWindow::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inFlgEx) throw()
{

    static WNDCLASS	wc = {0};
	if (! wc.lpszClassName )
	{
		wc.lpszClassName = "NormalWindow";
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)WndProcStatic;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = inINstance;
		wc.hIcon         = NULL;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName  = NULL;
		if(!RegisterClass(&wc))
		{
			throw RDoNotInitaliseException("ウィンドウクラスの作成に失敗しました");
		}

	}

	RWindow::Create(inINstance,wc.lpszClassName,isParentWindow,inID,inFlg  ,0);

	//よーしパパ、サブクラスにしちゃうぞー
	SetWindowLong(this->getWindowHandle() , GWL_USERDATA , (long)this);
}
