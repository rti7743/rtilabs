// RNeoSerialPort.h: RNeoSerialPort クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNEOSERIALPORT_H__ABB2FE77_CDDD_4840_A7A3_75A6DE914980__INCLUDED_)
#define AFX_RNEOSERIALPORT_H__ABB2FE77_CDDD_4840_A7A3_75A6DE914980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RCreateFile.h"
#include "RNeoIStreamble.h"

class RNeoSerialPort  : public RNeoIStreamble
{
public:
	RNeoSerialPort();
	virtual ~RNeoSerialPort();

	void Open(int inPortNumber,const string & inDCB = "9600,N,8,1" , int inTimeOut = 2000) throw (RWin32Exception);

	//閉じる.
	void Close()
	{
		File.Close();
	}

	virtual int Read(void* outPtr , int inSize ) throw(RWin32Exception);
	virtual int Write(const void* inPtr , int inSize) throw(RWin32Exception);
private:
	void QueeClear();

private:
	RCreateFile	File;

};

#endif // !defined(AFX_RNEOSERIALPORT_H__ABB2FE77_CDDD_4840_A7A3_75A6DE914980__INCLUDED_)
