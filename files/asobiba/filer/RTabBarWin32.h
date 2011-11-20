// RTabBarWin32.h: RTabBarWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_)
#define AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTabBarWin32;

#include "comm.h"
#include "RTabBar.h"
#include "RTabControl.h"
#include "RTableControl.h"

class RTabBarWin32 : public RTabBar  
{
public:
	RTabBarWin32(FilerSub* inFilerSub) throw();
	virtual ~RTabBarWin32();

	//サイズが変更されたときに呼び出される
	virtual void OnSize();
	//タブの追加
	virtual void InertTab(const Window* inWindow) ;
	//タブの削除
	virtual void DeleteTab(const Window* inWindow) ;
	//タブの変更
	virtual void UpdateTab(const Window* inWindow) ;


	//ウィンドウプロシージャ.
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	//選択されているタブが変化しますた。
	void OnSelChange();
	//右クリック
	bool OnRClick(const NMHDR * inNmhdr) const;

	//カレントのウィンドウの取得.
	Window* getCurrentWindow() const;
private:
	RTabControl		TabControl;
	RTableControl<RTabBarWin32>	Table;
};

#endif // !defined(AFX_RTABBARWIN32_H__150515BC_5246_430F_A1F9_99EA2EA8A106__INCLUDED_)
