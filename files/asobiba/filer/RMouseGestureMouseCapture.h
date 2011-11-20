// RMouseGestureMouseCapture.h: RMouseGestureMouseCapture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_)
#define AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMouseGestureMousePoints.h"

//マウスの動きをキャプチャー
class RMouseGestureMouseCapture  
{
	enum MouseCaptureState
	{
		NO_CAPTURE,
		PRE_CAPTURE,
		NOW_CAPTURE,
	};
public:
	RMouseGestureMouseCapture();
	virtual ~RMouseGestureMouseCapture();

	//右ボタンが押された	short にしないと符号の関係ではまる.
	bool OnRButtonDown(short x,short y);
	//右ボタンが離された
	bool OnRButtonUp(short x,short y);
	//マウスが移動した
	bool OnMouseMove(short x,short y);

	//ポイントを取得
	//インライン展開を激しくｷﾎﾞﾝﾇ.
	const MOUSE_GESTURE_MOUSE_POINTS	getPoints() const
	{
		return MousePoints.getPoints();
	}

//アクセッサ
	//親ウィンドウ
	void setParentWnidow(HWND inParentWnidow)
	{
		ASSERT(ParentWnidow == NULL);
		ASSERT(inParentWnidow != NULL);
		ParentWnidow = inParentWnidow;
	}


private:
	//前回の軌跡から今回の軌跡までを描画.
	static void DrawLine(int sx,int sy, int ex,int ey , int size , COLORREF color) ;

	//キャプチャを開始してよかつか?
	bool IsCaptureOK(short x,short y);

private:
	//軌跡のサイズ
	int							LineSize;
	//軌跡の色
	COLORREF					LineColor;
	//キャプチャ開始までの遊び
	int							Allowance;
	//親ウィンドウ.
	HWND						ParentWnidow;

	POINT						LastPos;
	RMouseGestureMousePoints	MousePoints;

	MouseCaptureState			State;
};

#endif // !defined(AFX_RMOUSEGESTUREMOUSECAPTURE_H__6439826E_BFF7_499F_A906_7306E13A9021__INCLUDED_)
