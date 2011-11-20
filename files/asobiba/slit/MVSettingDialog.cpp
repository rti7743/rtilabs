// MVSettingDialog.cpp: MVSettingDialog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVSettingDialog.h"
#include "MVConfig.h"
#include "WindowsMain.h"
#include "resrc1.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVSettingDialog::MVSettingDialog()
{

}

MVSettingDialog::~MVSettingDialog()
{

}

void MVSettingDialog::Create(MVMain	*	poolMVMain) throw(RWin32Exception)
{
	RDialog::Create(WindowsMain::getMainSystem()->getInstance() ,
					WindowsMain::getMainSystem()->getWindowHandle() , 
					"IDD_CONFIG_DIALOG");

	LastMemory.MountWindow( this->GetDlgItem(IDC_LASTMEMORY_COMBO) );
	JustFit.MountWindow( this->GetDlgItem(IDC_JUSTFIT_COMBO) );
	ScrollCount.MountWindow( this->GetDlgItem(IDC_SCROLLCOUNT_COMBO) );
	ScrollType.MountWindow( this->GetDlgItem(IDC_SCROLLTYPE_COMBO) );
	

	LastMemory.AddItem("する");			//0
	LastMemory.AddItem("しない");		//1

	JustFit.AddItem("する");			//0
	JustFit.AddItem("しない");			//1

	ScrollCount.AddItem("1");			//0
	ScrollCount.AddItem("2");			//1
	ScrollCount.AddItem("3");			//2
	ScrollCount.AddItem("4");			//3
	ScrollCount.AddItem("5");			//4
	ScrollCount.AddItem("6");			//5

	ScrollType.AddItem("横");			//0
	ScrollType.AddItem("縦");			//1

	LastMemory.Select( MVConfig::getInstance()->getLastMemory() == true ? 1 : 0);
	JustFit.Select( MVConfig::getInstance()->getJustFit() == true ? 1 : 0);
	ScrollCount.Select( MVConfig::getInstance()->getScrollCount() - 1);
	ScrollType.Select( MVConfig::getInstance()->getScrollType() );

	this->PoolMVMain = poolMVMain;
}

void MVSettingDialog::OnCancel()
{
	this->Hide();
}

void MVSettingDialog::OnOK()
{
	try
	{
		MVConfig::getInstance()->setFlg(LastMemory.getNowSelected() == 1 ? true : false ,
										JustFit.getNowSelected() == 1 ? true : false,
										ScrollCount.getNowSelected() + 1,
										(MVConfig::SCROLL_TYPE) ScrollType.getNowSelected() );

		PoolMVMain->OnSettingChange();
	}
	catch(RException e)
	{
		RMessageBox::Message( e.getMessage() );
	}
	this->Hide();
	WindowsMain::Update();
}

//ウィンドウプロシージャ.
LRESULT MVSettingDialog::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
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
			}
		}
		break;
	}
	return FALSE;
}

