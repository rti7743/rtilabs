// RStatue.h: RStatue クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTATUE_H__BA30BCE6_77F3_47E7_A713_1FF8BB03249D__INCLUDED_)
#define AFX_RSTATUE_H__BA30BCE6_77F3_47E7_A713_1FF8BB03249D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RStatue ;

#include "comm.h"
#include "FilerSub.h"
#include "Window.h"

class RStatue  
{
public:
	RStatue(FilerSub* inFilerSub);
	virtual ~RStatue();

	//サイズが変更されたときに呼び出される
	virtual void OnSize() = 0;
	//ステータスのセット.
	virtual void setStatue(string inStatue) = 0;
	//プログレスバー
	virtual void setProgressBar(int inParsent) = 0;

protected:
	FilerSub*	FSub;

};

#endif // !defined(AFX_RSTATUE_H__BA30BCE6_77F3_47E7_A713_1FF8BB03249D__INCLUDED_)
