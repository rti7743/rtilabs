// RStreamMemory.cpp: RStreamMemory クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RStreamMemory.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RStreamMemory::RStreamMemory(char *ioMem,int inSize)
{
	Mem = ioMem;
	Size = inSize;
	NowReadSize = 0;
}

RStreamMemory::~RStreamMemory()
{

}

int RStreamMemory::Write(const char *inBuffer ,int inBufferSize) 
{
	return 0;
}

int RStreamMemory::Read(char *outBuffer ,int inBufferSize) 
{
	return 0;
}

