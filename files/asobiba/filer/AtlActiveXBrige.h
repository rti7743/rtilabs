// AtlActiveXBrige.h: AtlActiveXBrige クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATLACTIVEXBRIGE_H__C7A4226C_FD0B_4C88_A756_0C24A690726E__INCLUDED_)
#define AFX_ATLACTIVEXBRIGE_H__C7A4226C_FD0B_4C88_A756_0C24A690726E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RAutoLibrary.h"

//ActiveX周り降参します。
//ATL使わせてください。
//参考 http://snak.tdiary.net/200201.html
class AtlActiveXBrige  
{
private:
	AtlActiveXBrige();		//singleton
public:
	virtual ~AtlActiveXBrige();

	static AtlActiveXBrige* getInstance() ;

	HRESULT AtlAxGetControl(HWND inWindow , IUnknown** outPP);

	HRESULT AtlAxCreateControl(LPCOLESTR inName, HWND inWindow, IStream* inStream, IUnknown** outUnkContainer);

	HRESULT AtlAxAttachControl(IUnknown* inControl, HWND inWindow, IUnknown** outUnkContainer);

private:
	//bool __stdcall AtlAxWinInit()
	typedef bool (__stdcall *ATLAXWININIT)    ();
	ATLAXWININIT ExportAtlAxWinInit;
	//HRESULT __stdcall AtlAxGetControl(HWND h, IUnknown** pp)
	typedef HRESULT (__stdcall *ATLAXGETCONTORL)    (HWND h, IUnknown** outPP);
	ATLAXGETCONTORL ExportAtlAxGetControl;

	//HRESULT __stdcall AtlAxCreateControl(LPCOLESTR lpszName, HWND hWnd, IStream* pStream, IUnknown** ppUnkContainer);
	typedef HRESULT (__stdcall *ATLAXCREATECONTORL)    (LPCOLESTR lpszName, HWND hWnd, IStream* pStream, IUnknown** ppUnkContainer);
	ATLAXCREATECONTORL ExportAtlAxCreateControl;

	//HRESULT __stdcall AtlAxAttachControl(IUnknown* pControl, HWND hWnd, IUnknown** ppUnkContainer);
	typedef HRESULT (__stdcall *ATLAXATTACHCONTROL)    (IUnknown* pControl, HWND hWnd, IUnknown** ppUnkContainer);
	ATLAXATTACHCONTROL ExportAtlAxAttachControl;

	RAutoLibrary	Lib;
};

#endif // !defined(AFX_ATLACTIVEXBRIGE_H__C7A4226C_FD0B_4C88_A756_0C24A690726E__INCLUDED_)
