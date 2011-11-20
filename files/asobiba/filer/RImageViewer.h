// RImageViewer.h: RImageViewer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_)
#define AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNormalWindow.h"
#include "RImage.h"
#include "RImageFilter.h"
#include "RSetupLoad.h"

class RImageViewer  : public RNormalWindow
{
public:
	RImageViewer();
	virtual ~RImageViewer();

	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//絵のロード
	bool Load(const string inFilename);

	//タイトルを取得する
	string getTitle() const ;
private:
	//ウィンドウプロシージャ(none static)
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	//水平スクロールバーはありますか?
	bool IsVSrcollBar() const;
	//垂直スクロールバーはありますか?
	bool IsHSrcollBar() const;

private:
	//設定の読み込み
	void ReloadSetting(string inReadName);
	//サイズ変更されました。
	void OnSize();
	//スクロールの計算
	void OnScroll(int inSw,int inFlg,int inValue);
	//描画
	void OnDraw(HDC inPaintDC) const;

	//キーが押されたときの処理
	void OnKeyDown(unsigned char inKeyCode ) const;
protected:
	//スクロールの仕切りなおし.
	void				Reset();
private:
	int					WindowWidth;
	int					WindowHeight;
	SCROLLINFO			ScrollInfoX;
	SCROLLINFO			ScrollInfoY;

	RImage				LoadImage;

	//設定ファイルからロードできます。
	RSetupLoad			SetupLoad;

};

#endif // !defined(AFX_RIMAGEVIEWER_H__893314CD_A86A_4E92_BE77_D2BC8F8972AC__INCLUDED_)
