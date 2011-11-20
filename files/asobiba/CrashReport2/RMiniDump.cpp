// RMiniDump.cpp: RMiniDump クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include "RMiniDump.h"
#include "RAutoLibrary.h"
#include "RThread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//miniダンプをとるため専用のクラス
class RMiniDumpChild : public RThread
{

public:
	void Set(const string & inFileName , MINIDUMP_TYPE inFlag,EXCEPTION_POINTERS *poolExceptionInfo)
	{

		this->Filename = inFileName;
		this->Flag = inFlag;
		this->PoolExceptionInfo = poolExceptionInfo;
		
		DbgHelpDLL.Load("dbghelp.dll");
		this->Func_MiniDumpWriteDump = (FUNC_MINIDUMPWRITEDUMP)DbgHelpDLL.GetProcAddress("MiniDumpWriteDump");
	}
	
	
	unsigned int RMiniDumpChild::Run();
private:
	string				Filename;
	MINIDUMP_TYPE		Flag;
	EXCEPTION_POINTERS*	PoolExceptionInfo;
	
	//dbghelp.dll から MiniDumpWriteDump をインポートする.
	RAutoLibrary		DbgHelpDLL;
	typedef BOOL (WINAPI *FUNC_MINIDUMPWRITEDUMP)(
		HANDLE hProcess,
		DWORD ProcessId,
		HANDLE hFile,
		MINIDUMP_TYPE	DumpType,
		CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
		CONST PVOID	UserStreamParam,
		CONST PVOID	CallbackParam
		);
	FUNC_MINIDUMPWRITEDUMP Func_MiniDumpWriteDump;
	
};

unsigned int RMiniDumpChild::Run()
{
	HANDLE file = CreateFile(
		this->Filename.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (this->PoolExceptionInfo == NULL)
	{
		//現在のアドレス.
		this->Func_MiniDumpWriteDump(
			GetCurrentProcess(), 
			GetCurrentProcessId(), 
			file, 
			this->Flag, 
			NULL,
			NULL,
			NULL
		);
	}
	else
	{
		//クラッシュが発生したと思われる場所
		MINIDUMP_EXCEPTION_INFORMATION   stExInfo ;

		stExInfo.ThreadId = GetCurrentThreadId ( ) ;
		stExInfo.ClientPointers = TRUE ;
		stExInfo.ExceptionPointers = this->PoolExceptionInfo ;

		this->Func_MiniDumpWriteDump(
			GetCurrentProcess(), 
			GetCurrentProcessId(), 
			file, 
			this->Flag, 
			&stExInfo,
			NULL,
			NULL
		);
	}
	CloseHandle(file);
	return 0;	
}


//任意に mini dumpを作成する.
bool RMiniDump::MiniDump(const string & inFilename , MINIDUMP_TYPE inFlag,EXCEPTION_POINTERS *poolExceptionInfo )
{
	try
	{
		RMiniDumpChild minidump;
		minidump.Set(inFilename,inFlag,poolExceptionInfo);
		//スレッド開始
		minidump.Start();
		//終了まで待つ
		minidump.Join();
	}
	catch(RException &e)
	{
		printf("エラー: %s\r\n",e.getMessage() );
		//Dumpを取るような自体のときに例外を飛ばしたらダメだろうJK
		return false;
	}
	return true;
}
