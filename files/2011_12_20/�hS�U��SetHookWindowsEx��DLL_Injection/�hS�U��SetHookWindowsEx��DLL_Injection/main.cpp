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
const char * MYNAME = "ドS攻撃HookWindowsExでDLL_Injection";

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

	HMODULE dll = LoadLibrary("ドS攻撃用DLL.dll");
	if (!dll)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError("攻撃用DLLのLoadLibraryに失敗",lastError );
	}
	typedef bool (*INJECTDLLDEF)( HWND targetwindow );
	INJECTDLLDEF injcetdllFunction = (INJECTDLLDEF)::GetProcAddress(dll,"InjectDll");
	if (!injcetdllFunction)
	{
		DWORD lastError = ::GetLastError();
		FreeLibrary(dll);
		return xreturn::windowsError("攻撃用DLLに InjectDllがありません",lastError );
	}
	bool r = injcetdllFunction(targetWindow);
	if (!r)
	{
		FreeLibrary(dll);
		return xreturn::error(std::string() + "フックに失敗しました" ,0);
	}

	FreeLibrary(dll);
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
