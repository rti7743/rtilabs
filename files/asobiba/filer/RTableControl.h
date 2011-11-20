// RTableControl.h: RTableControl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_)
#define AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"

template<class _CallBackClass>
class RTableControl : public RWindow  
{
public:
	RTableControl()
	{
	}
	virtual ~RTableControl()
	{
	}

	typedef LRESULT (_CallBackClass::*CALLBACK_WNDPROC)(HWND,UINT,WPARAM,LPARAM);

	//作成
	void Create(HINSTANCE inINstance,HWND isParentWindow,
		int inID , DWORD inFlg,
		_CallBackClass* inThis,CALLBACK_WNDPROC inCallback) throw()
	{
		//作成.
		RWindow::Create(inINstance,"STATIC",isParentWindow,inID,inFlg,WS_EX_TRANSPARENT);
		//コールバック先の保存
		this->This = inThis;
		this->Callback = inCallback;
		//サブクラス化.
		this->OrignalWindowProc = (WNDPROC) GetWindowLong( this->getWindowHandle() , GWL_WNDPROC );
		SetWindowLong( this->getWindowHandle() , GWL_USERDATA, (LONG)this );
		SetWindowLong( this->getWindowHandle() , GWL_WNDPROC, (LONG)WindowProcStatic);
	}
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{	//すぐに static ではないやつに譲渡する。
		RTableControl* classRet = 
			(RTableControl*) GetWindowLong( hwnd , GWL_USERDATA );
		if (classRet)
		{	//指定されたクラスの関数をコールバックデス!!
			return (classRet->This->*(classRet->Callback))(hwnd,msg,wparam,lparam);
		}
		return DefWindowProc(hwnd , msg , wparam,lparam);
	}
	
	//OrignalWindowProc を呼ぶ。
	LRESULT CallWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return ::CallWindowProc( OrignalWindowProc ,hwnd,msg,wparam,lparam);
	}

private:
	//オリジナルのWindowプロシージャ
	WNDPROC				OrignalWindowProc;
	CALLBACK_WNDPROC	Callback;
	_CallBackClass*		This;

};

#endif // !defined(AFX_RTABLECONTROL_H__07CC3C9D_121E_456E_A731_358DAB42CD77__INCLUDED_)
