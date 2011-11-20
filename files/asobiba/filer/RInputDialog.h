// RInputDialog.h: RInputDialog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_)
#define AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDoNotInitaliseException.h"
#include "RNotSupportException.h"
#include "IMyEnumString.h"
#include "RWin32Exception.h"
#include "RNoComplateException.h"

class RInputDialog  
{
public:
	RInputDialog();
	virtual ~RInputDialog();

	string DoModal(string inTitle , string inAutoCompleteName , string inDefualt) throw(RWin32Exception,RNoComplateException);
private:
	//ダイアログ作成のときに呼ばれます。
	bool OnInitDialog();
	//コンプリートデータの読み込み.
	WStringList LoadComplete(string inName);
	//コンプリートデータの書き込み
	void SaveComplete(string inName , WStringList inTargetString);
	//OKをおされたときの処理.
	void OnOK();
	//キャンセルをおされたときの処理.
	void OnCancel();

	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK DlgProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_INITDIALOG)
		{
			::SetWindowLong( hwnd , GWL_USERDATA,  (long)lparam);
			((RInputDialog*)(lparam))->WindowHandle = hwnd;
		}
		RInputDialog* This = (RInputDialog*)GetWindowLong( hwnd , GWL_USERDATA);
		if (This != NULL ) return This->DlgProc(hwnd,msg,wparam,lparam);

	    return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ.
	LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	IAutoComplete*				AutoComplete;
	IMyEnumString*				EnumStr;
	string						InputData;

	string						Title;
	string						AutoCompleteName;
	string						Defualt;

	HWND		WindowHandle;
};

#endif // !defined(AFX_RINPUTDIALOG_H__8D463E23_392C_473A_8893_FA3DEAFD45CE__INCLUDED_)
