// RSpliter.cpp: RSpliter クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RSpliter.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RSpliter::RSpliter()
{
	this->RubberBandSize = 10;
	this->OldWidth = 0;
}

RSpliter::~RSpliter()
{

}


//ひだりボタンが押された
bool RSpliter::OnLButtonDown(int x,int y)
{
	ASSERT(ParentWnidow != NULL);

	//左ウィンドウの幅の取得
	RECT leftRect;
	this->getWindowRect(true , &leftRect);
	// MouseDiffを設定
	MouseDiff=x-(leftRect.right-leftRect.left);

	// マウスをキャプチャ
	SetCapture(ParentWnidow);
	// スプリッターを描画
	this->DrawSpliter(x-MouseDiff);
	OldWidth = x-MouseDiff;

	return false;
}

//ひだりボタンが離された
bool RSpliter::OnLButtonUp(int x,int y)
{
	ASSERT(ParentWnidow != NULL);
	if(GetCapture()==ParentWnidow)
	{
		// スプリッターを消去
		this->DrawSpliter(OldWidth);
		// 左側ウィンドウの幅を設定
		this->setLeftWindowSize(x-MouseDiff);
		// マウスを解放
		ReleaseCapture();

		// メインウィンドウのクライアントサイズを取得
		RECT mainWindowRect;
		GetClientRect(this->ParentWnidow,&mainWindowRect);

		//ちょっと汚い方法だけど、メインウィンドウに WM_SIZE を送信します(w
		::PostMessage(this->ParentWnidow , WM_SIZE , SIZE_RESTORED , MAKELPARAM(mainWindowRect.right,mainWindowRect.bottom) );

		return true;
	}
	return false;
}

//マウスが移動した
bool RSpliter::OnMouseMove(int x,int y)
{
	ASSERT(this->ParentWnidow != NULL);
	if(GetCapture() == this->ParentWnidow)
	{
		// スプリッターを消去
		this->DrawSpliter(OldWidth);
		// スプリッターを描画
		this->DrawSpliter(x-MouseDiff);
		// スプリッターを描画した位置を記録
		OldWidth = x-MouseDiff;
		return true;
	}
	return false;
}

// 左のウィンドウのサイズを正規化して返す
int RSpliter::RegularWidth(int inWidth) const
{
	//マイナス突っ込むのはまずいっしょ.
	if(inWidth < 1)		return 1;

	// メインウィンドウのクライアントサイズを取得
	RECT MainWindowRect;
	GetClientRect(this->ParentWnidow,&MainWindowRect);

	//メインウィンドウよりでかいのはまずいっしょ.
	if(inWidth + this->RubberBandSize+1 > MainWindowRect.right)
		return MainWindowRect.right - this->RubberBandSize-1;
	return inWidth;
}


//スプリッターを描画.
void RSpliter::DrawSpliter(int inWidth) const
{
	ASSERT(this->ParentWnidow != NULL);

	int width=this->RegularWidth(inWidth);
	// スプリッターを描画
	HDC hScreenDc = GetDC( 0 );

	//メインウィンドウのRECT
	RECT mainRect;
	::GetWindowRect(this->ParentWnidow , &mainRect);
	//左ウィンドウの幅の取得
	RECT leftRect;
	this->getWindowRect(true , &leftRect);

	PatBlt(hScreenDc ,
		mainRect.left + leftRect.left + width ,
		mainRect.top + leftRect.top,
		this->RubberBandSize ,
		leftRect.bottom - leftRect.top,
		DSTINVERT);
	ReleaseDC( 0 ,hScreenDc);
}



//ラバーバンドのサイズの設定
void RSpliter::setRubberBandSize(int inRubberBandSize)
{
	this->RubberBandSize = inRubberBandSize;
}

//ラバーランドの左/右のサイズの取得
void RSpliter::getWindowRect(bool IsLeft,RECT* outRect) const
{
	// メインウィンドウのクライアントサイズを取得
	RECT mainWindowRect;
	GetClientRect(this->ParentWnidow,&mainWindowRect);

	//ラバーバンドの幅を　左右のウィンドウでもつので / 2 です。
	int corrodeSize = this->RubberBandSize / 2;

	//左右の計算
	RECT retRect;
	if (IsLeft)
	{
		retRect.left = 0;
		retRect.top = 0;
		retRect.right = this->LeftWindowSize - corrodeSize;
		retRect.bottom = mainWindowRect.bottom;
		*outRect = retRect;
	}
	else
	{
		retRect.left = this->LeftWindowSize + corrodeSize;
		retRect.top = 0;
		retRect.right = mainWindowRect.right - corrodeSize;
		retRect.bottom = mainWindowRect.bottom;

		*outRect = retRect;
	}
}


//親ウィンドウ
void RSpliter::setParentWnidow(HWND inParentWnidow)
{
	this->ParentWnidow = inParentWnidow;
}

//左ウィンドウの幅の設定
void RSpliter::setLeftWindowSize(int inNewLeftWidth)
{
	this->LeftWindowSize = this->RegularWidth(inNewLeftWidth);
}

//左ウィンドウの幅の設定
void RSpliter::setLeftWindowSize(double inNewLeftParsent)
{
	// メインウィンドウのクライアントサイズを取得
	RECT mainWindowRect;
	GetClientRect(this->ParentWnidow,&mainWindowRect);

	int newWidth = (int) (mainWindowRect.right * inNewLeftParsent);
	this->setLeftWindowSize(newWidth);
}
