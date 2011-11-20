// RToolBarControl.cpp: RToolBarControl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RToolBarControl.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RToolBarControl::RToolBarControl()
{

}

RToolBarControl::~RToolBarControl()
{
}

//作成
void RToolBarControl::Create(HINSTANCE inInstance,
			HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	RWindow::Create(inInstance,TOOLBARCLASSNAME,isParentWindow,inID,inFlg,0);
	
}

//ボタンの作成
void RToolBarControl::CreateButton(HINSTANCE inInstance,
						const char* inBitmap,TBBUTTON* inButton,int inButtonCount)
{
	SendMessage(this->getWindowHandle() , TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0); 
	
	TBADDBITMAP	tbab;
	tbab.hInst = NULL;
	tbab.nID = (UINT) ::LoadImage(NULL , inBitmap,IMAGE_BITMAP , 0 , 0 ,LR_LOADFROMFILE);

	SendMessage(this->getWindowHandle(), TB_ADDBITMAP, inButtonCount, (LPARAM)&tbab);
	SendMessage(this->getWindowHandle(), TB_ADDBUTTONS, (WPARAM)inButtonCount, (LPARAM)inButton); 
}



//拡張項目のセット.
void RToolBarControl::setExtended(DWORD inStyle)
{
	SendMessage(this->getWindowHandle(),  TB_SETEXTENDEDSTYLE , 0, (LPARAM)inStyle);
}

//拡張項目の取得.
DWORD RToolBarControl::getExtended()
{
	return (DWORD) SendMessage(this->getWindowHandle(),  TB_GETEXTENDEDSTYLE , 0, 0);
}

