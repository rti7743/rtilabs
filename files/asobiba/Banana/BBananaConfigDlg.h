// BBananaConfigDlg.h: BBananaConfigDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BBANANACONFIGDLG_H__E32788D2_6375_4479_B862_5DC59F68566C__INCLUDED_)
#define AFX_BBANANACONFIGDLG_H__E32788D2_6375_4479_B862_5DC59F68566C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "REdit.h"
#include "RComboBox.h"

class BBanana;

class BBananaConfigDlg  : public RDialog  
{
public:
	BBananaConfigDlg();
	virtual ~BBananaConfigDlg();

	void Create(BBanana	*	poolBanana) throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnCancel();
	void OnOK();

	BBanana	*	Banana;
	REdit				BananaMap;
	REdit				BananaUserDir;
	RComboBox			NoRotate;
};

#endif // !defined(AFX_BBANANACONFIGDLG_H__E32788D2_6375_4479_B862_5DC59F68566C__INCLUDED_)
