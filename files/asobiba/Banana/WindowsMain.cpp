// WindowsMain.cpp: WindowsMain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WindowsMain.h"
#include "resrc1.h"
#include "RDoNotInitaliseException.h"

#ifndef _WIN32_WCE
	#pragma comment(lib,"comctl32.lib")
	#pragma comment(lib,"winmm.lib")
	//#pragma comment(lib,"MPR.LIB")
#endif


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WindowsMain::WindowsMain()
{
	Instance = 0;
}

WindowsMain::~WindowsMain()
{

}

WindowsMain* WindowsMain::getMainSystem()
{
	static WindowsMain	main;
	return &main;
}

//適当にIDを振り出す(w
int WindowsMain::getID()
{
	static id = 1000;
	return id++;
}

//実行.
//戻り値		プログラムのリターン値.
//例外			初期化失敗の場合は RException が投げられる.
int WindowsMain::Run() throw(RException)
{
//    STARTUPINFO StatUpInfo;
    HINSTANCE hPrevInstance;
//    int nCmdShow;

	//スタートアップ処理.
//    GetStartupInfo(&StatUpInfo);
    this->Instance = GetModuleHandle(0);
    hPrevInstance = 0;
//    nCmdShow = (StatUpInfo.dwFlags & STARTF_USESHOWWINDOW)?
//                StatUpInfo.wShowWindow:SW_SHOWNORMAL;

    MSG			msg = {0};
	//CommonControlの初期化
	InitCommonControls();
	//Oleの初期化
//	::CoInitialize(NULL);
	//ATLの初期化
//	_Module.Init(0, Instance );

	//作成.
	BananaDlg.Create();

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if ( !BananaDlg.IsDialogMessage(&msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	} 
	//ATLの終了
//	_Module.Term();
	//COMの終了処理
//	CoUninitialize();
	//プログラム終了.
	return msg.wParam;	
}


//ウィンドウプロシージャ(none static)
LRESULT WindowsMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(hwnd, msg, wparam, lparam);
}


//画面の更新.
void WindowsMain::Update()
{
	InvalidateRect( getMainSystem()->getWindowHandle() , NULL , TRUE);
	
}

