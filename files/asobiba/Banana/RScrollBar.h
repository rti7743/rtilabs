// RScrollBar.h: RScrollBar クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_)
#define AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindowBase.h"

class RScrollBar  : public RWindowBase  
{
public:
	RScrollBar();
	virtual ~RScrollBar();

	//作成. オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//状態のセット.
	int SetScrollInfo(int inFlg ,const SCROLLINFO * inInfo , BOOL inRedraw );
	//状態の取得.
	int GetScrollInfo(int inFlg ,SCROLLINFO * outInfo ) const;

//サービス.
	//現在位置の取得
	int getPos(int inFlg) const;
	//位置の設定.
	void setPos(int inPos , int inFlg) ;
	//実際のスクロール(WM_VSCROLL / WM_HSCROLL) が発生したときに スクロールバーの位置を調整します.
	//true が帰ってきた場合は、 こいつに入れた ioTargetScrollInfo を SetScrollInfo で設定するといいでしょう.
	bool CalsScroll(SCROLLINFO * ioTargetScrollInfo , WPARAM inWParam) const;

};

#endif // !defined(AFX_RSCROLLBAR_H__FD3E1B7B_948D_4C3B_B924_6FB6528D9F2D__INCLUDED_)
