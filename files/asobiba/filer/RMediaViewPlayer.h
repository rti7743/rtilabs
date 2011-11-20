// RMediaViewPlayer.h: RMediaViewPlayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_)
#define AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNormalWindow.h"
#include "RProgressBar.h"

//interface クラス.
class RMediaViewPlayer		//視覚エフェクトを表示するウィンドウ.
{
public:
	RMediaViewPlayer()
	{
	}
	virtual ~RMediaViewPlayer()
	{
	}

	//実態の作成
	virtual void Create(HINSTANCE inINstance,HWND isParentWindow) throw() = 0;

	//ファイルを開く
	virtual void Open(const string inFilename )	throw() = 0;

	//再生する.
	virtual void Play() throw() = 0;

	//停止する.
	virtual void Stop() throw() = 0;

	//一時停止 / 解除
	virtual void Pause(bool inON)	throw() = 0;

	//ファイルクローズ
	virtual void Close()	throw() = 0;

	//ボリューム
	virtual int getVolum() const throw() = 0;

	virtual void setVolum(int inVolum) throw() = 0;

	//再生位置
	virtual int  getPos() const throw() = 0;

	virtual void setPos(int inPos) throw() = 0;

	//再生中?
	virtual bool IsPlay() const = 0;

	//一時停止中?
	virtual bool IsPause() const = 0;

	virtual const RWindow*	getWindow() const = 0;

	//タイトルの取得
	virtual string getTitle() const = 0;

};

#endif // !defined(AFX_RMEDIAVIEWPLAYER_H__DF926F90_5477_4CB1_AAB6_181414FF346F__INCLUDED_)
