/*
やられ役のアプリケーションです。


LdrLoadDll と BaseThreadInitThunk API をフックすることで、あらゆるフックから防衛できます。
(カーネルレイヤーからのフックを除く。)

BaseThreadInitThunkから防衛するときに相手のプロセスに対して、Sleep(3日) を発行することで、
相手のプロセスを3日冬眠状態にする仕返しをします。

*/
#include <windows.h>
#include <Ntsecapi.h>	// for PUNICODE_STRING
#include <tchar.h>
#include <tlhelp32.h>	
#include "sexyhook.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //メインウィンドウ
const char * MYNAME = "ドMやられ役_Anti_ALLHook無敵のバリア";


DWORD WINAPI USOThread(LPVOID lpParameter)
{
	MessageBox(g_MainWnd , "ドSくんのプロセスが攻撃を仕掛けてきたのでラリホります。" , "おやすみー",0);
	::Sleep(1000 * 60 * 60 * 24 * 3);
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

	//VirtualAlloc されたメモリの判別が難しい。
	//とりあえず、 VirtualAllocする人は、 PAGE_EXECUTE_READWRITE していたりしたら、 コード領域なのにWRITEができるーっていうことで落とすことにした。
	//相手が対策してきたら無意味なので、、、根本的な対処を考えないといけない。
	if (!IsBadWritePtr(_edx , 1))
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

		//この関数をなかった事にしたいので、 スタックフレームを復元する.
		mov esp,ebp
		//ebpの復元
		pop ebp

		//BaseThreadInitThunk のプロローグ
		mov         edi,edi  
        push        ebp  
        mov         ebp,esp  

		//元のルーチンに飛ばす.
		jmp         eax 
//74AB338F  jne         @BaseThreadInitThunk@12+15h (74AB620Ah)    ここに飛ばす.
	}
}
SEXYHOOK_END_AS() g_BaseThreadInitThunk;


//DLLと同じ場所にあるドSなexeを強制終了させます。
void killExecute(const wchar_t * dllpath )
{//結構手抜き
	wchar_t path[MAX_PATH];
	wcscpy( path , dllpath);

	wchar_t* p = wcsrchr(path, L'\\');
	if (!p)
	{
		return ;
	}
	*p = 0;
	wcscat( path , L"\\*.exe");

	bool foundS = false;
	WIN32_FIND_DATAW data;
	HANDLE findHandle = FindFirstFileW( path , &data);
	if (findHandle == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	do
	{
		if ( wcsstr(data.cFileName ,L"ドS" ) != NULL )
		{
			//ドSぢゃないと何もしない.
			foundS = true;
			break;
		}
	}
	while( FindNextFileW(findHandle ,  &data) );
	FindClose(findHandle);

	//ドSが見つからなければ何もしない.
	if (!foundS)
	{
		return ;
	}

	//プロセスを探す.
	DWORD dwProcessId = NULL;

	HANDLE hSnap;
	if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);
	BOOL bResult = Process32FirstW(hSnap, &pe);
	while(bResult)
	{
		if(wcsstr(pe.szExeFile, data.cFileName ) != NULL)
		{
			dwProcessId = pe.th32ProcessID;
			break;
		}
		bResult = Process32NextW(hSnap, &pe);
	}
	CloseHandle(hSnap);
	if (dwProcessId == NULL)
	{
		return ;
	}

	//見つけたので殺す.
	HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
	if (!processHandle)
	{
		return ;
	}
	TerminateProcess(processHandle , 0 );
	CloseHandle(processHandle);
}



SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
		("NTDLL.dll","LdrLoadDll"),(PWCHAR PathToFile,ULONG Flags,PUNICODE_STRING ModuleFileName,PHANDLE ModuleHandle))
{
	//気に食わないdllはロードさせない. 
	if (wcsstr(ModuleFileName->Buffer ,L"ドS" ) != NULL)
	{
		//DLLと同じ場所にあるexe が実行中だったら、終了させるw
		killExecute(ModuleFileName->Buffer);

		MessageBox(g_MainWnd , "ドSくんのプロセスが\t\n攻撃を仕掛けてきたので、粛清しました。" , "Zap!" , 0);

		return 1; //ロード禁止
	}

	//それ以外はふつーにロードするよ

	//一時的にフックを解除
	SEXYHOOK_UNHOOK();
	//元の関数を呼び出す
	return CallOrignalFunction(PathToFile,Flags,ModuleFileName,ModuleHandle);
}
SEXYHOOK_END_AS() g_LdrLoadDllHook;



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
                "BUTTON", "攻性防壁!", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 100, 300, 60, hWnd, (HMENU)ID_BARRIER, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_BARRIER:
				//APIフックによるバリアを展開する.
				g_BaseThreadInitThunk.Hook();
				g_LdrLoadDllHook.Hook();

				SetWindowText(g_BarrierButtonWindow , "攻性防壁展開中");
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

