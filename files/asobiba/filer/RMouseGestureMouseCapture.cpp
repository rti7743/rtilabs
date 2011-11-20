// RMouseGestureMouseCapture.cpp: RMouseGestureMouseCapture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMouseGestureMouseCapture.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMouseGestureMouseCapture::RMouseGestureMouseCapture()
{
	this->LineSize = 5;
	this->LineColor = RGB( 0 , 0 , 255);
	Allowance = 10;

	ParentWnidow = NULL;

	State = NO_CAPTURE;
}

RMouseGestureMouseCapture::~RMouseGestureMouseCapture()
{

}



//右ボタンが押された
bool RMouseGestureMouseCapture::OnRButtonDown(short x,short y)
{
	ASSERT(ParentWnidow != NULL);

	//前回の結果をクリア.
	MousePoints.Clear();
	//軌跡を追加.
	MousePoints.Insert( x, y);
	//前回の軌跡を登録.
	LastPos.x = x;
	LastPos.y = y;

	//キャプチャ準備中.
	State = PRE_CAPTURE;

	puts("mouse in");

	return true;
}

//右ボタンが離された
bool RMouseGestureMouseCapture::OnRButtonUp(short x,short y)
{
	ASSERT(ParentWnidow != NULL);

	//キャプチャ停止
	State = NO_CAPTURE;

	if(	GetCapture()==ParentWnidow)
	{
		puts("mouse out ... ok");

		// マウスを解放
		ReleaseCapture();

#if _DEBUG
		//ディスクトップに結果の線を書きます。
		POINT base = {0};
		::ClientToScreen(this->ParentWnidow , &base);
		MOUSE_GESTURE_MOUSE_POINTS points = MousePoints.getPoints();
		int size = points.size() - 1;
		for(int i = 0; i < size ; i++)
		{
			DrawLine(	points[i].x + base.x, points[i].y + base.y,
						points[i+1].x + base.x, points[i+1].y + base.y,LineSize , RGB(255,i*10,0) );
		}

#endif
		::InvalidateRect(0,NULL,TRUE);
		::InvalidateRect(this->ParentWnidow,NULL,TRUE);

//		return true;
		return false;
	}
	return false;
}

//マウスが移動した
bool RMouseGestureMouseCapture::OnMouseMove(short x,short y)
{
	ASSERT(this->ParentWnidow != NULL);

	//キャプチャ待機?
	if (State == PRE_CAPTURE)
	{
		//キャプチャーしていいのだったら、キャプチャします。
		if ( IsCaptureOK(x,y) )
		{
			// マウスをキャプチャ
			SetCapture(ParentWnidow);
			State = NOW_CAPTURE;
		}
	}

	if( GetCapture() == this->ParentWnidow )
	{
		puts("mouse move ... ok");

		//軌跡を追加.
		MousePoints.Insert(x,y);

		//スクリーン座標に変更.
		POINT base = {0};
		::ClientToScreen(this->ParentWnidow , &base);

		//前回の軌跡から今回の軌跡までを描画.
		DrawLine(LastPos.x + base.x, LastPos.y + base.y, x + base.x, y + base.y, LineSize , LineColor);

		LastPos.x = x;
		LastPos.y = y;
		return true;
	}
	return false;
}

//前回の軌跡から今回の軌跡までを描画. static 
void RMouseGestureMouseCapture::DrawLine(int sx,int sy, int ex,int ey , int size , COLORREF color)
{
	// デスクトップにするので HDC 取得
	HDC hScreenDc = GetDC( 0 );

	printf("(%d,%d)-(%d,%d)\n" , sx , sy , ex , ey);

	//線を書きます。。。あーめんどい.
	//ペン作って
	HPEN pen = ::CreatePen(PS_SOLID , size , color);
	HPEN oldpen = (HPEN) ::SelectObject(hScreenDc , pen);
	//位置を移動して
	::MoveToEx(hScreenDc , sx , sy , NULL);
	//ぴゅー
	::LineTo(hScreenDc,ex,ey);
	//以前のものに戻して
	::SelectObject(hScreenDc , oldpen);
	//おしまいっと。
	DeleteObject(pen);

	ReleaseDC(0,hScreenDc);
}


//キャプチャを開始してよかつか?
bool RMouseGestureMouseCapture::IsCaptureOK(short x,short y)
{
	return (abs(LastPos.x - x) >= Allowance ||  abs(LastPos.y - y) >= Allowance);
}
