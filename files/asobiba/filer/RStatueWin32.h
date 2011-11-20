// RStatueWin32.h: RStatueWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_)
#define AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStatue.h"
#include "RStatueControl.h"
#include "RTableControl.h"
#include "RProgressBar.h"

class RStatueWin32 : public RStatue
{
public:
	RStatueWin32(FilerSub* inFilerSub);
	virtual ~RStatueWin32();

	//ステータスのセット.
	virtual void setStatue(string inStatue);
	//プログレスバー
	virtual void setProgressBar(int inParsent);

	//ウィンドウプロシージャ.
	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//サイズが変更されたときに呼び出される
	virtual void OnSize();

private:
	RStatueControl		StatueControl;
	RTableControl<RStatueWin32>	Table;

	//プログレスバー
	RProgressBar		ProgressBar;

};

#endif // !defined(AFX_RSTATUEWIN32_H__AC0ED636_13FF_4678_905C_4B567CFE1C4A__INCLUDED_)
