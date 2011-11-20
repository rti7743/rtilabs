// RToken.cpp: RToken クラスのインプリメンテーション
//

//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RToken.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

void RToken::test()
{
	{
		RToken	t;
		t.setToken("/aaa/bbb/ccc/ddd" , '/' );

		ASSERT( strcmp( t.nextToken() , "") == 0 );
		ASSERT( strcmp( t.nextToken() , "aaa") == 0 );
		ASSERT( strcmp( t.nextToken() , "bbb") == 0 );
		ASSERT( strcmp( t.nextToken() , "ccc") == 0 );
		ASSERT( strcmp( t.nextToken() , "ddd") == 0 );
		ASSERT( t.nextToken() == NULL );
	}
	{
		RToken	t;
		t.setToken("/aaa/" , '/' );

		ASSERT( strcmp( t.nextToken() , "") == 0 );
		ASSERT( strcmp( t.nextToken() , "aaa") == 0 );
		ASSERT( t.nextToken() == NULL );
	}
	{
		RToken	t;
		t.setToken("/" , '/' );

		ASSERT( strcmp( t.nextToken() , "") == 0 );
		ASSERT( t.nextToken() == NULL );
	}
	{
		RToken	t;
		t.setToken("/aaa" , '/' );

		ASSERT( strcmp( t.nextToken() , "") == 0 );
		ASSERT( strcmp( t.nextToken() , "aaa") == 0 );
		ASSERT( t.nextToken() == NULL );
	}
	puts("RToken::test OK");
}
