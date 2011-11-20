// DisplayDebugPrintf.cpp: DisplayDebugPrintf クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayDebugPrintf.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DisplayDebugPrintf::DisplayDebugPrintf(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

}

DisplayDebugPrintf::~DisplayDebugPrintf()
{

}


static void test()
{
	DisplayDebugPrintf ddp;
}

