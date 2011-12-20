/*
CreateRemoteThread による DLL INJECTION で ドMを攻撃するサンプルです。

*/
#include <windows.h>
#include "xreturn.h"
#include <tchar.h>
//#include <imagehlp.h>
#include <tlhelp32.h>	

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_Instance = NULL;
HWND g_MainWnd = NULL;             //メインウィンドウ
const char * MYNAME = "ドS攻撃CreateRemoteThreadだけ";

//ドMプロセスを見つけるコード。
//"ドMやられ" というプロセス名で最初にマッチしたものをターゲットとする。
xreturn::r<DWORD> findTargetProcess()
{
	DWORD dwProcessId = NULL;

	HANDLE hSnap;
	if((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "CreateToolhelp32Snapshotに失敗" , lastError);
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	BOOL bResult = Process32First(hSnap, &pe);
	while(bResult)
	{
		if(strstr(pe.szExeFile, "ドMやられ") != NULL)
		{
			CloseHandle(hSnap);
			return pe.th32ProcessID;
		}
		bResult = Process32Next(hSnap, &pe);
	}
	CloseHandle(hSnap);
	return 0;
}

//プロセスIDから対応するHWND を取得する.
//出典元: http://d.hatena.ne.jp/yu-hr/20100323/1269355469
HWND GetWindowHandle(	// 戻り値: 成功 望みのHWND / 失敗 NULL
	const DWORD TargetID)	// プロセスID
{
	HWND hWnd = GetTopWindow(NULL);
	do {
		if(GetWindowLong( hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible( hWnd))
		{
			continue;
		}
		DWORD ProcessID;
		GetWindowThreadProcessId( hWnd, &ProcessID);
		if(TargetID == ProcessID)
		{
			return hWnd;
		}
	} while((hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

	return NULL;
}


typedef BOOL (__stdcall  *__SetWindowTextA)(HWND hWnd,LPCSTR lpString);
typedef HWND (__stdcall  *__GetDlgItem)(HWND hWnd,int nIDDlgItem);
struct INJECTIONDATA
{
	HWND             targetWindow;
	__SetWindowTextA funcSetWindowTextA;
	__GetDlgItem     funcGetDlgItem;
	char             nanohaKanbaiString[MAX_PATH];
};

const int ID_MI = 1000;
//相手プロセスで動作するルーチン
//必然的に他の関数を呼べないので、長いコードなるでしょうね。
DWORD __stdcall InjectionThread(LPVOID lpParameter)
{
	//ジャンプさせるコードを作るとアドレスがずれるので注意してやる。
	//アドレス計算はめんどいのでコンパイラにやってもらうよw
	INJECTIONDATA* data = (INJECTIONDATA*)lpParameter;
	HWND targetWindow = data->targetWindow;
	__SetWindowTextA funcSetWindowTextA = data->funcSetWindowTextA;
	__GetDlgItem funcGetDlgItem = data->funcGetDlgItem;
	const char * nanohaKanbaiString = data->nanohaKanbaiString;
	int id_mi = ID_MI;

//こういう演算がしたい。
//	data->funcSetWindowTextA( data->funcGetDlgItem(data->targetWindow , ID_MI ) , data->nanohaKanbaiString );
	__asm
	{
		push id_mi;
		push targetWindow;
		call funcGetDlgItem;

		push nanohaKanbaiString;
		push eax;

		call funcSetWindowTextA;

		xor eax,eax; //return 0;

		mov         esp,ebp   //エピローグ
        pop         ebp  

		ret;
	};

	//デバッグモードによっては、 espチェッカーが入り、それによる相対ジャンプが発生してしまってまともに動かない。
	//よって、この return は使用してはいけない。
	return 0;
}

//関数が開始される位置を求める.
uintptr_t CalcSemeFunctionAddress(void * inSemeFunctionProc )
{
	//フック関数が開始される絶対アドレス
	if (*((unsigned char*)inSemeFunctionProc+0) == 0xe9)
	{
		//フック関数も ILT経由で飛んでくる場合
		//0xe9 call [4バイト相対アドレス]          4バイト相対は64bit でも変わらず
		uintptr_t jmpaddress = *((unsigned long*)((unsigned char*)inSemeFunctionProc+1));
		return (((uintptr_t)inSemeFunctionProc) + jmpaddress) + 5;
	}
	else
	{
		//即、プログラム領域に飛んでくる場合
		return (uintptr_t)inSemeFunctionProc ;
	}
}


xreturn::r<int> Attach()
{
	//ドMくんのプロセスを探します。
	auto processID = findTargetProcess();
	if (processID.isError())
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "GetModuleFileNameに失敗。" , lastError);
	}
	if ( (DWORD)processID == 0)
	{
		return xreturn::error(std::string() + "攻撃対象がいません" ,0);
	}
	//ドMくんのメインウィンドウを見つけます。
	HWND targetWindow = GetWindowHandle(processID);
	if (!targetWindow)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "メインウィンドウが見つかりません。" , lastError);
	}

	//RemoteThreadに渡すデータを作る。
	INJECTIONDATA injectionSrc = {0};
	injectionSrc.targetWindow = targetWindow;
	
	//必須API系のロード.
	HMODULE user32dll = ::GetModuleHandle("User32");
	injectionSrc.funcSetWindowTextA = (__SetWindowTextA)::GetProcAddress(user32dll,"SetWindowTextA");
	injectionSrc.funcGetDlgItem = (__GetDlgItem)::GetProcAddress(user32dll,"GetDlgItem");
	strcpy(injectionSrc.nanohaKanbaiString , "なのは完売");

	//フック用関数のアドレスを求めます。
	//長さはわからないので、てきとーに取ります。多少余計にとっても死ぬわけではないので大丈夫だよ、、たぶん。
	uintptr_t functionStart = CalcSemeFunctionAddress(InjectionThread);
	uintptr_t functionSize = 256; //関数の長さだし 256バイトもあればいいんじゃね？

	//ドMくんのプロセスをひらきます。
	HANDLE processHandle = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ  | PROCESS_VM_WRITE , FALSE,processID );
	if (!processHandle)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "OpenProcessに失敗。" , lastError);
	}

	//実行命令文をドMくん名義でメモリを確保しておきます。
	//おれおれドMなんだけどメモリをちょっと振り込んでくれないかなー？
	void * remoteCodeMemory = VirtualAllocEx( processHandle, NULL , functionSize , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteCodeMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		return xreturn::windowsError(std::string() + "VirtualAllocExに失敗。remoteCodeMemoryが確保できぬ。" , lastError);
	}
	DWORD dwNumBytesXferred;
	::WriteProcessMemory(processHandle, remoteCodeMemory, (void*)functionStart,functionSize,&dwNumBytesXferred);


	//次にワークメモリを確保するために、ドMくん名義でメモリを確保しておきます。
	//おれおれドMなんだけどメモリをちょっと振り込んでくれないかなー？
	void * remoteDataMemory = VirtualAllocEx( processHandle, NULL , functionSize , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteDataMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
		return xreturn::windowsError(std::string() + "VirtualAllocExに失敗。remoteDataMemoryが確保できぬ。" , lastError);
	}
	::WriteProcessMemory(processHandle, remoteDataMemory, (void*)&injectionSrc,sizeof(INJECTIONDATA) ,&dwNumBytesXferred);

	HANDLE remoteThreadHandle;
	remoteThreadHandle = ::CreateRemoteThread( processHandle, NULL, 0,	
					(LPTHREAD_START_ROUTINE) remoteCodeMemory, 
					remoteDataMemory, 0, NULL );

	if( !remoteThreadHandle )
	{
		DWORD lastError = ::GetLastError();

		::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
		::VirtualFreeEx( processHandle, remoteDataMemory, functionSize , MEM_RELEASE );
		CloseHandle(processHandle);

		return xreturn::windowsError(std::string() + "CreateRemoteThreadに失敗。" , lastError);
	}

	//注入スレッドが終了するまで待ちます.
	::WaitForSingleObject( remoteThreadHandle, INFINITE );

	DWORD hLibModule;
	::GetExitCodeThread( remoteThreadHandle, &hLibModule );

	::VirtualFreeEx( processHandle, remoteCodeMemory, functionSize , MEM_RELEASE );
	::VirtualFreeEx( processHandle, remoteDataMemory, functionSize , MEM_RELEASE );
	::CloseHandle( processHandle );
	return true;
}


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
	const int ID_ATTACH = 1000;
	static HWND g_AttachButtonWindow = NULL;	 //攻撃ボタン
    switch (msg) {
		case WM_CREATE:
            g_AttachButtonWindow = CreateWindow(
                "BUTTON", "ボタンを押すとドMくんを攻撃するよ。", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 20, 350, 60, hWnd, (HMENU)ID_ATTACH, g_Instance ,NULL);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
			case ID_ATTACH:
				auto r = Attach();
				if (!r)
				{
					std::string errorMessage = std::string() + "攻撃に失敗しました。" + r.getErrorMessage();
					MessageBox( NULL , errorMessage.c_str()  , "失敗" , 0);
					break;
				}

				SetWindowText(g_AttachButtonWindow , "攻撃成功!?");
				EnableWindow(g_AttachButtonWindow , FALSE);
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
