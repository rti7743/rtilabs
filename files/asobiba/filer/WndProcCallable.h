// WndProcCallable.h: WndProcCallable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_)
#define AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//interface class
class WndProcCallable  
{
protected:
	WndProcCallable()
	{
	}
	virtual ~WndProcCallable()
	{
	}
public:
	//WindowProcを受け取る
	//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
	//自分は処理しない奴は false を返す。
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet) = 0;
};

#endif // !defined(AFX_WNDPROCCALLABLE_H__4914344B_5B74_4EF9_9DE5_C4A76A0519FB__INCLUDED_)
