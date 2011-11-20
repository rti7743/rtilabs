// SolvableFileLZH.cpp: SolvableFileLZH クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFileLZH.h"
#include "VirtualPath.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
//そのパスを解決してみる
bool SolvableFileLZH::DoSolvable(string inPath) 
{
	//ネストしているかもしれないので調べる.
	VirtualPath		vp(inPath);
	if (vp.getPathDilimiterCount() == 0) 
	{	//最初なので / を付属
		this->setPath(inPath + "::\\" );
	}
	else
	{
		//最後が \\ でない場合は最後に \\ を補完します。
		if ( MultiString::IsPathToComplement(inPath) )
		{
			inPath += "\\";
		}
		this->setPath(inPath);
	}

	//OK
	return true;
}


void SolvableFileLZH::Test()
{
	SolvableFileLZH sf;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


