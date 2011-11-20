/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef TOOLBAR_LIFE

#include <.\VCL\rti_vcl_toolbar_event.h>

/**********************************************************/
//コンストラクタ   初期化します...
/**********************************************************/
TToolBarEvent::TToolBarEvent()
{
    //ボタンの個数初期化
    ButtonCounter = 0;
}
/**********************************************************/
//デストラクタ   
/**********************************************************/
TToolBarEvent::~TToolBarEvent()
{
    ToolBarUsingIT it;
    for(it  = VToolBarUsing.begin() ;
		it != VToolBarUsing.end() ; it ++)
	{
	    delete (*it);
	}
	//リストのすべてのデータの破棄
	VToolBarUsing.clear();
}
/**********************************************************/
//WM_COMMAND イベントディスパッチャ
/**********************************************************/
void TToolBarEvent::WmCommandCome(WPARAM wParam)
{
    //ボタン検索
    FindCommandToButton(wParam);
//	switch(HIWORD(wParam))
//	{
//	}
}

/**********************************************************/
//WM_COMMAND から、ボタンを探す
/**********************************************************/
bool TToolBarEvent::FindCommandToButton(WPARAM wParam)
{
    //そのメッセージを検索
    ToolBarUsingIT it;
    for(it  = VToolBarUsing.begin() ;
		it != VToolBarUsing.end() ; it ++)
	{ 
	    if ((*it)->idCommand == wParam) break;  //発見
	}
    if(it == VToolBarUsing.end()) return false ; //検索失敗

    //イベントを起こす
    if ((*it)->CallRtn)
            (*it)->CallRtn(this);
    return true;
}

#endif
