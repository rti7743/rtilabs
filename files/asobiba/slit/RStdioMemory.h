// RStdioMemory.h: RStdioMemory クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTDIOMEMORY_H__203F2FA5_25AD_4A96_A7BA_A3C4C32AD754__INCLUDED_)
#define AFX_RSTDIOMEMORY_H__203F2FA5_25AD_4A96_A7BA_A3C4C32AD754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"

class RStdioMemory  
{
public:
	RStdioMemory()
	{
	}
	~RStdioMemory()
	{
	}

	//開く
	void Open(const char* ioBuffer )
	{
		Buffer = ioBuffer;
		Pointer = 0;
	}
	//閉じる	dummy
	void Close(){}	



	inline unsigned long readGetUL()
	{
		register unsigned long ptr;
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned long));
#ifdef ARM
		*(((unsigned char*)&ptr)+0) = *((unsigned char*)(Buffer + Pointer)+0);
		*(((unsigned char*)&ptr)+1) = *((unsigned char*)(Buffer + Pointer)+1);
		*(((unsigned char*)&ptr)+2) = *((unsigned char*)(Buffer + Pointer)+2);
		*(((unsigned char*)&ptr)+3) = *((unsigned char*)(Buffer + Pointer)+3);
#else
		ptr = *(unsigned long*)(Buffer + Pointer);
#endif
		Pointer += sizeof(unsigned long);

		return ptr;
	}
	inline unsigned short readGetUS()
	{
		register unsigned short ptr;
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned short));
#ifdef ARM
		*(((unsigned char*)&ptr)+0) = *((unsigned char*)(Buffer + Pointer)+0);
		*(((unsigned char*)&ptr)+1) = *((unsigned char*)(Buffer + Pointer)+1);
#else
		ptr = *(unsigned short*)(Buffer + Pointer);
#endif

		Pointer += sizeof(unsigned short);

		return ptr;
	}
	inline unsigned char readGetUC()
	{
		register unsigned char ptr;
		ptr = *(unsigned char*)(Buffer + Pointer);
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned char));

		Pointer += sizeof(unsigned char);

		return ptr;
	}
	inline char readGetC()
	{
		register char ptr;
		ptr = *(char*)(Buffer + Pointer);
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned char));

		Pointer += sizeof(char);

		return ptr;
	}

	void fread(void* inPtr , int inSize )
	{
		::memcpy(inPtr , Buffer + Pointer , inSize);

		Pointer += inSize;
	}
	int fscan(const char* inFormat,...)
	{
		return ::sscanf( Buffer + Pointer , inFormat,(char*)(&inFormat+1));
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
	const char*		Buffer;
	int				Pointer;
};

#endif // !defined(AFX_RSTDIOMEMORY_H__203F2FA5_25AD_4A96_A7BA_A3C4C32AD754__INCLUDED_)
