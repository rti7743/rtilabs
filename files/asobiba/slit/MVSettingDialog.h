// MVSettingDialog.h: MVSettingDialog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_)
#define AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RDialog.h"
#include "RComboBox.h"

class MVMain;

class MVSettingDialog : public RDialog  
{
public:
	MVSettingDialog();
	virtual ~MVSettingDialog();

	void Create(MVMain	*	poolMVMain) throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnCancel();
	void OnOK();

	//閉じる前の状態の記録
	RComboBox			LastMemory;
	//ジャストフィット
	RComboBox			JustFit;
	//スクロールカウント
	RComboBox			ScrollCount;
	//スクロールタイプ
	RComboBox			ScrollType;

	MVMain *			PoolMVMain;
};

#include "MVMain.h"

#endif // !defined(AFX_MVSETTINGDIALOG_H__310F5E0D_1B29_4824_A8FD_E94F66EBE248__INCLUDED_)
