// SolvablePathDebug.cpp: SolvablePathDebug クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathDebug.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//そのパスを解決してみる
//解決できた場合は解決した結果のパス 
//inPath www.yahoo.co.jp ---> return http://www.yahoo.co.jp/ などとなる
//解決できなかった場合は "" を返す.
bool SolvablePathDebug::DoSolvable(string inPath) 
{
	if (inPath == "debug") return true;

	return false;
}

