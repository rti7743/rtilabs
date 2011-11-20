// RNeoSerialPort.cpp: RNeoSerialPort クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RNeoSerialPort.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////



void RNeoSerialPort::Open(int inPortNumber,const string & inDCB , int inTimeOut) throw (RWin32Exception)
{

//	if ( BuildCommDCB(inDCB.c_str() , &dcb) == FALSE )
	{
//		throw RWin32Exception("BuildCommDCB(%s)に失敗" , inDCB.c_str() );
	}

	File.Open( string("COM") + num2str(inPortNumber) + ":" ,
			GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		WaitCommEvent(


	DCB dcb = {0};
	if ( GetCommState(File.getHandle(), &dcb) == FALSE )
	{
		throw RWin32Exception("GetCommState に失敗");
	}
	dcb.BaudRate = CBR_4800 ;
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
void RNeoSerialPort::QueeClear()
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



int RNeoSerialPort::Read(void* outPtr , int inSize ) throw(RWin32Exception)
{
	COMSTAT		comstate;
	DWORD		errorMask;
	
//	TRACE("//COMから読み込みます.");
	int first = ::GetTickCount();
	while( 1 )
	{
		ClearCommError(File.getHandle(), &errorMask, &comstate);

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
		::Sleep(50);
	}
//	TRACE("//COMから %d 読み込みます." , (int)comstate.cbInQue);
	return File.Read(outPtr,inSize < comstate.cbInQue ? inSize : comstate.cbInQue);
}
int RNeoSerialPort::Write(const void* inPtr , int inSize) throw(RWin32Exception)
{
	return File.Write(inPtr,inSize);
}
