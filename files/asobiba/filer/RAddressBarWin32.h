// RAddressBarWin32.h: RAddressBarWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_)
#define AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RAddressBar.h"
#include "WindowsMain.h"


class RAddressBarWin32 : public RAddressBar  
{
public:
	RAddressBarWin32(FilerSub* inFilerSub);
	virtual ~RAddressBarWin32();

	//サイズが変更されたときに呼び出される
	virtual void OnSize() ;
	//パスが変更されたときに呼び出される.
	virtual void OnAddressChange(string inNewPath);
private:

	//指定したパスを閲覧する.
	void PathView(string inPath);
	//新しいウィンドウでパスを閲覧する.
	void NewView(string inPath);

	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK AddressWindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{	//すぐに static ではないやつに譲渡する。
		RAddressBarWin32* classRet = 
			(RAddressBarWin32*) GetWindowLong( hwnd , GWL_USERDATA );
		if (classRet)
		{
			return classRet->AddressWindowProc(hwnd,msg,wparam,lparam);
		}
		return DefWindowProc(hwnd , msg , wparam,lparam);
	}
	//ウィンドウプロシージャ.
	LRESULT CALLBACK AddressWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND		WindowHandle;
	HWND		EditHandle;

	//Editのオリジナルのウィンドウプロシージャ.
	WNDPROC		AddressOrignalWindowProc;
};

#endif // !defined(AFX_RADDRESSBARWIN32_H__8509F3F2_31D8_4E07_94EC_EE3899296269__INCLUDED_)
