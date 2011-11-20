// BGPSConfigDlg.cpp: BGPSConfigDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BGPSConfigDlg.h"
#include "WindowsMain.h"
#include "BBanana.h"
#include "resrc1.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BGPSConfigDlg::BGPSConfigDlg()
{

}

BGPSConfigDlg::~BGPSConfigDlg()
{

}

void BGPSConfigDlg::Create(BBanana	*	poolBanana) throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
	RDialog::Create(winMain->getInstance() ,
					winMain->getWindowHandle() , 
					"IDD_GPS_DIALOG");

	Port.MountWindow( this->GetDlgItem(IDC_PORT_COMBO) );
	char comportName[16];
	for(int i = 0;  i < 8 ; i++)
	{
		sprintf(comportName , "COM%d" , i + 1);
		Port.AddItem(comportName);
	}
	Port.Select(0);

	Speed.MountWindow( this->GetDlgItem(IDC_SPEED_COMBO) );
	Speed.AddItem("1200");
	Speed.AddItem("2400");
	Speed.AddItem("4800");
	Speed.AddItem("9600");
	Speed.AddItem("14400");
	Speed.AddItem("28800");
	Speed.AddItem("38400");
	Speed.Select(2);

	Type.MountWindow( this->GetDlgItem(IDC_TYPE_COMBO) );
	Type.AddItem("Tokyo");
//	Type.AddItem("WGS84");
	Type.Select(0);

	CheckTime.MountWindow( this->GetDlgItem(IDC_CHECKTIME_EDIT) );
	CheckTime.setText("5");

	LogType.MountWindow( this->GetDlgItem(IDC_LOGTYPE_COMBO) );
	LogType.AddItem("None");
	LogType.AddItem("NMEA0183");
//	LogType.AddItem("pot");
	LogType.Select(0);

	LogDir.MountWindow( this->GetDlgItem(IDC_LOGDIR_EDIT) );
	LogDir.setText("");

	TrackLog.MountWindow( this->GetDlgItem(IDC_TRACKLOG_COMBO) );
	TrackLog.AddItem("表示しない");
	TrackLog.AddItem("表示する");
	TrackLog.Select(0);

	ConnectButtom.MountWindow( this->GetDlgItem(IDC_CONNECT_BUTTON) );
	DisConnectButtom.MountWindow( this->GetDlgItem(IDC_DIS_CONNECT_BUTTON) );

	
	this->PoolBanana = poolBanana;
}

void BGPSConfigDlg::Show()
{
	if ( this->PoolBanana->IsGPSAlive() )
	{
		Port.EnableWindow(false);
		Speed.EnableWindow(false);
		Type.EnableWindow(false);
		CheckTime.EnableWindow(false);
		LogType.EnableWindow(false);
		LogDir.EnableWindow(false);
		TrackLog.EnableWindow(false);
		ConnectButtom.EnableWindow(false);
		DisConnectButtom.EnableWindow(true);
	}
	else
	{
		Port.EnableWindow(true);
		Speed.EnableWindow(true);
		Type.EnableWindow(true);
		CheckTime.EnableWindow(true);
		LogType.EnableWindow(true);
		LogDir.EnableWindow(true);
		TrackLog.EnableWindow(true);
		ConnectButtom.EnableWindow(true);
		DisConnectButtom.EnableWindow(false);
	}
	RWindowBase::Show();
}

void BGPSConfigDlg::OnCancel()
{
	this->Hide();
}

void BGPSConfigDlg::OnConnect()
{
	int port = Port.getNowSelected() + 1;
	int speed  = atoi( Speed.GetItem( Speed.getNowSelected() ).c_str() );
	int type = Type.getNowSelected();
	int checkTime = atoi( CheckTime.getText().c_str() );
	int logType = LogType.getNowSelected();
	string logDir = LogDir.getText();
	int trackLog = TrackLog.getNowSelected();
	

	this->PoolBanana->GPSOn(port , speed , type , checkTime , logType , logDir , trackLog);

	this->Hide();
}
void BGPSConfigDlg::OnDisConnect()
{
	this->PoolBanana->GPSOff();

	this->Hide();
}

//ウィンドウプロシージャ.
LRESULT BGPSConfigDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
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
			case IDC_CONNECT_BUTTON:	//接続
				OnConnect();
				break;
			case IDC_DIS_CONNECT_BUTTON://切断
				OnDisConnect();
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			}
		}
		break;
	}
	return FALSE;
}

