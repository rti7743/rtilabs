/*
やられ役のアプリケーションです。

*/
#include <windows.h>
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //メインウィンドウ
const char * MYNAME = "ドMやられ役_Anti_DLL_Injection_LoadLibraryA";


//LoadLibraryAを保護する.
SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
			("kernel32.dll","LoadLibraryA"),( const char * lpLibFileName ))
{

	//ドSくんのDLLだけロードさせない.
	if ( strstr(lpLibFileName , "ドS" ))
	{
		//ドSって入っているから、ドSくんのdllだろうから、ロード拒否.
		MessageBox( g_MainWnd , "ドSくんからDLLを挿し込まれそうになったので防御しました" , "バリアー" , 0);

		return 1;
	}
	//それ以外はロードする.
	//全部のDLLを拒否してはいけない。
	//gdiplus や winsock など遅延読み込みされるAPIがあるため、そいつらまで殺してしまう。

	//一時的にフックを解除
	SEXYHOOK_UNHOOK();
	//元の関数を呼び出す
	return CallOrignalFunction(lpLibFileName);
}
SEXYHOOK_END_AS() g_LoadLibraryAHook;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
                   LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS myProg;

	g_Instance = hInstance;

    if (!hPreInst) {
        myProg.style            =CS_HREDRAW | CS_VREDRAW;
        myProg.lpfnWndProc      =WndProc;
        myProg.cbClsExtra       =0;
        myProg.cbWndExtra       =0;
        myProg.hInstance        =hInstance;
        myProg.hIcon            =NULL;
        myProg.hCursor          =LoadCursor(NULL, IDC_ARROW);
        myProg.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
        myProg.lpszMenuName     =NULL;
        myProg.lpszClassName    =MYNAME;
        if (!RegisterClass(&myProg))
            return FALSE;
    }
    g_MainWnd = CreateWindow(MYNAME
        ,MYNAME
        ,WS_OVERLAPPEDWINDOW
        ,CW_USEDEFAULT
        ,CW_USEDEFAULT
        ,400
        ,400
        ,NULL
        ,NULL
        ,hInstance
        ,NULL);
    ShowWindow(g_MainWnd, nCmdShow);
    UpdateWindow(g_MainWnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//各種、ウィンドウ制御用 面倒のでstaticですが何か？
	const int ID_NANOHA_STATUS = 1000;
	const int ID_BARRIER = 1001;
	static HWND g_NanohaStatusWindow = NULL;	 //攻撃判定ウィンドウ
	static HWND g_BarrierButtonWindow = NULL;    //バリアボタン
    switch (msg) {
		case WM_CREATE:
			g_NanohaStatusWindow = CreateWindow("STATIC","なのフェイの薄い本ください",
                 WS_CHILD|WS_VISIBLE|SS_LEFT,
                 20,20,300,30,
                 hWnd,
                 (HMENU)ID_NANOHA_STATUS, 
				 g_Instance,
                 NULL);
            g_BarrierButtonWindow = CreateWindow(
                "BUTTON", "バリアー!!", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 100, 300, 60, hWnd, (HMENU)ID_BARRIER, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_BARRIER:
				//APIフックによるバリアを展開する.
				g_LoadLibraryAHook.Hook();

				SetWindowText(g_BarrierButtonWindow , "バリアーを展開中");
				EnableWindow(g_BarrierButtonWindow , FALSE);
				break;
			}
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}

