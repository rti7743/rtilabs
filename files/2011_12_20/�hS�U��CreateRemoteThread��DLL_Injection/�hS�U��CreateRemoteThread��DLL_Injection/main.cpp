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
const char * MYNAME = "ドS攻撃CreateRemoteThreadとDLL_Injection";

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

//CreateRemoteThread で ドM を攻撃する
xreturn::r<bool> Attach()
{
	HMODULE kernel32dll = ::GetModuleHandle("Kernel32");

	//埋め込むフック用のdll
	char injectionDLLName[MAX_PATH];
	if( !GetModuleFileName( g_Instance ,injectionDLLName,_MAX_PATH) )
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "GetModuleFileNameに失敗。" , lastError);
	}
	_tcscpy(_tcsrchr(injectionDLLName, '\\') + 1, "ドS攻撃用DLL_CreateRemoteThread.dll" );

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

	//ドMくんのプロセスをひらきます。
	HANDLE processHandle = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ  | PROCESS_VM_WRITE , FALSE,processID );
	if (!processHandle)
	{
		DWORD lastError = ::GetLastError();
		return xreturn::windowsError(std::string() + "OpenProcessに失敗。" , lastError);
	}

	//文字列用のメモリを確保するために、ドMくん名義でメモリを確保しておきます。
	//おれおれドMなんだけどメモリをちょっと振り込んでくれないかなー？
	void * remoteMemory = VirtualAllocEx( processHandle, NULL , lstrlen(injectionDLLName) , MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	if (!remoteMemory)
	{
		DWORD lastError = ::GetLastError();
		CloseHandle(processHandle);
		return xreturn::windowsError(std::string() + "VirtualAllocExに失敗。" , lastError);
	}
	::WriteProcessMemory(processHandle, remoteMemory, (void*)injectionDLLName,lstrlen(injectionDLLName),NULL);

	//ターゲットプロセス上にスレッドを作成し、dllを注入します。

	HANDLE remoteThreadHandle;
	//フック
	remoteThreadHandle = ::CreateRemoteThread( processHandle, NULL, 0,	
					(LPTHREAD_START_ROUTINE) ::GetProcAddress(kernel32dll,"LoadLibraryA"), 
					remoteMemory, 0, NULL );

	if( !remoteThreadHandle )
	{
		DWORD lastError = ::GetLastError();

		::VirtualFreeEx( processHandle, injectionDLLName, lstrlen(injectionDLLName) , MEM_RELEASE );
		CloseHandle(processHandle);

		return xreturn::windowsError(std::string() + "CreateRemoteThreadに失敗。" , lastError);
	}

	//注入スレッドが終了するまで待ちます.
	::WaitForSingleObject( remoteThreadHandle, INFINITE );

	DWORD hLibModule;
	::GetExitCodeThread( remoteThreadHandle, &hLibModule );

	::VirtualFreeEx( processHandle, injectionDLLName, lstrlen(injectionDLLName) , MEM_RELEASE );
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
