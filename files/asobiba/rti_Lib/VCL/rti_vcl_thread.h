#include "StartWithExeFile.h"
#ifdef THREAD_LIFE

#ifndef ___TTHREADH
#define ___TTHREADH
#include <windows.h>

class TThread
{
public:
	//呼び出すルーチン , 引数 , その他 , 優先度  
    TThread(LPTHREAD_START_ROUTINE lpStartAdd,LPVOID lpvThreadParm,DWORD fdwCreate,int Priority);
    ~TThread();
	void WaitFor();
	void TheEnd();


	HANDLE ThreadHandle; //スレッドハンドル
	DWORD lpIDThread;	 //スレッドID
	bool AllBreaker;     //スレッド中断
};

#endif

#endif   // LIFE END
