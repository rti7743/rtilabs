// RDialog.h: RDialog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDIALOG_H__B0BDF40D_B981_4E19_9A6C_C7C961C28E54__INCLUDED_)
#define AFX_RDIALOG_H__B0BDF40D_B981_4E19_9A6C_C7C961C28E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindowBase.h"
#include "RWin32Exception.h"

class RDialog  : public RWindowBase
{
public:
	RDialog();
	virtual ~RDialog();

	void Create(HINSTANCE inInstance ,HWND inParentWindow , const char* inResourceName) throw(RWin32Exception);
	int DoModel(HINSTANCE inInstance , HWND inParentWindow , const char* inResourceName) throw(RWin32Exception);

	HWND GetDlgItem(int inID);
	void EndDialog(int inExitCode);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam) = 0;
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		RDialog* This;
		if (msg == WM_INITDIALOG)
		{
			This = (RDialog*)lparam;

			//class にコールバックするためにサブクラス化
			This->MountWindow(hwnd);
			::SetWindowLong( hwnd , GWL_USERDATA,  (long)This);
		}
		else
		{
			This = (RDialog*)GetWindowLong( hwnd , GWL_USERDATA);
		}
		if (This != NULL ) return This->DlgProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
};

#endif // !defined(AFX_RDIALOG_H__B0BDF40D_B981_4E19_9A6C_C7C961C28E54__INCLUDED_)
