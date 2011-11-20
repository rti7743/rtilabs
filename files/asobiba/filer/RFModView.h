// RFModView.h: RFModView クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_)
#define AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "FModPlay.h"
#include "RMediaViewPlayer.h"
#include "RTimer.h"



class RFModView  :
 public RMediaViewPlayer ,	//インターフェース
 public FModCallable, 		//再生中のコールバック.
 public RNormalWindow		//通常ウィンドウ
{
public:
	RFModView();
	virtual ~RFModView();

	//実態の作成
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//ファイルを開く
	void Open(const string inFilename )	throw()
	{
		Player.Open(inFilename );
	}

	//再生する.
	void Play() throw()
	{
		Player.Play(this);	//自分自身にコールバックさせる.
	}

	//停止する.
	void Stop() throw()
	{
		Player.Stop();
	}

	//一時停止 / 解除
	void Pause(bool inON)	throw()
	{
		Player.Pause(inON);
	}

	//ファイルクローズ
	void Close()	throw()
	{
		Player.Close();
	}

	//ボリューム
	int getVolum() const throw()
	{
		return CHANGE_SCALE(Player.getVolum() , 255 , 255);
	}

	void setVolum(int inVolum) throw()
	{
		ASSERT(inVolum <= 255);
		Player.setVolum( REV_CHANGE_SCALE(inVolum,255,255) );
	}

	//再生位置
	int  getPos() const throw()
	{
		int a = Player.getPos() ;
		int b = Player.getLength();
		return CHANGE_SCALE( Player.getPos() , Player.getLength() , 255);
	}

	void setPos(int inPos) throw()
	{
		ASSERT(inPos <= 255);
		Player.setPos( REV_CHANGE_SCALE(inPos , Player.getLength() , 255) );
	}

	//再生中?
	bool IsPlay() const
	{
		return Player.IsPlay();
	}

	//一時停止中?
	bool IsPause() const
	{
		return Player.IsPause();
	}

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}

	//サポートしてます?
	static bool IsSupport(const string inFilename);

	//タイトルの取得
	virtual string getTitle() const ;
protected:
	//コールバック
	virtual signed char FModCallBack(FSOUND_STREAM *stream, void *buff, int len) 
	{
		SeenProgressBar->setPos( getPos() );
		return 1;
	}

private:
	FModPlay	Player;
	RProgressBar*	SeenProgressBar;

	RTimer			Timer;
};

#endif // !defined(AFX_RFMODVIEW_H__1547552F_3732_4EB3_A60F_AAF0836BD164__INCLUDED_)
