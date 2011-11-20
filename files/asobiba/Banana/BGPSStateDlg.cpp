// BGPSStateDlg.cpp: BGPSStateDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BGPSStateDlg.h"
#include "WindowsMain.h"
#include "resrc1.h"
#include "RResorceImageLoad.h"
#include "BConf.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BGPSStateDlg::BGPSStateDlg()
{

}

BGPSStateDlg::~BGPSStateDlg()
{

}

void BGPSStateDlg::Create() throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
	RDialog::Create(winMain->getInstance() ,
					winMain->getWindowHandle() , 
					"IDD_GPS_STATE_DIALOG");

	Satellites.MountWindow( this->GetDlgItem(IDC_SATELLITES_EDIT) );
	Pic.MountWindow( this->GetDlgItem(IDC_STEPIC_STATIC) );
	Log.MountWindow( this->GetDlgItem(IDC_LOG_EDIT) );

	NewFont.Create(9 , "ＭＳ ゴシック" , false , false , false , false ,SHIFTJIS_CHARSET);
	Satellites.SendMessage(WM_SETFONT ,(WPARAM) NewFont.getFont()  , 1);
	Log.SendMessage(WM_SETFONT ,(WPARAM) NewFont.getFont()  , 1);

	//数学関数を利用します.
	this->MathAngle = RMathAngleFixed::getMathAngleFixed();

	//衛星のアイコンの読み込み.
	RResorceImageLoad ril;
	ril.LoadFile(&SatellitesIcon, BConf::getConf()->getCurrentDirectory() + "SatellitesIcon.bmp" );

	Resize();

}

//サイズ調整
void BGPSStateDlg::Resize()
{
	//絵 | ステート
	//----ログ ----
	//という配置にする.

	RECT dlgRC;
	this->GetWindowRect(&dlgRC);

	RECT picRC;
	Pic.GetWindowRect(&picRC);

	RECT satellitesRC;
	Satellites.GetWindowRect(&satellitesRC);

	//ダイアログの幅と高さ.
	int w = dlgRC.right - dlgRC.left;
	int h = dlgRC.bottom - dlgRC.top;

	int halfw = ((w/2) % 2) ? (w/2) - 1 : w/2;
	int halfh = ((h/2) % 2) ? (h/2) - 1 : h/2;

	Pic.MoveWindow(0,0,halfw,halfw);
	Satellites.MoveWindow(halfw,0,halfw,halfw);
	Log.MoveWindow(0,halfw,w,h - halfw);

	SatellitesGraphic.Create( halfw , halfw , 16);

}

//表示する
void BGPSStateDlg::Show() 
{
	this->ShowFlg = true;

	RWindowBase::Show();
}

//非表示にする
void BGPSStateDlg::Hide() 
{
	this->ShowFlg = false;

	RWindowBase::Hide();
}


void BGPSStateDlg::OnCancel()
{
	this->Hide();
}


void BGPSStateDlg::OnTailLog( const string & inLog)
{
	if (!ShowFlg) return ;

	Log.setText(inLog);
}

void BGPSStateDlg::OnGPSNotify(const LPOINT * xy ,int speed , int direction , SatellitesState satellitesState[SATELLITES_MAX])
{
	if (!ShowFlg) return ;

	RGraphics::Clear(&SatellitesGraphic);

	string r = "";
	for(int i = 0 ; i < SATELLITES_MAX ; i++)
	{
		const SatellitesState * ss = &satellitesState[i];
		if (ss->Alive)
		{
			r += PrintFormat("%02d:%d/%d/%dDB\n" , i + 1, ss->Elevation , ss->Direction , ss->SNR);

			POINT pos = CalsSatellitesPossition(ss);

			RGraphics::Copy(&SatellitesIcon , 8 * i, 0 , 8 ,8 , &SatellitesGraphic , pos.x -(8/2), pos.y -(8/2)); 
		}
	}
	Satellites.setText(r);

	//画面を更新.
	InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
}

//衛星の位置を計算.
POINT BGPSStateDlg::CalsSatellitesPossition(const SatellitesState * inSS) const
{

	int minCenter = min( SatellitesGraphic.getWidth(),SatellitesGraphic.getHeight())  / 2 - 8;

	//仰角により、 半径の長さを決定.
	RAngle deg = RevAngle( ConvertAngle360To256(inSS->Elevation) );
	int r = FtoI( minCenter * MathAngle->Cos(deg) );

	//方位により、方向が決定する.
	POINT xy ;
	deg = RevAngle( ConvertAngle360To256(inSS->Direction) );
	xy.x = (FtoI(r*MathAngle->Cos(deg) - 0*MathAngle->Sin(deg )) + (SatellitesGraphic.getWidth()  / 2) ) ;
	xy.y = (FtoI(r*MathAngle->Sin(deg) + 0*MathAngle->Cos(deg) ) + (SatellitesGraphic.getHeight() / 2) ) ;


	return xy;
}

void BGPSStateDlg::OnPaint()
{
	RECT rc;
	this->getControlClientRect(this->GetDlgItem(IDC_STEPIC_STATIC) , &rc);

    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = this->getWindowHandle();

    hdc = BeginPaint( window , &paint);
	::BitBlt(hdc  , rc.left , rc.top , SatellitesGraphic.getWidth() , SatellitesGraphic.getHeight() ,
		SatellitesGraphic.getConstDC() , 0 , 0 , SRCCOPY);
    EndPaint(window, &paint);
}


//ウィンドウプロシージャ.
LRESULT BGPSStateDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//検索
				OnCancel();
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			}
		}
		break;
	case WM_PAINT:
		OnPaint();
		break;

	}
	return FALSE;
}

