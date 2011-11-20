/**********************************************************/
// ボタンのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#if defined(BUTTON_LIFE) || defined(CHECKBUTTON_LIFE) || defined(GROUPBOX_LIFE) || defined(RADIOBUTTON_LIFE)

#include <.\VCL\rti_vcl_button_event.h>

/**********************************************************/
//コンストラクタ   初期化します...
/**********************************************************/
TButtonEvent::TButtonEvent()
{
	//BN_CLICKED	ユーザーがボタンをクリックしたことを示します。 
	OnClick		= NULL;
	//BN_HILITE	ユーザーがボタンを反転表示させたことを示します。 
	OnHilite	= NULL;
	//BN_UNHILITE	反転表示を除去する必要があることを示します。 
	OnUnHilite	= NULL;
}

/**********************************************************/
//WM_COMMAND イベントディスパッチャ
/**********************************************************/
void TButtonEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case BN_CLICKED:	//ユーザーがボタンをクリックしたことを示します。 
		if(OnClick) OnClick(this);
		break;
	case BN_HILITE:	//ユーザーがボタンを反転表示させたことを示します。 
		if(OnHilite) OnHilite(this);
		break;
	case BN_UNHILITE:	//反転表示を除去する必要があることを示します。 
		if(OnUnHilite) OnUnHilite(this);
		break;
	}
}

#endif
