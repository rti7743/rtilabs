// RBanana.h: RBanana クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_)
#define AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RDrawObject.h"
#include "RDIBBitmap.h"
#include "RZlib.h"
#include "RGISSmall.h"
#include "RGISSmallIntelligentCache.h"
#include "BFindEkiDlg.h"
#include "BFindAdressDlg.h"
#include "BUserDataEditDlg.h"
#include "RDialog.h"
#include "RGISGPS.h"
#include "RGISSmallMaster.h"
#include "RGISSlmINdex.h"
#include "BGPSStateDlg.h"
#include "BConf.h"
#include "RTimer.h"
#include "BRotateMode.h"
#include "BClickMode.h"
#include "BBananaConfigDlg.h"
#include "BGPSConfigDlg.h"
#include "BBananaConfig.h"

#ifndef VIEWONLY
	#include "RGISSlp.h"
	#include "GISType.h"
	#include "RGISDouro.h"
	#include "RGIS.h"
	#include "RGISTetudo.h"
	#include "RGISKawa.h"
	#include "RGISSuiikiKai.h"
	#include "RGISEki.h"
	#include "RGISGyousei.h"
	#include "RGISChimei.h"
	#include "RFileUtil.h"
	#include "RGISIntelligentCache.h"
	#include "RGIS2500Head.h"
	#include "RGISSlm.h"
	#include "Sec2Utm.h"
	#include "Bl2Sec.h"
	#include "RGISCVCut.h"
	#include "RGISNameDatabaseDisk.h"
	#include "RGISNode.h"
	#include "RGISAllTest.h"
	#include "BMakeDlg.h"
#endif


class BBanana  : public RDialog
{
public:
	BBanana();
	virtual ~BBanana();

	void Create()throw(RException);

	//ウィンドウプロシージャ.
	virtual LRESULT BBanana::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);

	//マップの再読み込み.
	void MapLoad(const string & mapName , const string & mapUserDirName) throw(RException);
	//ディフォルトのマップを読み込む.
	void MapDefualtLoad();

	//この場所を画面の中心に持ってきます。
	void LookAt(const LPOINT & inPoint);

	//ユーザーデータの編集.
	void UserDataModify(const LPOINT * inPoint ,
		const int * inDeleteNumber , int inDeleteCount ,
		const char * inData , int inSize,int inCount ) throw(RException);

	RGISSmallMaster* getGisMaster() const
	{
		return GisMaster;
	}

	//画面を更新する.
	void Update();

	void GPSOn(int port ,int speed ,int type ,int checkTime ,int logType ,const string & logDir , int trackLog);
	void GPSOff();
	bool IsGPSAlive() const;

	bool IsDialogMessage(MSG * inMessage);

private:
	void OnGPSNotify();
	void OnKeyDown(unsigned char inKey);
	void OnPaint();
	void OnDestory();
	void OnCommand(int inID);
	//スクリーンをタップされた場合
	void OnLBottonDown(int inX,int inY);
	void BBanana::OnLBottonMove(int inX,int inY);
	void BBanana::OnLBottonUp(int inX,int inY);
	//画面の作成
	void BBanana::InitScreen() throw(RException);

private:
	//ダイアログをウィンドウの下にくっつけます。
	void SetDialogPos(RDialog * ioDialog);

private:
	//キャンバス
	RDIBBitmap	Dib;
	RDIBBitmap	Caption;

	//GISのボス
	RGISSmallMaster* GisMaster;

	//ユーザが地図に線を書き込めるようにする.
	RGISSmallPenFile	PenFile;
	//トラックログ
	RGISSmallPenFile	TrackLog;

	//ダイアログ
	BFindEkiDlg	FindEkiDlg;
	BFindAdressDlg FindAddressDlg;
	BGPSStateDlg GPSStateDlg;
	BUserDataEditDlg UserDataEditDlg;
	BBananaConfigDlg BananaConfigDlg;
	BGPSConfigDlg	GPSConfigDlg;

	//マウスジェスチャー操作
	BRotateMode		RotateMode;
	BClickMode		ClickMode;

	//GPS制御スレッド
	RGISGPS			GPS;
	bool			GPSTrackLog;
	RTimer			GPSTimer;

	unsigned char	PenColor;

	//共有して使用するイメージ.
	RGISSmallShareImage	ShareImage;
#ifdef _WIN32_WCE
	HWND		RpCommandBar;
	SHACTIVATEINFO Sai;
#endif
};

#endif // !defined(AFX_RBANANA_H__E5049A08_284B_4C67_9193_AF06F591E9D0__INCLUDED_)
