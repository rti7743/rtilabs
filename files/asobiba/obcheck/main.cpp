
#include <windows.h>
#include <process.h>
#include <Dbghelp.h>

#include <stdio.h>

#define ASSERT
#define TRACE

class StackTrace
{
public:
	StackTrace()
	{
		this->CurrentProccessHandle = NULL;
		this->CurrentThreadHandle = NULL;
		this->IsGet = false;
	}
	~StackTrace()
	{
		//これは閉じなくてもいい.
		this->CurrentProccessHandle = NULL;

		//こっちは閉じる
		if (this->CurrentThreadHandle)
		{
			CloseHandle(this->CurrentThreadHandle);
			this->CurrentThreadHandle = NULL;
		}

	}
private:
	//現在のプロセスハンドル
	HANDLE CurrentProccessHandle;
	//現在のスレッドハンドル
	HANDLE CurrentThreadHandle;

	//成功したかな?
	bool IsGet;
private:
//	static unsigned int __stdcall _Start(void* inThis)
	static unsigned long __stdcall _Start(void* inThis)
	{
		return ((StackTrace*)inThis)->Run();
	}

	//スタック取得スレッドの中
	unsigned int Run()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		//まず、調査したいスレッドを停止させる.
		::SuspendThread(this->CurrentThreadHandle);

		StackDump();

		//調査が完了したので復帰させる.
		::ResumeThread(this->CurrentThreadHandle);

		return 0;
	}

	void StackDump()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		this->CurrentProccessHandle = ::GetCurrentProcess();

		BOOL r;

		//スレッドのコンテキストゲット
		CONTEXT threadContext = {0};
		threadContext.ContextFlags = CONTEXT_FULL;
		r = ::GetThreadContext(this->CurrentThreadHandle , &threadContext);
		if (!r)
		{
			ASSERT(0);
			return ;
		}

		//シンボルエンジンの初期化
		r = ::SymInitialize(this->CurrentProccessHandle , NULL , TRUE);
		if (!r)
		{
			ASSERT(0);
			return ;
		}

		//行番号までロードしてほしいなー
		DWORD option = ::SymGetOptions();
		option |= SYMOPT_LOAD_LINES;
		option &= ~SYMOPT_UNDNAME;
		::SymSetOptions(option);


		//調査したいスレッドのスタックの情報はこんな感じ.
		STACKFRAME stackFrame = {0};
		stackFrame.AddrPC.Offset    = threadContext.Eip;
		stackFrame.AddrStack.Offset = threadContext.Esp;
		stackFrame.AddrFrame.Offset = threadContext.Ebp;

		stackFrame.AddrPC.Mode      = AddrModeFlat;
		stackFrame.AddrStack.Mode   = AddrModeFlat;
		stackFrame.AddrFrame.Mode   = AddrModeFlat;
		stackFrame.AddrReturn.Mode  = AddrModeFlat;
		stackFrame.AddrBStore.Mode  = AddrModeFlat;

		while(1)
		{
			//スタックをもぐります.
			r =	StackWalk(IMAGE_FILE_MACHINE_I386 , 
					this->CurrentProccessHandle , this->CurrentThreadHandle ,
					&stackFrame, &threadContext , NULL, 
					SymFunctionTableAccess, SymGetModuleBase, NULL);
			if (!r)
			{
				break;
			}
			if ( stackFrame.AddrPC.Offset == 0 )
			{
				break;
			}

			IMAGEHLP_MODULE imageModule = { sizeof(IMAGEHLP_MODULE) };
			r = SymGetModuleInfo(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &imageModule);
			if (!r)
			{
				printf("%p:no module:no symbol\r\n" , stackFrame.AddrPC.Offset );
			}
			else
			{
				//シンボル情報格納バッファ.
				IMAGEHLP_SYMBOL * imageSymbol;
				char buffer[6000 + sizeof(IMAGEHLP_SYMBOL) ] = {0};
				imageSymbol = (IMAGEHLP_SYMBOL*)buffer;
				imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
				imageSymbol->MaxNameLength = 6000 - sizeof(IMAGEHLP_SYMBOL);

				//関数名の取得...
				DWORD disp = 0;
				r = ::SymGetSymFromAddr(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &disp , imageSymbol );
				if (!r)
				{//関数名がわかりません.
					printf("%p:%s:no symbol\r\n" , stackFrame.AddrPC.Offset , imageModule.ModuleName );
				}
				else
				{
					//行番号の取得
					IMAGEHLP_LINE line ={0};
					r = ::SymGetLineFromAddr
						(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &disp , &line);
					if (!r)
					{//行番号が分かりません
						printf("%p:%s:%s\r\n" , stackFrame.AddrPC.Offset , imageModule.ModuleName , imageSymbol->Name);
					}
					else
					{//行番号がわかりました.
						printf("%p:%s:%s %s:%d\r\n" , stackFrame.AddrPC.Offset , 
										imageModule.ModuleName , imageSymbol->Name , 
										line.FileName , line.LineNumber	);
					}
				}
			}
		}
		//シンボルエンジンの終了
		SymCleanup(this->CurrentProccessHandle);

		this->IsGet = true;


	}
public:
	bool Here()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		HANDLE currentProccessHandle = ::GetCurrentProcess();

		//現在のスレッドのハンドルを取得...
		//でも、 GetCurrentThread で取得できるのは、そのプロセス内で有効なダミーで
		//私はちゃんとしたやつがほしいんだな
		//それは、 DuplicateHandle で複製すると手に入る!!
		HANDLE currentThreadHandleDummy = ::GetCurrentThread();
		HANDLE currentThreadHandle;
		BOOL dup = 
			::DuplicateHandle(	currentProccessHandle , currentThreadHandleDummy , 
								currentProccessHandle , &currentThreadHandle , 
								0 , FALSE , DUPLICATE_SAME_ACCESS);
		if (!dup)
		{
			TRACE("StackTrace::Here:DuplicateHandle error:%d" , ::GetLastError() );
			ASSERT(0);
			return false;
		}
		//念願のスレッドハンドルを手に入れたぞ!!
		this->CurrentProccessHandle = currentProccessHandle;
		this->CurrentThreadHandle = currentThreadHandle;

		//現在のスレッドのスタック情報を自分自身のスレッドで見ることは出来ません
		//だって、スタック取得の処理を走らせたら、スタックに余計に荷物が詰まれるし
		//よって、別スレッドを作成します.
//		unsigned int threadID = 0;
//		HANDLE stackTrackerThread = (HANDLE)_beginthreadex(NULL , 0 , _Start , (void*) this ,  0 , &threadID);
		unsigned long threadID = 0;
		HANDLE stackTrackerThread = CreateThread(NULL , 0 ,_Start , (VOID*) this , 0 , &threadID);
		if (stackTrackerThread == NULL)
		{
			TRACE("StackTrace::Here:_beginthreadex error:%d" , ::GetLastError() );
			ASSERT(0);

			return false;
		}

		//スタック取得スレッドが終わるまで待機.
		while( WaitForSingleObject(stackTrackerThread , 0 ) != WAIT_OBJECT_0 )
		{
		}

		//スタック取得スレッドを停止、お疲れ様
		::CloseHandle(stackTrackerThread);
		stackTrackerThread = NULL;

		return this->IsGet;
	}
};


int sum(int a , int b)
{
	StackTrace st;
	st.Here();

	return a + b;
}

void main()
{
	int ans = sum(1,2);

	printf("ans:%d\r\n" , ans);
}