// RNeoMemory.h: RNeoMemory クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNEOMEMORY_H__60382F6B_BB62_4B4F_A3F7_70D90CFA4AF2__INCLUDED_)
#define AFX_RNEOMEMORY_H__60382F6B_BB62_4B4F_A3F7_70D90CFA4AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNeoIStreamble.h"

class RNeoMemory  : public RNeoIStreamble
{
public:
	RNeoMemory()
	{
	}
	~RNeoMemory()
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



	unsigned long readGetUL()
	{
		unsigned long ptr;
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
	unsigned short readGetUS()
	{
		unsigned short ptr;
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
	unsigned char readGetUC()
	{
		unsigned char ptr;
		ptr = *(unsigned char*)(Buffer + Pointer);
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned char));

		Pointer += sizeof(unsigned char);

		return ptr;
	}
	char readGetC()
	{
		char ptr;
		ptr = *(char*)(Buffer + Pointer);
//		memcpy(&ptr , Buffer + Pointer , sizeof(unsigned char));

		Pointer += sizeof(char);

		return ptr;
	}

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

	virtual int Write(const void* inPtr , int inSize )
	{
		::memcpy(Buffer + Pointer , inPtr , inSize);

		Pointer += inSize;

		return inSize;
	}

	virtual int Read(void* outPtr , int inSize )
	{
		::memcpy(outPtr , Buffer + Pointer , inSize);

		Pointer += inSize;

		return inSize;
	}

	int scan(const char* inFormat,...)
	{
		return ::sscanf( Buffer + Pointer , inFormat,(char*)(&inFormat+1));
	}

	long seek(long inP , int inFlg = SEEK_SET)
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

	long tell() const
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

#endif // !defined(AFX_RNEOMEMORY_H__60382F6B_BB62_4B4F_A3F7_70D90CFA4AF2__INCLUDED_)
