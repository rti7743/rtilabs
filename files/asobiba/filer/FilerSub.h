// FilerSub.h: FilerSub クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_)
#define AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FilerSub;

#include "comm.h"
#include "RAddressBar.h"
#include "RTabBar.h"
#include "RToolBar.h"
#include "RStatue.h"
#include "Window.h"


typedef deque<Window*> TAB_WINDOWS;

class FilerSub  
{
private:
	//アドレスバー
	RAddressBar*	AddressBar;
	//タブ
	RTabBar*		TabBar;
	//ツールバー
	RToolBar*		ToolBar;
	//ステータスバー
	RStatue*		StatueBar;
	//左にあるの?
	bool			IsLeft;
	//タブで表示するウィンドウ
	TAB_WINDOWS		Wins;

	//現在表示されているウィンドウ
	Window*			ActiveWindow;
	//現在タブ上で選択されているウィンドウ.
	//表示されているだけではなくて、タブで右クリックしたときなどで変化する
	Window*			TargetWindow;
public:
	FilerSub(bool inIsLeft);
	virtual ~FilerSub();

	//左にあるの?
	bool getIsLeft() const
	{
		return IsLeft;
	}
	//指定したウィンドウをアクティヴにする.
	//TABから呼ぶこと!
	bool setActiveWindow(Window* inWin);
	//指定したウィンドウをターゲットにする
	bool setTargetWindow(Window* inWin);

	//アクティヴウィンドウですか?
	bool IsActiveWindow(const Window* inWin);
	//アクティヴなWindowの取得.
	Window* getActiveWindow() const
	{
		return ActiveWindow;
	}

	//ターゲットになっているウィンドウ
	//たとえば、タブで閉じるターゲットになっているウィンドウです。
	//もし、ターゲットがいない場合は、アクティヴなウィンドウを返します。
	Window* getTargetWindow() const
	{
		return TargetWindow != NULL ? TargetWindow : ActiveWindow;
	}

	
	//新しいウィンドウを開く
	Window* NewWindow(string inPath);
	//ウィンドウを閉じる
	bool CloseWindow(Window* inTargetWindow);
	//検索
	bool Search(const Window* inWin) const;
//イベント
	//OnQuickChange が発生したときに呼ばれる
	void OnQuickChange();
	//タイトルが変更された場合
	void OnTitleChange();
	//ウィンドウがリサイズされたときに呼ばれる
	void OnWindowResize();

	//ステータスバーの取得.
	RStatue*	getStatue()
	{
		return StatueBar;
	}
private:
};

#endif // !defined(AFX_FILERSUB_H__C6FEB548_35C6_4B9A_8000_4ACAAA3AEAE8__INCLUDED_)
