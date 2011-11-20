/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#include <.\VCL\rti_vcl_list_event.h>

/**********************************************************/
//コンストラクタ   初期化します...
/**********************************************************/
TListBoxEvent::TListBoxEvent()
{
	//LBN_SELCANCEL	項目の選択がキャンセルされたことを示します。 
	OnCancel	= NULL;
	//LBN_SELCHANGE	選択項目が変更されようとしていることを示します。 
	OnChange	= NULL;
}
/**********************************************************/
//WM_COMMAND イベントディスパッチャ
/**********************************************************/
void TListBoxEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case LBN_SELCANCEL:	//項目の選択がキャンセルされたことを示します。 
		if (OnCancel) OnCancel(this);
		break;
	case LBN_SELCHANGE:	//選択項目が変更されようとしていることを示します。 
		if (OnChange) OnChange(this);
		break;
	}
}

#endif
