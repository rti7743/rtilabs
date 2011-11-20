// RSpliter.h: RSpliter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_)
#define AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



//ラバーバンド
//参考 http://www7.big.or.jp/~hina/zidney/programing/sample/rubberband/
class RSpliter  
{
public:
	RSpliter();
	virtual ~RSpliter();

	//ひだりボタンが押された
	bool OnLButtonDown(int x,int y);
	//ひだりボタンが離された
	bool OnLButtonUp(int x,int y);
	//マウスが移動した
	bool OnMouseMove(int x,int y);

	//親ウィンドウ
	void setParentWnidow(HWND inParentWnidow);
	//ラバーバンドのサイズの設定
	void setRubberBandSize(int inRubberBandSize);
	//左ウィンドウの幅の設定
	void setLeftWindowSize(int inNewLeftWidth);
	//左ウィンドウの幅の設定
	void setLeftWindowSize(double inNewLeftParsent);

	//ラバーランドの左/右のサイズの取得
	void getWindowRect(bool IsLeft,RECT* outRect) const;

private:
	// 左のウィンドウのサイズを正規化して返す
	int RegularWidth(int inWidth) const;
	//スプリッターを描画.
	void DrawSpliter(int inWidth) const;

private:
	//親ウィンドウ.
	HWND		ParentWnidow;
	//ラバーバンドのサイズ.
	int			RubberBandSize;
	//左ウィンドウのサイズ.
	int			LeftWindowSize;

	// スプリッター上で左ボタンが押されたときの位置
	int			MouseDiff;
	// 前回スプリッターを書いた座標
	int			OldWidth;
};

#endif // !defined(AFX_RSPLITER_H__CF508D70_DC61_4E31_850A_8EB54A8C81AA__INCLUDED_)
