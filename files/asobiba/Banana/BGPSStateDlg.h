// BGPSStateDlg.h: BGPSStateDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BGPSSTATEDLG_H__3F98E0F0_C1C2_4056_A5B3_0A2F6403F937__INCLUDED_)
#define AFX_BGPSSTATEDLG_H__3F98E0F0_C1C2_4056_A5B3_0A2F6403F937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RDialog.h"
#include "REdit.h"
#include "RGISGPS.h"
#include "RTimer.h"
#include "RWin32Exception.h"
#include "RState.h"
#include "RFont.h"
#include "RDIBBitmap.h"
#include "RMathAngle.h"

class BGPSStateDlg : public RDialog  
{
public:
	BGPSStateDlg();
	virtual ~BGPSStateDlg();

	void Create() throw(RWin32Exception);
	void OnGPSNotify(const LPOINT * xy ,int speed , int direction , SatellitesState satellitesState[SATELLITES_MAX]);
	void OnTailLog(const string & inLog);

	//表示する
	virtual void Show() ;
	//非表示にする
	virtual void Hide() ;

private:


	void OnCancel();

	void OnPaint();

	//ウィンドウプロシージャ.
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);

	//サイズ調整
	void Resize();

	POINT CalsSatellitesPossition(const SatellitesState * inSS) const;

private:
	RState	Pic;
	REdit	Log;
	REdit	Satellites;
	RFont	NewFont;
	RTimer	Timer;
	const RMathAngleFixed	* MathAngle;
	bool	ShowFlg;	//表示されているかどうかのフラグ

	RDIBBitmap	SatellitesGraphic;
	RDIBBitmap	SatellitesIcon;
};

#endif // !defined(AFX_BGPSSTATEDLG_H__3F98E0F0_C1C2_4056_A5B3_0A2F6403F937__INCLUDED_)
