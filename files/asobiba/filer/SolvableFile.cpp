// SolvableFile.cpp: SolvableFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvableFile.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//一つディレクトリを上がる
string SolvableFile::UpDirectory() const 
{
	string path = getPath();

	const int lastpos = MultiString::getLastDirectoryPos(path);
	string ret = path.substr( 0 , lastpos + 1) ;
	return ret;
}
