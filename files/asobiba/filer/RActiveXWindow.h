// RActiveXWindow.h: RActiveXWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RACTIVEXWINDOW_H__4129415D_EE00_435F_AC76_5DE39A6C43A1__INCLUDED_)
#define AFX_RACTIVEXWINDOW_H__4129415D_EE00_435F_AC76_5DE39A6C43A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNormalWindow.h"
#include "AtlActiveXBrige.h"
#include "RNotSupportException.h"

class RActiveXWindow : public RWindow
{
public:
	RActiveXWindow();
	virtual ~RActiveXWindow();

	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,const string inProgID, IID inIID , void** ioInterface,
						   HWND isParentWindow,int inID, DWORD inFlg,DWORD inExStyle) 
						   throw(RNotSupportException,RDoNotInitaliseException);

private:
	IUnknown*				Unknown;
	//サブクラス化.
	WNDPROC			OrignalWindowProc;
};

#endif // !defined(AFX_RACTIVEXWINDOW_H__4129415D_EE00_435F_AC76_5DE39A6C43A1__INCLUDED_)
