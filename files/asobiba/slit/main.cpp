
#include "comm.h"
#include "WindowsMain.h"
#include "RMessageBox.h"

//テスト
#include "MVFileList.h"

int WINAPI WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance,  TCHAR* lpCmdLine, int nCmdShow)
{
	int ret = -1;

	try
	{
#ifndef _WIN32_WCE
		#ifdef _dEBUG
			//テスト
			MVFileList::test();
		#endif
#endif

		//起動...
		WindowsMain* winMain = WindowsMain::getMainSystem();
		ret = winMain->Run();
	}
	catch(RException e)
	{
		TRACE( e.getMessage() );
		RMessageBox::Message( e.getMessage() );
		ASSERT(0);

	}
	catch(std::bad_alloc)
	{
		TRACE( "BAD alloc!" );
		RMessageBox::Message( "BAD alloc!" );
		ASSERT(0);
	}
/*
	catch(...)
	{
		TRACE( "未知のエラー" );
		RMessageBox::Message( "未知のエラー" );
		ASSERT(0);
	}
*/
#ifndef _WIN32_WCE
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
#endif
	return ret;
}

