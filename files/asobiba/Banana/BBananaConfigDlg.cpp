// BBananaConfigDlg.cpp: BBananaConfigDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BBananaConfigDlg.h"
#include "WindowsMain.h"
#include "BBanana.h"
#include "RDialogUtil.h"
#include "BBananaConfig.h"
#include "resrc1.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


BBananaConfigDlg::BBananaConfigDlg()
{

}

BBananaConfigDlg::~BBananaConfigDlg()
{

}


void BBananaConfigDlg::Create(BBanana	*	poolBanana) throw(RWin32Exception)
{
	RDialog::Create(WindowsMain::getMainSystem()->getInstance() ,
					WindowsMain::getMainSystem()->getWindowHandle() , 
					"IDD_CONFIG_DIALOG");

	BananaMap.MountWindow( this->GetDlgItem(IDC_BANANAMAP_EDIT) );
	BananaUserDir.MountWindow( this->GetDlgItem(IDC_BANANAUSERDIR_EDIT) );
	NoRotate.MountWindow( this->GetDlgItem(IDC_NOROTATE_COMBO) );

	BananaMap.setText( BBananaConfig::getInstance()->getMapFilename() );
	BananaUserDir.setText( BBananaConfig::getInstance()->getUserDataDirectory() );
	NoRotate.AddItem("禁止する");	//0
	NoRotate.AddItem("行う");		//1
	NoRotate.Select( BBananaConfig::getInstance()->getRotateFlg() == true ? 1 : 0);

	this->Banana = poolBanana;
}

void BBananaConfigDlg::OnCancel()
{
	this->Hide();
}

void BBananaConfigDlg::OnOK()
{
	try
	{
		BBananaConfig::getInstance()->setRotateFlg(NoRotate.getNowSelected() == 1 ? true : false);

		Banana->MapLoad( BananaMap.getText() , BananaUserDir.getText() );
	}
	catch(RException e)
	{
		Banana->MapDefualtLoad();

		RMessageBox::Message( e.getMessage() );
	}
	this->Hide();
	Banana->Update();
}

//ウィンドウプロシージャ.
LRESULT BBananaConfigDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
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
			case IDOK:	//開く
				OnOK();
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			case IDC_BANANAMAP_BUTTON:
				{
					string name = RDialogUtil::FileOpenDialog( this->getWindowHandle() , 
						"使用するマップを選択してください" , BananaMap.getText() , "kuma.map" , _T("mapファイル (*.map)\0*.map\0\0") );
					BananaMap.setText(name);
				}
				break;
			case IDC_BANANAUSERDIR_BUTTON:
				{
					string name = RDialogUtil::SelectPath( this->getWindowHandle() , 
						"ユーザデータを保存するパスを選択してください" , BananaMap.getText() );
					BananaUserDir.setText(name);
				}
				break;
			}
		}
		break;
	}
	return FALSE;
}

