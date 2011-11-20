// RMVMain.cpp: RMVMain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "MVMain.h"
#include "WindowsMain.h"
#include "RDialogUtil.h"
#include "resrc1.h"
#include "MVConfig.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVMain::MVMain()
{
	this->IsFullScreen = false;
}

MVMain::~MVMain()
{
}


void MVMain::Create() throw(RException)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();

	MVConfig::getInstance()->Load();

	RDialog::Create(winMain->getInstance() ,    NULL , 	"IDD_MVMAIN_DIALOG");

 	RECT rc;
	this->GetClientRect(&rc);

	MVImagePreConvert imageConvertParam;
	imageConvertParam.Create(
		(rc.right  - rc.left) ,	//画面の横
		(rc.bottom - rc.top) ,	//画面の縦
		MVConfig::getInstance()->getJustFit() , 
		MVConfig::getInstance()->getScrollType() );
	Control.ChangeOption( &imageConvertParam );

	//最後の状態を復元する?
	if ( MVConfig::getInstance()->getLastMemory() )
	{
		try
		{
			this->NowOpenFilename = MVConfig::getInstance()->getLastFilename();
			Control.Update( this->NowOpenFilename , MVConfig::getInstance()->getLastPage() );
			WindowsMain::Update();
		}
		catch(RException e)
		{
			//RMessageBox::Message("本" + name + "を開くことができませんでした\r\n" + e.getMessage() );
		}
	}

	SettingDialog.Create(this);

	this->Show();
}


void MVMain::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case VK_RIGHT:
	case VK_DOWN:
		Control.Scroll(false);
		break;
	case VK_LEFT:
	case VK_UP:
		Control.Scroll(true);
		break;
	}
	WindowsMain::Update();
}

void MVMain::OnDestory()
{
	//現在のページを保存して..
	MVConfig::getInstance()->SavePage(this->NowOpenFilename , this->Control.getNowPage() );
	//.conf に書き込み.
	MVConfig::getInstance()->Save();

	this->PostMessage(WM_DESTROY , 0 , 0 );
}

void MVMain::OnPaint()
{
    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = winMain->getWindowHandle();
    hdc = BeginPaint( window , &paint);
	Control.Draw(hdc);
    EndPaint(window, &paint);
}

void MVMain::OnSize()
{
	RECT rc;
	this->GetClientRect(&rc);
	int width  = (rc.right  ) ;
	int height = (rc.bottom ) ;

	MVImagePreConvert imageConvertParam;
	imageConvertParam.Create(
		width ,	//画面の横
		height ,	//画面の縦
		MVConfig::getInstance()->getJustFit() , 
		MVConfig::getInstance()->getScrollType() );

	Control.ChangeOption( &imageConvertParam );
	Control.GotoPage(Control.getNowPage() );
	WindowsMain::Update();
}


void MVMain::OnClick()
{
#ifdef _WIN32_WCE
	RECT rc;
	this->GetWindowRect(&rc);

	RECT rcTaskbar = {0};
	int taskbarSize = 0;
	HWND windowTask = FindWindow(_T("HHTaskbar"),NULL);
	if (windowTask)
	{
		::GetWindowRect( windowTask , &rcTaskbar);

		taskbarSize = rcTaskbar.bottom - rcTaskbar.top;
	}

	if (this->IsFullScreen)
	{
		rc.top +=  taskbarSize;
		rc.bottom -= (2*taskbarSize) ;

		//元に戻す
		::SHFullScreen(this->getWindowHandle() , SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
		::ShowWindow(this->RpCommandBar, SW_SHOW);
		::MoveWindow(this->getWindowHandle() , rc.left, 	rc.top, 	rc.right, 	rc.bottom, 	TRUE);	

		this->IsFullScreen = false;
	}
	else
	{
		rc.top -= taskbarSize ;
		rc.bottom += (2*taskbarSize) ;

		//フルスクーリンに
		::SHFullScreen(this->getWindowHandle() , SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON);
		::ShowWindow(this->RpCommandBar, SW_HIDE);
		::MoveWindow(this->getWindowHandle() , rc.left, 	rc.top, 	rc.right, 	rc.bottom, 	TRUE);	

		this->IsFullScreen = true;
	}
	//リサイズされたウィンドウに合うように画像を読み直す.
	this->GetClientRect(&rc);
	int width  = (rc.right  ) ;
	int height = (rc.bottom ) ;

	MVImagePreConvert imageConvertParam;
	imageConvertParam.Create(
		width ,	//画面の横
		height ,	//画面の縦
		MVConfig::getInstance()->getJustFit() , 
		MVConfig::getInstance()->getScrollType() );

	Control.ChangeOption( &imageConvertParam );
	Control.GotoPage(Control.getNowPage() );
#endif
}
void MVMain::OnCommand(int inID)
{
	switch(inID)
	{
	case ID_MENUITEM_OPEN:
		{
			string name = RDialogUtil::FileOpenDialog( this->getWindowHandle() , 
				"本を開く" , "" , "" , _T("使用可能なファイル\0*.zip\0\0") );

			if ( !name.empty() )
			{
				try
				{
					Control.Update( name , 0 );
					this->NowOpenFilename = name;
					WindowsMain::Update();
				}
				catch(RException e)
				{
					RMessageBox::Message("本" + name + "を開くことができませんでした\r\n" + e.getMessage() );
				}
			}
		}
		break;
	case ID_MENUITEM_CLOSE:
		::SendMessage(this->getWindowHandle() ,WM_CLOSE, 0 , 0 );
		break;
	case ID_MENUITEM_CONFIG:
		SettingDialog.Show();
		break;
	}
}


//ウィンドウプロシージャ.
LRESULT MVMain::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		{
			WindowsMain* winMain = WindowsMain::getMainSystem();


			SHMENUBARINFO	cbi = { 0 };
			cbi.cbSize		= sizeof( SHMENUBARINFO );
			cbi.hwndParent	= ioHwnd;	// ダイアログのウインドウハンドル
			cbi.nToolBarId	= IDR_MENU1;	// メニューリソースの ID
			cbi.hInstRes	= winMain->getInstance();
			cbi.dwFlags		= SHCMBF_HMENU;
			BOOL ret;
			if ( ret = SHCreateMenuBar( &cbi ) )
			{
				RpCommandBar = cbi.hwndMB;
			}
			else
			{
				RpCommandBar = NULL;
			}
			SHINITDLGINFO shidi = {0};
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = ioHwnd;
			SHInitDialog(&shidi);
            memset (&Sai, 0, sizeof (Sai));
            Sai.cbSize = sizeof (Sai);
		}
#endif
		return TRUE;
#ifdef _WIN32_WCE
	case WM_ACTIVATE:
	// Notify shell of our activate message
		SHHandleWMActivate(ioHwnd, inWParam, inLParam, &Sai, FALSE);
		break;
	case WM_SETTINGCHANGE:
		SHHandleWMSettingChange(ioHwnd, inWParam, inLParam, &Sai);
		break;
#endif
	case WM_SIZE:
		OnSize();
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_KEYDOWN:
		OnKeyDown( (unsigned char)inWParam );
		break;
	case WM_LBUTTONDOWN:
		OnClick();
 		break;
	case WM_CLOSE:
		OnDestory();
		return TRUE;
   case WM_DESTROY:
#ifdef _WIN32_WCE
		//メニュバーを削除
		if (RpCommandBar)		CommandBar_Destroy(RpCommandBar);
#endif
		PostQuitMessage(0);
        return 0;
	case WM_COMMAND:
		OnCommand( LOWORD(inWParam) );
		break;
	}
	return FALSE;
}

void MVMain::OnSettingChange()
{
	//ジャストフィットが変更されているかもしれないので、ページを読み込みなおす.
	OnSize();

}

bool MVMain::IsDialogMessage(MSG * inMessage)
{
	return 
		(SettingDialog.getWindowHandle() != 0 &&	::IsDialogMessage( SettingDialog.getWindowHandle() , inMessage) )  ;

}
