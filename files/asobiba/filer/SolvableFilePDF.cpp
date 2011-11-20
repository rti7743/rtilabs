// SolvableFilePDF.cpp: SolvableFilePDF クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFilePDF.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvableFilePDF::DoSolvable(string inPath) 
{
	this->setPath(inPath);

	//OK
	return true;
}


void SolvableFilePDF::Test()
{
	SolvableFilePDF sf;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


