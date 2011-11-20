// BFindEkiDlg.h: BFindEkiDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BFINDEKIDLG_H__5E3CF69D_10A0_4259_9FC5_005F72ED2D3E__INCLUDED_)
#define AFX_BFINDEKIDLG_H__5E3CF69D_10A0_4259_9FC5_005F72ED2D3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "RGISSmallMaster.h"
#include "RGISSmallFindEki.h"
#include "RListView.h"
#include "REdit.h"

class BBanana;

class BFindEkiDlg  : public RDialog
{
public:
	BFindEkiDlg();
	virtual ~BFindEkiDlg();

	void Create(BBanana	*	poolBanana) throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnCancel();
	void OnSearch();
	bool OnDoubleClick();
	bool OnKeyDown(unsigned char inKey);

	BBanana	*	Banana;
	RListView			ListView;
	REdit				Word;

	RGISSmallFindEki	FindEki;
};

#endif // !defined(AFX_BFINDEKIDLG_H__5E3CF69D_10A0_4259_9FC5_005F72ED2D3E__INCLUDED_)
