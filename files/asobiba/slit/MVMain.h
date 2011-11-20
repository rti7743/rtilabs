// RBanana.h: RBanana クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_)
#define AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "MVControl.h"
#include "MVSettingDialog.h"

class MVMain  : public RDialog
{
public:
	MVMain();
	virtual ~MVMain();

	void Create()throw(RException);

	void OnSettingChange();

	bool IsDialogMessage(MSG * inMessage);

	//ウィンドウプロシージャ.
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnKeyDown(unsigned char inKey);
	void OnDestory();
	void OnPaint();
	void OnCommand(int inID);
	void OnClick();
	void OnSize();

private:
	MVControl	Control;

	MVSettingDialog	SettingDialog;

	bool		IsFullScreen;
	string		NowOpenFilename;

#ifdef _WIN32_WCE
	HWND		RpCommandBar;
	SHACTIVATEINFO Sai;
#endif
};

#endif // !defined(AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_)
