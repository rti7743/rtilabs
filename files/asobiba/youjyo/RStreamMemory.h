// RStreamMemory.h: RStreamMemory クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAMMEMORY_H__BFE710F6_A2FE_44B8_8B9E_ABFD353E042E__INCLUDED_)
#define AFX_RSTREAMMEMORY_H__BFE710F6_A2FE_44B8_8B9E_ABFD353E042E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RStream.h"

class RStreamMemory : public RStream  
{
public:
	RStreamMemory(char *ioMem,int inSize);
	virtual ~RStreamMemory();

	virtual int Write(const char *inBuffer ,int inBufferSize) ;
	virtual int Read(char *outBuffer ,int inBufferSize) ;
private:
	char *Mem;
	int Size;
	int NowReadSize;
};

#endif // !defined(AFX_RSTREAMMEMORY_H__BFE710F6_A2FE_44B8_8B9E_ABFD353E042E__INCLUDED_)
