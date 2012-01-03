#include <windows.h>
#include <iostream>
#include "xreturn.h"


const std::string DLLFILENAME = "pcdll.dll";
const int MYPID = 0x900;
//const int MYVID = 0x590;
const int MYVID = 0x711;

const int PC_OP_SW1_ON		=	0x1;
const int PC_OP_SW1_OFF		=	0x2;
const int PC_OP_SW2_ON		=	0x4;
const int PC_OP_SW2_OFF		=	0x8;


//構造化例外が発生すると、この関数が呼ばれる
void se_translator_function(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
	throw ep; //標準C++の例外を発生させる
}


class PTU2F3
{
	typedef BOOL (__stdcall *LPPCInitLib)(UINT);
	typedef VOID (__stdcall *LPPCCloseLib)(VOID);
	typedef UINT (__stdcall *LPPCGetStatus)(VOID);
	typedef BOOL (__stdcall *LPPCSetStatus)(UINT);

	LPPCInitLib PCInitLib;
	LPPCCloseLib PCCloseLib;
	LPPCGetStatus PCGetStatus;
	LPPCSetStatus PCSetStatus;

	HINSTANCE DLL;
public:
	PTU2F3()
	{
	}
	virtual ~PTU2F3()
	{
	}

	xreturn::r<bool> Init(const std::string & dllname)
	{
		this->DLL = ::LoadLibrary( dllname.c_str() );
		if (!this->DLL)
		{
			DWORD lasterror = ::GetLastError();
			return xreturn::windowsError(std::string("") + "DLL" + dllname + "が読み込めません",lasterror);
		}
		this->PCInitLib =  (LPPCInitLib)GetProcAddress(this->DLL, "PCInitLib");
		this->PCCloseLib = (LPPCCloseLib)GetProcAddress(this->DLL, "PCCloseLib");
		this->PCGetStatus = (LPPCGetStatus)GetProcAddress(this->DLL, "PCGetStatus");
		this->PCSetStatus = (LPPCSetStatus)GetProcAddress(this->DLL, "PCSetStatus");

		return true;
	}

	xreturn::r<bool> Power(int switchNumber , bool isON)
	{
		if (! (switchNumber == 4 || switchNumber == 5) )
		{
			return xreturn::error("指定されたスイッチはありません",0);
		}
		UINT nVIDPID = ((MYVID << 16) | (MYPID));
		this->PCInitLib(nVIDPID);

		//命令発行
		if (switchNumber == 4)
		{
			if (isON)
			{
				this->PCSetStatus(PC_OP_SW1_ON);
			}
			else
			{
				this->PCSetStatus(PC_OP_SW1_OFF);
			}
		}
		else //switchNumber == 5
		{
			if (isON)
			{
				this->PCSetStatus(PC_OP_SW2_ON);
			}
			else
			{
				this->PCSetStatus(PC_OP_SW2_OFF);
			}
		}

		::Sleep(3000);

		this->PCCloseLib();

		return true;
	}

};

int usage()
{
	return 1;
}

LONG CALLBACK ExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo)
{
	//どうも、このファームウェア制御用DLLがバグっているらしく・・・
	//初期化したメモリを参照するらしい。
	//非常に残念であるが、エラーを握りつぶすほかない。
	//このクソドライバーをリライトしたいのだが、ソースも仕様もないという・・・
	return(EXCEPTION_EXECUTE_HANDLER);
}

int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		return usage();
	}
	int switchNumber = atoi(argv[1]);
	if ( ! (switchNumber == 4 || switchNumber == 5) )
	{
		return usage();
	}

	bool flg = true;
	if ( stricmp(argv[2] , "on") == 0 )
	{
		flg = true;
	}
	else if ( stricmp(argv[2] , "off") == 0 )
	{
		flg = false;
	}
	else
	{
		return usage();
	}

	SetUnhandledExceptionFilter(ExceptionFilter);

	PTU2F3 o;
	{
		auto ret = o.Init(DLLFILENAME);
		if (!ret)
		{
			std::cout << ret.getErrorMessage() << std::endl;
			return usage();
		}
	}
	{
		auto ret = o.Power(switchNumber , flg);
		if (!ret)
		{
			std::cout << ret.getErrorMessage() << std::endl;
			return usage();
		}
	}


	return 0;
}