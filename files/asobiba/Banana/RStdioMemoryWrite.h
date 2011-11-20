// RStdioMemoryWrite.h: RStdioMemoryWrite クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTDIOMEMORYWRITE_H__560F8CCE_755A_4631_A2E3_B5340778F51A__INCLUDED_)
#define AFX_RSTDIOMEMORYWRITE_H__560F8CCE_755A_4631_A2E3_B5340778F51A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"

class RStdioMemoryWrite  
{
public:
	RStdioMemoryWrite()
	{
	}
	~RStdioMemoryWrite()
	{
	}

	//開く
	void Open(char* ioBuffer )
	{
		Buffer = ioBuffer;
		Pointer = 0;
	}
	//閉じる	dummy
	void Close(){}	



	void writeGetUL(unsigned long inPtr)
	{
#ifdef ARM
		*((unsigned char*)(Buffer + Pointer)+0) = *(((unsigned char*)&inPtr)+0);
		*((unsigned char*)(Buffer + Pointer)+1) = *(((unsigned char*)&inPtr)+1);
		*((unsigned char*)(Buffer + Pointer)+2) = *(((unsigned char*)&inPtr)+2);
		*((unsigned char*)(Buffer + Pointer)+3) = *(((unsigned char*)&inPtr)+3);
#else
		*(unsigned long*)(Buffer + Pointer) = inPtr ;
#endif
		Pointer += sizeof(unsigned long);
	}
	void writeGetUS(unsigned short inPtr)
	{
#ifdef ARM
		*((unsigned char*)(Buffer + Pointer)+0) = *(((unsigned char*)&inPtr)+0);
		*((unsigned char*)(Buffer + Pointer)+1) = *(((unsigned char*)&inPtr)+1);
#else
		*(unsigned short*)(Buffer + Pointer) = inPtr ;
#endif

		Pointer += sizeof(unsigned short);
	}
	void writeGetUC(unsigned char inPtr)
	{
		*(unsigned char*)(Buffer + Pointer) = inPtr;

		Pointer += sizeof(unsigned char);
	}
	void writeGetC(char inPtr)
	{
		*(char*)(Buffer + Pointer) = inPtr;

		Pointer += sizeof(char);
	}

	void fwrite(const void* inPtr , int inSize )
	{
		::memcpy(Buffer + Pointer , inPtr , inSize);

		Pointer += inSize;
	}

	long fseek(long inP , int inFlg = SEEK_SET)
	{
		if (inFlg == SEEK_SET)
		{
			Pointer = inP;
		}
		else if (inFlg == SEEK_CUR)
		{
			Pointer += inP;
		}
		return Pointer;
	}

	long ftell() const
	{
		return Pointer;
	}

	//長危険!
	const char* ptr()
	{
		return Buffer + Pointer;
	}
private:
	char*			Buffer;
	int				Pointer;
};
#endif // !defined(AFX_RSTDIOMEMORYWRITE_H__560F8CCE_755A_4631_A2E3_B5340778F51A__INCLUDED_)
