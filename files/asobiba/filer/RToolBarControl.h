// RToolBarControl.h: RToolBarControl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_)
#define AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "comm.h"

#include "RWindow.h"

class RToolBarControl  : public RWindow
{
public:
	RToolBarControl();
	virtual ~RToolBarControl();

	//作成
	void Create(HINSTANCE inInstance,
					HWND isParentWindow,int inID, DWORD inFlg) throw();

	//ボタンの作成
	void CreateButton(HINSTANCE inInstance,
				const char* inBitmap,TBBUTTON* inButton,int inButtonCount);

	//拡張項目のセット.
	void setExtended(DWORD inStyle);
	//拡張項目の取得.
	DWORD getExtended();

};

#endif // !defined(AFX_RTOOLBARCONTROL_H__BF00D8A4_754C_4965_9D20_9791C32E3A02__INCLUDED_)
