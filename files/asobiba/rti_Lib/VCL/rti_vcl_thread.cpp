#include "StartWithExeFile.h"
#ifdef THREAD_LIFE

#include <.\VCL\rti_vcl_thread.h>
/*******************************************************************/
//コンストラクタ
/*******************************************************************/
TThread::TThread(LPTHREAD_START_ROUTINE lpStartAdd,LPVOID lpvThreadParm = NULL,DWORD fdwCreate = NULL,int Priority = THREAD_PRIORITY_NORMAL)
{
	//スレッド作成
	ThreadHandle = CreateThread(
		NULL, 0, lpStartAdd, lpvThreadParm, fdwCreate, &lpIDThread);
	if(Priority != THREAD_PRIORITY_NORMAL)
	{ //優先度設定
		SetThreadPriority(ThreadHandle, Priority);
	}
	//AllBreaker が true に設定されたらスレッドを抜けるようにつくる必要があります
	AllBreaker = false;
}

/*******************************************************************/
//デストラクタ
/*******************************************************************/
TThread::~TThread()
{
	if(ThreadHandle)
	{ //スレッドがまだ生きている
		WaitFor(); //終了させるまで待つ
		//スレッドハンドルを閉じる
		CloseHandle(ThreadHandle);
	}
}

/*******************************************************************/
//スレッドが終わるまで待つ
/*******************************************************************/
void TThread::WaitFor()
{
	AllBreaker = true;
	WaitForSingleObject(ThreadHandle,INFINITE);
}
/*******************************************************************/
//ハンドルをクリアする
/*******************************************************************/
void TThread::TheEnd()
{
	if(ThreadHandle)
	{
		CloseHandle(ThreadHandle);
		ThreadHandle=NULL;
	}
}

#endif //LIFE END
