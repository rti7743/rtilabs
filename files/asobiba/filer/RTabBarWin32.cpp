// RTabBarWin32.cpp: RTabBarWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTabBarWin32.h"
#include "WindowsMain.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTabBarWin32::RTabBarWin32(FilerSub* inFilerSub) : RTabBar(inFilerSub)
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//ToolBarをのせる台の作成.
	Table.Create(mainSystem->getInstance(),
					  mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE,
					  this,
					  &RTabBarWin32::WindowProc);

	//タブコントロールを作成します。
	//失敗したら RDoNotInitaliseException を投げます。
	TabControl.Create(mainSystem->getInstance(),
					  Table.getWindowHandle(), 
				      mainSystem->getID() ,
					  WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);
	OnSize();
}

RTabBarWin32::~RTabBarWin32()
{

}

//サイズが変更されたときに呼び出される
void RTabBarWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	Table.MoveWindow(itemRC.left , itemRC.top + TAB_BAR_START , itemRC.right , itemRC.top + TAB_BAR_SIZE );
	TabControl.MoveWindow(0 , 0 , itemRC.right , TAB_BAR_SIZE );
}

//タブの追加
void RTabBarWin32::InertTab(const Window* inWindow) 
{
	//タブ数の取得.
	int last = TabControl.GetTabCount();

	TabControl.InsertTab(last , "loading..." , (LPARAM)inWindow);
	TabControl.SelectTab(last);
}

//タブの削除
void RTabBarWin32::DeleteTab(const Window* inWindow) 
{
	//タブ数の取得.
	int last = TabControl.GetTabCount();

	//inWindowのタブを検索...
	for(int i = 0; i < last ; i++)
	{
		if ( (TabControl.GetTabParam(i)) == (LPARAM)inWindow)
		{
			//見つけたので削除
			TabControl.DeleteTab(i);
			return ;
		}
	}
}

//タブの変更
void RTabBarWin32::UpdateTab(const Window* inWindow) 
{
	//タブ数の取得.
	int last = TabControl.GetTabCount();

	//inWindowのタブを検索...
	for(int i = 0; i < last ; i++)
	{
		if ( (TabControl.GetTabParam(i)) == (LPARAM)inWindow)
		{
			//見っけ.
			TabControl.UpdateTab(i, inWindow->getTitle().c_str() , (LPARAM)inWindow);
			::UpdateWindow( TabControl.getWindowHandle() );
			return;
		}
	}
}

//ウィンドウプロシージャ.
LRESULT RTabBarWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;
			switch(nmhdr->code)
			{
			case TCN_SELCHANGE:	//タブが変更されたとき.
				{
					this->OnSelChange();
					return 0;
				}
				break;
			case NM_RCLICK:		//右クリックされました.
				if (this->OnRClick((LPNMHDR)lparam) )
				{
					return 0;
				}
				break;
			}
		}
		break;
	}
	return Table.CallWindowProc( hwnd,msg,wparam,lparam);
}

//選択されているタブが変化しますた。
void RTabBarWin32::OnSelChange()
{
	//Windowの取得.
	Window* win = getCurrentWindow();
	if (win == NULL) 	return ;

	//アクティヴにする.
	bool ret = FSub->setActiveWindow( win );
	if (ret)
	{
		TRACE("Tabが選択されたので %p をアクティヴにしました\n",win);
	}
}


//右クリック
bool RTabBarWin32::OnRClick(const NMHDR * inNmhdr) const
{
	//Windowの取得.
	Window* win = getCurrentWindow();
	if (win == NULL) 	return false;


	try
	{
		//ターゲット設定
		FSub->setTargetWindow(win);

		//そのウィンドウのポップアップメニューを表示.
		win->PopupMenu("TabPopup");

		//ターゲットクリア
		FSub->setTargetWindow(NULL);
	}
	catch(RException e)
	{
		printf("エラーが発生しました:\n%s",e.getMessage() );
		return false;
	}

	return true;	
}

//カレントのウィンドウの取得.
Window* RTabBarWin32::getCurrentWindow() const
{
	int index = TabControl.GetCurSel();
	if (index < 0)
	{
		TRACE("負のタグがアクティヴになりました　内部エラー\n");
		return NULL;
	}

	//Windowの取得.
	Window* win = (Window*)TabControl.GetTabParam(index);
	if (win == NULL) 
	{
		TRACE("NULLのタグがアクティヴになりました　内部エラー\n");
		return NULL;
	}
	return win;
}
