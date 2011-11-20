// RImageButton.cpp: RImageButton クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImageButton.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RImageButton::RImageButton()
{

}

RImageButton::~RImageButton()
{

}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RImageButton::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,"BUTTON",isParentWindow,inID, inFlg | WS_CHILD ,0);

	//よーしパパ、サブクラスにしちゃうぞー
	this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
	SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WndProcStatic);
}

//イメージのセット
bool RImageButton::CreateImage(const string inFilename , bool isRegion)
{
	LoadImage.Release();
	return LoadImage.Load(inFilename) ;
}

//絵の大きさにウィンドウを合わせる.
void RImageButton::FitPictureTo()
{
	this->ResizeWindow( LoadImage.getContentsWidth() , LoadImage.getContentsHeight());
}


//ウィンドウプロシージャ(none static)
LRESULT RImageButton::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint( hwnd , &ps);
			BitBlt(ps.hdc ,
				ps.rcPaint.left,ps.rcPaint.top, 
				ps.rcPaint.right  - ps.rcPaint.left ,
				ps.rcPaint.bottom - ps.rcPaint.top ,
				LoadImage.getHDC(),
				ps.rcPaint.left,
				ps.rcPaint.top,
				SRCCOPY);
			::EndPaint(hwnd , &ps);
			return 0;
		}

	case BM_SETSTATE:
		{
			HDC dc = GetDC(hwnd);
			if (wparam == 1)
			{
				BitBlt(dc ,
					0 , 0, 
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					LoadImage.getHDC(),
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					BLACKNESS);
			}
			else
			{
				BitBlt(dc ,
					0 , 0, 
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					LoadImage.getHDC(),
					LoadImage.getContentsWidth() ,
					LoadImage.getContentsHeight() ,
					SRCCOPY);
			}
			//return 0;
		}

	}
	return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
}
