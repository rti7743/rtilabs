// WindowsMain.cpp: WindowsMain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "WindowsMain.h"
#include "DisplayFactory.h"
#include "FileModifyChacheLargeWin32.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"MPR.LIB")


#include "RTabBarWin32.h"
#include "RTaskManager.h"
#include "RNotify.h"
#include "AtlActiveXBrige.h"
#include "RThreadExecuteOnMainThread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WindowsMain::WindowsMain()
{
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
//例外			初期化失敗の場合は RDoNotInitaliseException が投げられる.
int WindowsMain::Run()
{
    STARTUPINFO StatUpInfo;
    HINSTANCE hPrevInstance;
    int nCmdShow;

	//スタートアップ処理.
    GetStartupInfo(&StatUpInfo);
    this->Instance = GetModuleHandle(0);
    hPrevInstance = 0;
    nCmdShow = (StatUpInfo.dwFlags & STARTF_USESHOWWINDOW)?
                StatUpInfo.wShowWindow:SW_SHOWNORMAL;

    WNDCLASS	wc;
    MSG			msg = {0};
	//メインウィンドクラスの作成.
	wc.lpszClassName = "SuperFiler";
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WndProcStatic;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = this->Instance;
    wc.hIcon         = LoadIcon(this->Instance, MAKEINTRESOURCE(IDI_APPLICATION));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;

    if(!RegisterClass(&wc))
	{
        throw RDoNotInitaliseException("ウィンドウクラスの作成に失敗しました");
    }
	//メインウィンドウの作成.
    this->WindowHandle = CreateWindow(wc.lpszClassName,
				            wc.lpszClassName,WS_OVERLAPPEDWINDOW,
						    CW_USEDEFAULT, CW_USEDEFAULT,
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL,this->Instance, NULL);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("メインウィンドウの作成に失敗しました");
	}

	//パネルを2つ張ります。
	//---------------------------------------
	//|  BasePanelLeft  |  BasePanelRight   |
	//|                 |                   |
	//|                 |                   |
	//|                 |                   |
	//|                 |                   |
	//|-------------------------------------|

	//左側のパネルの作成
	Panel[0].Create(true);
	//右側のパネルの作成
	Panel[1].Create(true);

	//スプリッターの作成.
	Spliter.setParentWnidow( this->getWindowHandle() );
	Spliter.setRubberBandSize(5);
	Spliter.setLeftWindowSize(0.50);


	//CommonControlの初期化
	InitCommonControls();
	//Oleの初期化
	::CoInitialize(NULL);
	//ATLの初期化
	_Module.Init(0, Instance );

	//サイズ調整.
	OnSize();
	//作成.
	Filer* filer = Filer::getFiler();
	filer->OnCreate();
	//ウィンドウ表示開始.
    ShowWindow(this->WindowHandle, nCmdShow);
    UpdateWindow(this->WindowHandle);

	//C++のクセ static の初期化.
	FileModifyChacheLargeWin32::getInstance();


	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	} 
	//ATLの終了
	_Module.Term();
	//COMの終了処理
	CoUninitialize();
	//プログラム終了.
	return msg.wParam;	
}


//サイズ変更
void WindowsMain::OnSize()
{
	RECT theRC;

	//Left
	Spliter.getWindowRect(true,&theRC);
	Panel[0].MoveWindow(theRC.left , theRC.top , theRC.right-theRC.left , theRC.bottom-theRC.top);

	//Right
	Spliter.getWindowRect(false,&theRC);
	Panel[1].MoveWindow(theRC.left , theRC.top , theRC.right-theRC.left , theRC.bottom-theRC.top);
}

//ウィンドウプロシージャ(none static)
LRESULT WindowsMain::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
	{
	case WM_CREATE:
		//分割ウィンドウ設定
		break;
	case WM_LBUTTONDOWN:
		if ( Spliter.OnLButtonDown(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_LBUTTONUP:
		if ( Spliter.OnLButtonUp(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_MOUSEMOVE:
		if ( Spliter.OnMouseMove(LOWORD(lparam) , HIWORD(lparam)) ) return 0;
		break;
	case WM_SIZE:
		OnSize();
		Filer::getFiler()->OnWindowResize();
		break;
	case WM_KEYDOWN:
		Filer::getFiler()->OnKeyDown( (unsigned char)wparam );
		break;
	case WM_CLOSE:
		Filer::getFiler()->OnDestory();
		DestroyWindow(hwnd);
		return 0;
	case WM_TASKEND:	//タスクが終了しました。
		RTaskManager::getInstance()->CallEndOfTask((RTask*)wparam,(Window*)lparam);
		return 0;
	case RTI_WM_INTERRUPT:	//メインスレッドに割り込む
		((RThreadExecuteOnMainThread*)(wparam))->_Start();
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

