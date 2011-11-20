// FilerSub.cpp: FilerSub クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FilerSub.h"
#include "RAddressBarWin32.h"
#include "RTabBarWin32.h"
#include "RToolBarWin32.h"
#include "RStatueWin32.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FilerSub::FilerSub(bool inIsLeft)
{
	IsLeft = inIsLeft;

	AddressBar = new RAddressBarWin32(this);
	TabBar = new RTabBarWin32(this);
	ToolBar = new RToolBarWin32(this);
	StatueBar = new RStatueWin32(this);

	ActiveWindow = NULL;
	TargetWindow = NULL;
}

FilerSub::~FilerSub()
{
	delete AddressBar;
	delete TabBar;
	delete ToolBar;
	delete StatueBar;

	for(TAB_WINDOWS::iterator p = Wins.begin(); p != Wins.end() ; p++)
	{
		delete (*p);
	}
	Wins.clear();
}

//新しいウィンドウを開く
Window* FilerSub::NewWindow(string inPath)
{
	Window* win = new Window( this , inPath);
	Wins.push_back( win );
	TabBar->InertTab(win);

	if (ActiveWindow == NULL) ActiveWindow = win;

	return win;
}

//指定したウィンドウをアクティヴにする.
//TABから呼ぶこと!
bool FilerSub::setActiveWindow(Window* inWin)
{
	//自分の管理化?
	if ( !Search(inWin) ) return false;

	//すでにアクティヴ.
	if (ActiveWindow != inWin)
	{
		//非アクティヴを通知
		if (ActiveWindow) ActiveWindow->OnNoActive();
		//設定.
		ActiveWindow = inWin;
	}
	//アクティヴを通知.
	ActiveWindow->OnActive();

	//アドレバーを変更
	AddressBar->OnAddressChange( inWin->getNowPath() );
	//タブの更新だよもん。
	TabBar->UpdateTab(inWin);

	return true;
}

//指定したウィンドウをターゲットにする
bool FilerSub::setTargetWindow(Window* inWin)
{
	if (inWin != NULL )
	{
		//自分の管理化?
		if ( !Search(inWin) ) return false;
	}

	TargetWindow = inWin;
	return true;
}



//アクティヴウィンドウですか?
bool FilerSub::IsActiveWindow(const Window* inWin)
{
	return ActiveWindow == inWin;
}

//ウィンドウがリサイズされたときに呼ばれる
void FilerSub::OnWindowResize()
{
	AddressBar->OnSize();
	TabBar->OnSize();
	ToolBar->OnSize();
	StatueBar->OnSize();
}

//検索
bool FilerSub::Search(const Window* inWin) const
{
	return  (find(Wins.begin() , Wins.end() , inWin) != Wins.end());
}


//OnQuickChange が発生したときに呼ばれる
void FilerSub::OnQuickChange()
{
	ASSERT(ActiveWindow);

	//アドレバーを変更
	AddressBar->OnAddressChange( ActiveWindow->getNowPath() );
	//タブの更新だよもん。
	TabBar->UpdateTab( ActiveWindow );
}

//タイトルが変更された場合
void FilerSub::OnTitleChange()
{
	ASSERT(ActiveWindow);

	//タブの更新だよもん。
	TabBar->UpdateTab( ActiveWindow );
}

//ウィンドウを閉じる
bool FilerSub::CloseWindow(Window* inTargetWindow)
{
	//自分の管理下ではなければ没.
	TAB_WINDOWS::iterator p = find(Wins.begin() , Wins.end() , inTargetWindow);
	if (p == Wins.end() ) return false;

	//自分がアクティヴだったら、隣のヤツをアクティヴにする.
	//検索する順番は 右 -> 左
	if (ActiveWindow == inTargetWindow)
	{
		if (Wins.size() > 1)
		{//ほかにタブがあるはせあい
			TAB_WINDOWS::iterator next = (p + 1);
			if (next == Wins.end())
			{
				//実は自分が一番最後だった場合、左に検索.
				next = (p - 1);
			}
			ActiveWindow = NULL;	//自分はもう消えるので無効にしておく.
			setActiveWindow( *next );
		}
		else
		{//唯一のタブの場合
			ActiveWindow = NULL;
		}
	}

	//削除.
	delete *p;
	Wins.erase( p );

	TabBar->DeleteTab(inTargetWindow);

	return true;
}

