/**********************************************************/
// ボタンのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#if defined(BUTTON_LIFE) || defined(CHECKBUTTON_LIFE) || defined(GROUPBOX_LIFE) || defined(RADIOBUTTON_LIFE)

#ifndef ___TBUTTONEVENTH
#define ___TBUTTONEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//ボタンのイベントクラス
//ボタンから作るコンポーネントはこれを継承させます.
//BUTTON CHECKBUTTON RADIOBUTTON GROUPBOX

class TButtonEvent	: public TBaseObject
{
public:
	TButtonEvent();
	void WmCommandCome(WPARAM wParam);
	//ボタンをセットする(チェックをつける (ラジオも有効))
	void Set()
		{SendMessage(hWnd,BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);};
	//ボタンをリセットする(チェックを消す (ラジオも有効))
	void ReSet()
		{SendMessage(hWnd,BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);};
	//ボタンの状態を得る(ラジオも有効)
	int Get()
		{return SendMessage(hWnd,BM_GETCHECK, 0L, 0L);};

	//BN_CLICKED	ユーザーがボタンをクリックしたことを示します。 
	SimpleEvent	OnClick;
	//BN_HILITE	ユーザーがボタンを反転表示させたことを示します。 
	SimpleEvent	OnHilite;
	//BN_UNHILITE	反転表示を除去する必要があることを示します。 
	SimpleEvent	OnUnHilite;
};


#endif

#endif
