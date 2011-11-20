// RProgressBar.h: RProgressBar クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_)
#define AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindow.h"


//real:len = full:x で x を求める.
#define CHANGE_SCALE(real,len,full)	( (real) * (full) / (len) )
//real:len = full:x で real を求める.
#define REV_CHANGE_SCALE(x,len,full)	( (x) * (len) / (full) )


#define ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGE	0x01	//マウスでクリックした位置に容量を合わせる

#define PB_ORIGNAL_EXTENDS_MOUSE_DOWN	(100+1)							//ORIGNAL_EXTENDS_STYLE_MOUSE_CHANGEの通知.

class RProgressBar : public RWindow  
{
public:
	RProgressBar();
	virtual ~RProgressBar();
public:
	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//最大値を決める.
	void setRange(int inStart , int inEnd)
	{
		Start = inStart;
		End = inEnd;
		SendMessage( this->getWindowHandle() , PBM_SETRANGE , 0 , MAKELPARAM(inStart, inEnd)  ) ;
	}
	//現在の位置を取得.
	int getPos()
	{
		return SendMessage( this->getWindowHandle() , PBM_GETPOS , 0 , 0 ) ;
	}
	//位置を設定.
	void setPos(int inNewPos)
	{
		SendMessage( this->getWindowHandle() , PBM_SETPOS , (WPARAM) inNewPos , 0 ) ;
	}
	//現在の位置からこんだけ移動させる.
	void DeltaPos(int inIncrement)
	{
		SendMessage( this->getWindowHandle() , PBM_DELTAPOS , (WPARAM) inIncrement  , 0 ) ;
	}
	//開始の色と終了の色を決める.
	void setGradationRange(COLORREF inStartColor,COLORREF inEndColor)
	{
		StartColor = inStartColor;
		EndColor = inEndColor;
	}
	//グラデーションをする単位を取得
	//大きくすればするほど　みえは落ちますが速度は速くなります。
	void setBockSize(unsigned int inBockSize)
	{
		ASSERT(inBockSize >= 0);
		BockSize = inBockSize;
	}
	//オリジナル拡張スタイルの設定
	void setOrignalExtendsStyle(DWORD inStyle)
	{
		OrignalExtends = inStyle;
	}
	//オリジナル拡張スタイルの取得
	DWORD getOrignalExtendsStyle() const
	{
		return OrignalExtends;
	}
private:
	//ウィンドウプロシージャ.(static)
	static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//すぐに static ではないやつに譲渡する。
		RProgressBar* p = (RProgressBar*)GetWindowLong(hwnd, GWL_USERDATA );
		if (p) return p->WndProc(hwnd,msg,wparam,lparam);

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	//ウィンドウプロシージャ(none static)
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void OnPaint(HDC hdc);
	//バーのサイズの取得.
	int getBarSize(int inFullSize);

	//指定されたカラーを使って inRC を塗りつぶす.
	void FillRectWithSolidBrush(HDC hdc , const RECT * inRC , COLORREF colorFill)
	{
		HBRUSH brush = ::CreateSolidBrush(colorFill);
		::FillRect(hdc, inRC , brush);
		::DeleteObject(brush);
	}

private:
	//サブクラス化.
	WNDPROC			OrignalWindowProc;
private:
	COLORREF			StartColor;
	COLORREF			EndColor;
	int					Start;
	int					End;

	unsigned int		BockSize;
	DWORD				OrignalExtends;

};

#endif // !defined(AFX_RPROGRESSBAR_H__8ECF1021_1968_4518_9CAE_3D2A47313CF3__INCLUDED_)
