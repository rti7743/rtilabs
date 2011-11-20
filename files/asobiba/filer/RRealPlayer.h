// RRealPlayer.h: RRealPlayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_)
#define AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "AtlActiveXBrige.h"
#include "RNotSupportException.h"
#include "RActiveXWindow.h"

#import "C:\WINNT\System32\rmoc3260.dll" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")

class RRealPlayer : 
 public RMediaViewPlayer ,	//インターフェース
 public RActiveXWindow  
{
public:
	RRealPlayer();
	virtual ~RRealPlayer();

	//実態の作成
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//ファイルを開く
	virtual void Open(const string inFilename )	throw() ;

	//再生する.
	virtual void Play() throw() ;

	//停止する.
	virtual void Stop() throw() ;

	//一時停止 / 解除
	virtual void Pause(bool inON)	throw() ;

	//ファイルクローズ
	virtual void Close()	throw() ;

	//ボリューム
	virtual int getVolum() const throw() ;

	virtual void setVolum(int inVolum) throw() ;

	//再生位置
	virtual int  getPos() const throw() ;

	virtual void setPos(int inPos) throw() ;

	//再生中?
	virtual bool IsPlay() const ;

	//一時停止中?
	virtual bool IsPause() const ;
	//サポートしてます?
	static bool IsSupport(const string inFilename);

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}

	//タイトルの取得
	virtual string getTitle() const;

private:
	//ウィンドウプロシージャ
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

private:
	CComPtr<RealAudioObjects::IRealAudio>	RealAudio;
};

#endif // !defined(AFX_RREALPLAYER_H__89FB5274_E992_411E_BA86_E803D94982E3__INCLUDED_)
