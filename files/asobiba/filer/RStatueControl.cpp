// RStatueControl.cpp: RStatueControl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RStatueControl.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStatueControl::RStatueControl()
{

}

RStatueControl::~RStatueControl()
{

}

//作成.
void RStatueControl::Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RWindow::Create(inINstance,STATUSCLASSNAME,isParentWindow,inID,inFlg,0);
}

