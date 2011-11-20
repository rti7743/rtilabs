// BGPSConfigDlg.h: BGPSConfigDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BGPSCONFIGDLG_H__DBA4E8BB_74C5_4607_8483_E6BA7AA5EF7F__INCLUDED_)
#define AFX_BGPSCONFIGDLG_H__DBA4E8BB_74C5_4607_8483_E6BA7AA5EF7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "RListView.h"
#include "RComboBox.h"
#include "REdit.h"

class BBanana;

class BGPSConfigDlg   : public RDialog  
{
public:
	BGPSConfigDlg();
	virtual ~BGPSConfigDlg();

	void Create(BBanana	*	poolBanana) throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);

	virtual void Show();
private:
	void OnCancel();
	void OnConnect();
	void OnDisConnect();

private:
	BBanana	*	PoolBanana;

	RComboBox	Port;
	RComboBox	Speed;
	RComboBox	Type;
	REdit		CheckTime;
	RComboBox	LogType;
	REdit		LogDir;
	RComboBox	TrackLog;

	RWindowBase	ConnectButtom;
	RWindowBase	DisConnectButtom;
};

#endif // !defined(AFX_BGPSCONFIGDLG_H__DBA4E8BB_74C5_4607_8483_E6BA7AA5EF7F__INCLUDED_)
