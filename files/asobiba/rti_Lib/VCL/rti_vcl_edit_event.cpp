/**********************************************************/
// エディトのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#include <.\VCL\rti_vcl_edit_event.h>

/**********************************************************/
//コンストラクタ   初期化します...
/**********************************************************/
TEditEvent::TEditEvent()
{
	//編集されたとき
	OnChange  = NULL;
	//EN_HSCROLL	ユーザーがスクロール バーをクリックしたことを示します。 
	OnHScroll  = NULL;
	//EN_MAXTEXT	挿入部分が切り捨てられたことを示します。 
	OnMaxText  = NULL;
	//EN_UPDATE	エディット コントロールが代わりのテキストを表示しようとしていることを示します。 
	OnUpdate  = NULL;
	//EN_VSCROLL	ユーザーが垂直スクロール バーをクリックしたことを示します。 
	OnVscroll  = NULL;
}

/**********************************************************/
//入力されているヤツを int 型に変換して返します
/**********************************************************/
int TEditEvent::GetInt()
{
    char buf[256];
    //まず入力されているヤツを取得
    GetCaption(buf,256);
    return atoi(buf);
}

/**********************************************************/
//WM_COMMAND イベントディスパッチャ
/**********************************************************/
void TEditEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case EN_CHANGE:   //編集されたとき
		if (OnChange) OnChange(this);
		break;
	case EN_HSCROLL:	//ユーザーがスクロール バーをクリックしたことを示します。 
		if (OnHScroll) OnHScroll(this);
		break;
	case EN_MAXTEXT:	//挿入部分が切り捨てられたことを示します。 
		if (OnMaxText) OnMaxText(this);
		break;
	case EN_UPDATE:		//エディット コントロールが代わりのテキストを表示しようとしていることを示します。 
		if (OnUpdate) OnUpdate(this);
		break;
	case EN_VSCROLL:	//ユーザーが垂直スクロール バーをクリックしたことを示します。 
		if (OnVscroll) OnVscroll(this);
		break;
	}
}

#endif
