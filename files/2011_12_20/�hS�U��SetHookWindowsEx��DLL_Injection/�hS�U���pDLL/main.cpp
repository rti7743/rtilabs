/*
SetWindowHook を行うための フック用DLL
*/
#include <Windows.h>

HINSTANCE g_DLLInstance = NULL;
HHOOK g_Hook = NULL;
UINT WM_OREORE_HOOK = NULL;
const int ID_MI = 1000;



BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{	
	
	switch(fdwReason)
    {
        case    DLL_PROCESS_ATTACH:
			g_DLLInstance = hinstDLL;
			WM_OREORE_HOOK = ::RegisterWindowMessage("WM_OREORE_HOOK");
			break;

        case    DLL_PROCESS_DETACH:
            break;

        case    DLL_THREAD_ATTACH:
            break;

        case    DLL_THREAD_DETACH:
            break;
    }
    return  TRUE;
}

//フックで呼び出されるコールバック
//ターゲットのプロセスの中で呼び出される。
LRESULT CALLBACK HookProc(UINT code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* cw = ((CWPSTRUCT*)lParam);
	if (cw->message == WM_OREORE_HOOK)
	{
		//いたづらをする.
		SetWindowText( GetDlgItem(cw->hwnd , ID_MI) , "なのは完売" );

		//ずっとフックしつづけてもいいが、ターゲットの負荷を減らすためにここでフックを抜く.
		::UnhookWindowsHookEx( g_Hook );
		g_Hook = NULL;
	}
	return ::CallNextHookEx(g_Hook, code, wParam, lParam);
}

//フックのスタート
extern "C" __declspec(dllexport)  int  InjectDll( HWND targetwindow )
{	
	g_Hook = SetWindowsHookEx( WH_CALLWNDPROC
		,(HOOKPROC)HookProc
		,g_DLLInstance
		,GetWindowThreadProcessId(targetwindow,NULL)
		);
	if (g_Hook == NULL)
	{
		return false;
	}
	//メッセージを投げて、フック内での最初の処理をする.
	SendMessage(targetwindow,WM_OREORE_HOOK , 0 , 0);
	return true;
}

