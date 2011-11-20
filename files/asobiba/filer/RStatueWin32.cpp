// RStatueWin32.cpp: RStatueWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "comm.h"
#include "RStatueWin32.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStatueWin32::RStatueWin32(FilerSub* inFilerSub) : RStatue(inFilerSub)
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//ToolBarをのせる台の作成.
	Table.Create(mainSystem->getInstance(),
					  mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
				      mainSystem->getID() ,
					  WS_CHILD | WS_VISIBLE,
					  this,
					  &RStatueWin32::WindowProc);

	//タブコントロールを作成します。
	//失敗したら RDoNotInitaliseException を投げます。
	StatueControl.Create(mainSystem->getInstance(),
					  Table.getWindowHandle(), 
				      mainSystem->getID() ,
					  WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE);

	//プログレスバーを作ります。
	ProgressBar.Create( mainSystem->getInstance() , 
						mainSystem->getBaseWindow( FSub->getIsLeft() ) , 
						mainSystem->getID() , 
						WS_VISIBLE);
	ProgressBar.setRange(0,100);
	ProgressBar.setGradationRange(RGB(0,0,255) , RGB(0,255,0) );
	ProgressBar.setPos(0);

	//ステータスバーを親とします。
	SetParent( ProgressBar.getWindowHandle() , StatueControl.getWindowHandle() );

	OnSize();
}

RStatueWin32::~RStatueWin32()
{

}

//サイズが変更されたときに呼び出される
void RStatueWin32::OnSize()
{
	RECT itemRC;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	::GetClientRect( mainSystem->getBaseWindow( FSub->getIsLeft() ) , &itemRC);

	Table.MoveWindow(itemRC.left , itemRC.bottom - STATUS_BAR_SIZE , itemRC.right , STATUS_BAR_SIZE);
	StatueControl.MoveWindow(0 , 0 , itemRC.right , STATUS_BAR_SIZE );

	//ますめの作成
	int sbSize[3];
	sbSize[0] = itemRC.right * 0.70 ;
	sbSize[1] = itemRC.right * 0.20 ;
	sbSize[2] = -1 ;

	SendMessage(StatueControl.getWindowHandle() , SB_SETPARTS, (WPARAM)3, (LPARAM)sbSize);
	ShowWindow(StatueControl.getWindowHandle(), SW_SHOW); 

	//プログレスバーを移動.
	ProgressBar.MoveWindow( sbSize[0] , 0 , sbSize[1] , STATUS_BAR_SIZE );
}


//ウィンドウプロシージャ.
LRESULT RStatueWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;
			switch(nmhdr->code)
			{
			case NM_RCLICK:		//右クリックされました.
				break;
			}
		}
		break;
	}
	return Table.CallWindowProc( hwnd,msg,wparam,lparam);
}


//ステータスのセット.
void RStatueWin32::setStatue(string inStatue)
{
	SendMessage(StatueControl.getWindowHandle(), SB_SETTEXT, (WPARAM)0 | 0, (LPARAM)(LPSTR)inStatue.c_str() );
}

//プログレスバー
void RStatueWin32::setProgressBar(int inParsent)
{
	//補正
	if (inParsent > 100) inParsent = 100;
	else if (inParsent < 0)	 inParsent = 0;

	//位置表示.
	ProgressBar.setPos(inParsent);
}
