// SolvableFileBitmap.cpp: SolvableFileBitmap クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileBitmap.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvableFileBitmap::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}

void SolvableFileBitmap::Test()
{
	SolvableFileBitmap sf;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


