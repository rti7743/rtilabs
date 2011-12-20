/*
やられ役のアプリケーションです。

BaseThreadInitThunk API をフックすることで、あらゆる CreateRemoetThread を防衛できます。

*/
#include <windows.h>
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //メインウィンドウ
const char * MYNAME = "ドMやられ役_Anti_RemoteThread";


DWORD WINAPI USOThread(LPVOID lpParameter)
{
	MessageBox( g_MainWnd , "CreateRemoteThreadを検出したので無力化しました。\r\n" , "CreateRemoteThread検出" , 0);
	return 0;
}


SEXYHOOK_BEGIN(void,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
			("kernel32.dll","BaseThreadInitThunk"),(LPVOID threadParama,LPVOID nazo ))
{
	//このレジスタの値は壊してはいけない
	void * _ecx = NULL;
	void * _edx = NULL;
	_asm
	{
		mov _ecx , ecx; //何かのフラグに利用しているらしい
		mov _edx , edx; //スレッド内で実行する関数のアドレス
	}
	//選別ルーチン

	//VirtualAlloc されたメモリは 32bitだと 0x70000000 以降に配置されるので、そのアドレスで判断している。
	//もっとちゃんとやる方法はあるはず。ただし、COMなどでDLLの中でthreadを作っている場合などすべての場合に対処するのは結構難しいかも。
	if (((unsigned long)_edx) >= 0x70000000)
	{
		//嘘のスレッド関数を渡してあげる。
		_edx = (void*)USOThread;
	}

	//オリジナルの関数 +5 はsexyhookが破壊している領域を飛ばすためアドレスをずらす.
	void* orignalFunction = (void*)(((unsigned long) sexyhookThis->getOrignalFunctionAddr() ) + 5);
	
	//なりジナルの BaseThreadInitThunk を呼び出します。
	_asm
	{
		push nazo;
		push threadParama;

		mov  eax,orignalFunction;    //ebp esp を下で書き換えるため ローカル変数にアクセスできなくなるので、eaxに保持.
		mov  ecx,_ecx;
		mov  edx,_edx;

		//BaseThreadInitThunk のコードの一部 プロローグ部分
		mov         edi,edi  
        push        ebp  
        mov         ebp,esp  

		//元のルーチンに飛ばす.
		jmp         eax 
//74AB338F  jne         @BaseThreadInitThunk@12+15h (74AB620Ah)    ここに飛ばす.
	}
}
SEXYHOOK_END_AS() g_BaseThreadInitThunk;




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
				g_BaseThreadInitThunk.Hook();

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

