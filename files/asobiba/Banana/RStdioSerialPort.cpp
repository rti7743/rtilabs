// RStdioSerialPort.cpp: RStdioSerialPort クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RStdioSerialPort.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStdioSerialPort::RStdioSerialPort()
{

}

RStdioSerialPort::~RStdioSerialPort()
{

}

void RStdioSerialPort::Open(int inPortNumber,int speed , int inTimeOut) throw (RWin32Exception)
{

	File.Open( string("COM") + num2str(inPortNumber) + ":" ,
			GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DCB dcb = {0};
	if ( GetCommState(File.getHandle(), &dcb) == FALSE )
	{
		throw RWin32Exception("GetCommState に失敗");
	}
	dcb.BaudRate = speed ;
	dcb.ByteSize = 8 ;
	dcb.StopBits = ONESTOPBIT ;
	dcb.Parity = NOPARITY ;

	if ( SetCommState( File.getHandle() ,&dcb) == FALSE )
	{
		throw RWin32Exception("SetCommState に失敗");
	}

	//受信キューのクリア.
	QueeClear();
}

//受信キューのクリア.
void RStdioSerialPort::QueeClear()
{
	char buffer[1024];
	COMSTAT		comstate;
	DWORD		errorMask;

//	TRACE("//受信キューのクリア.");
	while(1)
	{
		ClearCommError(File.getHandle(), &errorMask, &comstate);
		if (comstate.cbInQue == 0) break;

		Read(buffer , 1024);
	}
//	TRACE("//受信キューのクリア 終了.");
}



int RStdioSerialPort::Read(void* inPtr , DWORD inSize ) throw(RWin32Exception)
{
	COMSTAT		comstate;
	DWORD		errorMask;
	
//	TRACE("//COMから読み込みます.");
	int first = ::GetTickCount();
	while( 1 )
	{
		if (! ClearCommError(File.getHandle(), &errorMask, &comstate) )
		{
			throw RWin32Exception(EXCEPTIONTRACE + ":ClearCommError");
		}

		//受信バッファになんか入ってる?
		if ( comstate.cbInQue > 0) break;
		//60秒でタイムアウト.
		if (::GetTickCount() - first > 1000 * 60)
		{
//			TRACE("//COMから読み込み中にタイムアウト.");
			return 0;
		}
//		TRACE("//COMから読み込めないようなのでしばらく休みます.");
		//ちょっとお休み
		::Sleep(1000);
	}
//	TRACE("//COMから %d 読み込みます." , (int)comstate.cbInQue);
	return File.Read(inPtr,inSize < comstate.cbInQue ? inSize : comstate.cbInQue);
}
int RStdioSerialPort::Write(const void* inPtr , int inSize) throw(RWin32Exception)
{
	return File.Write(inPtr,inSize);
}
