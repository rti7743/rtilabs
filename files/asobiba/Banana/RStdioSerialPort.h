// RStdioSerialPort.h: RStdioSerialPort クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTDIOSERIALPORT_H__D2D55D80_AA45_4D92_A878_A00BA94BD36A__INCLUDED_)
#define AFX_RSTDIOSERIALPORT_H__D2D55D80_AA45_4D92_A878_A00BA94BD36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RCreateFile.h"

class RStdioSerialPort  : public RIStream
{
public:
	RStdioSerialPort();
	virtual ~RStdioSerialPort();

	void Open(int inPortNumber,int speed, int inTimeOut = 2000) throw (RWin32Exception);

	//閉じる.
	void Close()
	{
		File.Close();
	}

	virtual int Read(void* inPtr , DWORD inSize ) throw(RWin32Exception);
	virtual int Write(const void* inPtr , int inSize) throw(RWin32Exception);
private:
	void QueeClear();

private:
	RCreateFile	File;
};

#endif // !defined(AFX_RSTDIOSERIALPORT_H__D2D55D80_AA45_4D92_A878_A00BA94BD36A__INCLUDED_)
