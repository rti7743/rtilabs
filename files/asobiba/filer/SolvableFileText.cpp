// SolvableFileText.cpp: SolvableFileText クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileText.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvableFileText::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}

void SolvableFileText::Test()
{
	SolvableFileText sf;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


