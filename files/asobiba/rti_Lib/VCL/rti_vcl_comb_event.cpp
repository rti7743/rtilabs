/**********************************************************/
// ボタンのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#include <.\VCL\rti_vcl_comb_event.h>

/**********************************************************/
//コンストラクタ   初期化します...
/**********************************************************/
TCombEvent::TCombEvent()
{
	//CBN_CLOSEUP	コンボ ボックスのリスト ボックスがクローズされたことを示します。 
	OnClose = NULL;
	//CBN_DROPDOWN	コンボ ボックスのリスト ボックスがドロップダウンされていることを示します。 
	OnDropDown = NULL;
	//CBN_EDITCHANGE	ユーザーがエディット コントロール内のテキストを変更したことを示します。 
	OnEditChange = NULL;
	//CBN_EDITUPDATE	代わりのテキストが表示されようとしていることを示します。 
	OnEditUpDate = NULL;
	//CBN_SELENDCANCEL	ユーザーの選択項目をキャンセルする必要があることを示します。 
	OnChange = NULL;
	//CBN_SELENDOK	ユーザーの選択項目が有効であることを示します。 
	OnCancel = NULL;
	//CBN_SETFOCUS	コンボ ボックスが入力フォーカスを受け取っていることを示します。 
	OnOk = NULL;
}

/**********************************************************/
//WM_COMMAND イベントディスパッチャ
/**********************************************************/
void TCombEvent::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	case CBN_CLOSEUP:	//コンボ ボックスのリスト ボックスがクローズされたことを示します。 
		if (OnClose) OnClose(this);
		break;
	case CBN_DROPDOWN:	//コンボ ボックスのリスト ボックスがドロップダウンされていることを示します。 
		if (OnDropDown) OnDropDown(this);
		break;
	case CBN_EDITCHANGE:	//ユーザーがエディット コントロール内のテキストを変更したことを示します。 
		if (OnEditChange) OnEditChange(this);
		break;
	case CBN_EDITUPDATE:	//代わりのテキストが表示されようとしていることを示します。 
		if (OnEditUpDate) OnEditUpDate(this);
		break;
	case CBN_SELCHANGE:	//コンボ ボックスで新しいリスト項目が選択されたことを示します。 
		if (OnChange) OnChange(this);
		break;
	case CBN_SELENDCANCEL:	//ユーザーの選択項目をキャンセルする必要があることを示します。 
		if (OnCancel) OnCancel(this);
		break;
	case CBN_SELENDOK:	//ユーザーの選択項目が有効であることを示します。 
		if (OnOk) OnOk(this);
		break;
	}
}

#endif
