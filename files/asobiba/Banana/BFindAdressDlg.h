// BFindAdressDlg.h: BFindAdressDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BFINDADRESSDLG_H__AC790FDA_31D9_4B23_A0AF_DEB53E42A950__INCLUDED_)
#define AFX_BFINDADRESSDLG_H__AC790FDA_31D9_4B23_A0AF_DEB53E42A950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "RGISSmallFindAddress.h"
#include "RListView.h"
#include "REdit.h"
#include "BAddressMatching.h"

class BBanana;

class BFindAdressDlg : public RDialog  
{
public:
	BFindAdressDlg();
	virtual ~BFindAdressDlg();

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

	RGISSmallFindAddress	FindAddress;
};

#endif // !defined(AFX_BFINDADRESSDLG_H__AC790FDA_31D9_4B23_A0AF_DEB53E42A950__INCLUDED_)
