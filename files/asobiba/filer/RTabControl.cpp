// RTabControl.cpp: RTabControl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTabControl.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTabControl::RTabControl()
{

}

RTabControl::~RTabControl()
{
}

//作成
void RTabControl::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,WC_TABCONTROL,isParentWindow,inID,inFlg,0);
}

//タブをすべて削除する.
bool RTabControl::ClearTab()
{
	return Btob( TabCtrl_DeleteAllItems(this->getWindowHandle() ) );
}

//タブの追加.
bool RTabControl::InsertTab(int inItem,const char* inText,LPARAM inLPARAM)
{
	TCITEM	tc = {0};
	tc.mask = TCIF_TEXT | TCIF_PARAM;
	tc.pszText = (char*)inText;
	tc.lParam = inLPARAM;
	return Btob( TabCtrl_InsertItem(this->getWindowHandle() , inItem , &tc)  );
}

//タブの変更.
bool RTabControl::UpdateTab(int inItem,const char* inText,LPARAM inLPARAM)
{
	TCITEM	tc = {0};
	tc.mask = TCIF_TEXT | TCIF_PARAM;
	tc.pszText = (char*)inText;
	tc.lParam = inLPARAM;
	return Btob( TabCtrl_SetItem(this->getWindowHandle() , inItem , &tc)  );
}
//タブの取得
LPARAM RTabControl::GetTabParam(int inItem) const
{
	//取得
	TCITEM	tc = {0};
	tc.mask = TCIF_PARAM;
	TabCtrl_GetItem(this->getWindowHandle() , inItem , &tc  );

	return tc.lParam;
}

//タブの削除
bool RTabControl::DeleteTab(int inItem)
{
	return Btob( TabCtrl_DeleteItem(this->getWindowHandle() , inItem) );
}

//タブの数の取得
int RTabControl::GetTabCount() const
{
	//タブ数の取得.
	return TabCtrl_GetItemCount(this->getWindowHandle() );
}

//現在選択されているタブの位置を取得.
int RTabControl::GetCurSel() const
{
	return TabCtrl_GetCurSel(this->getWindowHandle() );
}

//指定したタブを選択.
void RTabControl::SelectTab(int inItem)
{
	TabCtrl_SetCurFocus( this->getWindowHandle() ,inItem);
}




