// RTabBar.h: RTabBar クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_)
#define AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTabBar;

#include "comm.h"
#include "FilerSub.h"
#include "Window.h"

class RTabBar  
{
public:
	RTabBar(FilerSub* inFilerSub);
	virtual ~RTabBar();

	//サイズが変更されたときに呼び出される
	virtual void OnSize() = 0;
	//タブの追加
	virtual void InertTab(const Window* inWindow) = 0;
	//タブの削除
	virtual void DeleteTab(const Window* inWindow) = 0;
	//タブの変更
	virtual void UpdateTab(const Window* inWindow) = 0;

protected:
	FilerSub*	FSub;

};

#endif // !defined(AFX_RTABBAR_H__AFF10BAD_61E1_4657_B600_9C48282C32D7__INCLUDED_)
