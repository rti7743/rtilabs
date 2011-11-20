// RListViewWithMouseGesture.cpp: RListViewWithMouseGesture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RListViewWithMouseGesture.h"
#include "filer.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RListViewWithMouseGesture::RListViewWithMouseGesture()
{

}

RListViewWithMouseGesture::~RListViewWithMouseGesture()
{

}

//作成.	オーバーライド
void RListViewWithMouseGesture::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RListView::Create(inINstance , isParentWindow , inID , inFlg);

	//マウスジェスチャーにターゲットのウィンドウを教える.
	MouseGestureMouseCapture.setParentWnidow( this->getWindowHandle() ) ;
	//よーしパパ、サブクラスにしちゃうぞー
	this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
	SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
	SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WndProcStatic);
}

//ウィンドウプロシージャ(none static)
LRESULT RListViewWithMouseGesture::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		if ( MouseGestureMouseCapture.OnRButtonDown(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_RBUTTONUP:
		if ( MouseGestureMouseCapture.OnRButtonUp(LOWORD(lparam) , HIWORD(lparam)) ) return 0;

		//ジェスチャーに失敗
		{
			const HWND win = this->getWindowHandle() ;
			const int id = ::GetWindowLong( win , GWL_ID);
			//WM_NOTIFY  --> NM_RCLICK  を偽装
			NMHDR nmhdr;
			nmhdr.code = NM_RCLICK;
			nmhdr.hwndFrom = win;
			nmhdr.idFrom = id;
			SendMessage( ::GetParent( win ) , WM_NOTIFY , (WPARAM) id ,(LPARAM)&nmhdr );
			return 0;
		}
		break;
	case WM_MOUSEMOVE:
		if ( MouseGestureMouseCapture.OnMouseMove(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		return 0;	//やらないとどっかでキャプチャーされっぱなしになる?
		break;
	}
	return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
}
