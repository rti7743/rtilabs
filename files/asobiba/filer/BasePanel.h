// BasePanel.h: BasePanel クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_)
#define AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "WndProcCallable.h"
#include "RDoNotInitaliseException.h"


class BasePanel
{
public:
	BasePanel();
	virtual ~BasePanel();

	//ウィンドウの作成
	//失敗時はRDoNotInitaliseException例外.
	void Create(bool isLeft) throw();
	//WndProcの処理の横流しに登録する.
	void registeCallbackWndProc(WndProcCallable * inProc)
	{
		this->WndProcCallOnList.push_back(inProc);
	}
	//WndProcの処理の横流しの解除
	void UnregisteCallbackWndProc(WndProcCallable * inProc)
	{
		for ( WNDPROCCALLABLE::iterator it = WndProcCallOnList.begin() ; it != WndProcCallOnList.end() ; ++it)
		{
			if ( (*it) == inProc) 
			{
				WndProcCallOnList.erase(it);
				return ;
			}
		}
	}
	//移動.
	void MoveWindow(int x,int y,int width,int height)
	{
		::MoveWindow(getWindowHandle() , x ,y , width , height , TRUE);
	}
	HWND getWindowHandle() const
	{
		return WindowHandle;
	}
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		BasePanel* p = (BasePanel*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//WndProcの横流しを希望するリスト.
	typedef deque<WndProcCallable*> WNDPROCCALLABLE;
	WNDPROCCALLABLE WndProcCallOnList;

	HWND		WindowHandle;

	WNDPROC		OrignalWndProc;
};

#endif // !defined(AFX_BASEPANEL_H__44100190_4A8E_4CA7_8F66_9F79CD2AAA39__INCLUDED_)
