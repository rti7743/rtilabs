// RGISSmallDupKill.cpp: RGISSmallDupKill クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallDupKill.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallDupKill::RGISSmallDupKill()
{
	//バッファの初期化.
	memset(Points , sizeof(char*)  , DupKillBufferMax);
	//文字列のときにわざと返す、空のバッファー
	NullString[0] = '\0';

	Count = 0;
}

RGISSmallDupKill::~RGISSmallDupKill()
{

}

bool RGISSmallDupKill::Uniq(const char* inString)
{
	ASSERT(inString != NULL);

	if (inString[0] == '\0') return false;

	for(int i = 0 ; i < Count ; i ++)
	{
		if ( Points[i] == inString ) return false;
	}
	//ないなら追加.
	if (i != DupKillBufferMax)
	{
		Points[i] = inString;
		Count ++;
	}
	return true;
}

