// RAlphabetHash.cpp: RAlphabetHash クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RAlphabetHash.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

void RAlphabetHash::test()
{
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == RAlphabetHash("anime") );
	}
	{
		RAlphabetHash	hash;
		hash = "rti";
		ASSERT (hash == RAlphabetHash("rti") );
	}
	{
		RAlphabetHash	hash;
		hash = "a";
		ASSERT (hash == RAlphabetHash("a") );
		ASSERT (hash != RAlphabetHash("anime") );
	}
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == "anime" );
	}
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == "anime" );
	}
}
