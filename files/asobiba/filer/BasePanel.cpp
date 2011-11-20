// BasePanel.cpp: BasePanel クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "BasePanel.h"
#include "WindowsMain.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BasePanel::BasePanel()
{
	this->WindowHandle = NULL;
}

BasePanel::~BasePanel()
{
	::DestroyWindow(this->WindowHandle);
}

//ウィンドウの作成
//失敗時はRDoNotInitaliseException例外.
void BasePanel::Create(bool isLeft) throw()
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	

    this->WindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE , "STATIC",
				            "",WS_CHILD | WS_VISIBLE | WS_THICKFRAME ,
						    CW_USEDEFAULT, CW_USEDEFAULT,
							CW_USEDEFAULT, CW_USEDEFAULT,
							mainSystem->getWindowHandle(), (HMENU)mainSystem->getID(),
							mainSystem->getInstance(), NULL);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("BasePanelの作成に失敗");
	}
	this->OrignalWndProc = (WNDPROC)GetWindowLong(this->WindowHandle , GWL_WNDPROC );
	SetWindowLong(this->WindowHandle , GWL_WNDPROC ,  (long) WndProcStatic);
	SetWindowLong(this->WindowHandle , GWL_USERDATA , (long)this);
}


//ウィンドウプロシージャ(none static)
LRESULT BasePanel::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT ret;
	for ( WNDPROCCALLABLE::iterator it = WndProcCallOnList.begin() ; it != WndProcCallOnList.end() ; ++it)
	{
		//もし処理できたら true が帰ってきて、 ret に戻り地がセットされているので
		//それを変えします。
		if ( (*it)->WndProc(hwnd,msg, wparam, lparam,&ret) )
		{
			return ret;
		}
	}
	return ::CallWindowProc( OrignalWndProc ,hwnd,msg,wparam,lparam);
}

