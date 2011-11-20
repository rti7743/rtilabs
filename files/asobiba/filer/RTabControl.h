// RTabControl.h: RTabControl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_)
#define AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"


class RTabControl  : public RWindow
{
public:
	RTabControl();
	virtual ~RTabControl();

	//作成.
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//タブをすべて削除する.
	bool ClearTab();
	//タブの追加.
	bool InsertTab(int inItem,const char* inText,LPARAM inLPARAM);
	//タブの変更.
	bool UpdateTab(int inItem,const char* inText,LPARAM inLPARAM);
	//タブの取得
	LPARAM GetTabParam(int inItem) const;
	//タブの削除
	bool DeleteTab(int inItem);
	//タブの数の取得
	int GetTabCount() const;
	//指定したタブを選択.
	void SelectTab(int inItem);

	//現在選択されているタブの位置を取得.
	int GetCurSel() const;

};

#endif // !defined(AFX_RTABCONTROL_H__E645CE0A_B5D3_49D4_BCD3_54F48727B83C__INCLUDED_)
