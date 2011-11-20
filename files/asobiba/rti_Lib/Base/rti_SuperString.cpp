#include <./Base/rti_SuperString.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************/
//指定された文字列を検索.
//引数				inBase				検索される文字列
//					inBaseLen			検索される文字列の長さ
//					inChallenge			検索する文字列
//					inChallengeLen		検索する文字列の長さ
//					inFindStartPos		trueの場合ヒットした場所の先頭 / falseはヒットした場所の次
//戻り値			場所 / 失敗 -1
/*****************************************************/
int TSuperString::FindString( const char * inBase , int inBaseLen  ,
							  const char * inChallenge , int inChallengeLen ,
							  bool inFindStartPos/*=false*/)
{
	int theLen ;
	int theHit = 0 ;

	for( theLen = 0; theLen < inBaseLen ; theLen++ )
	{
		if (inBase[theLen] == inChallenge[theHit])
		{
			theHit++;
			if (theHit >= inChallengeLen) 
			{//一致.
				//次の場所を返す.
				if (inFindStartPos == false)	return theLen + 1;
				//ヒットした場所の先頭を返す.
				else							return (theLen - inChallengeLen) + 1;
			}
		}
		else
		{	//最初から検査しなおし.
			theHit = 0;
		}
	}
	//見つからない.
	return -1;
}

/*****************************************************/
//次の行へ.
//引数				inBase				検索される文字列
//					inBaseLen			検索される文字列の長さ
//					inRetCode			リターンコード
//戻り値			場所 / 失敗 -1
/*****************************************************/
int TSuperString::NextLine( const char * inBase , int inBaseLen  ,
							const char * inRetCode/*="\r\n"*/ )
{
	return TSuperString::FindString( inBase , inBaseLen  ,
							inRetCode	,	strlen(inRetCode)	,	true );
}


/*****************************************************/
//指定された文字列を検索.
//引数				inBase				検索される文字列
//					inChallenge			検索する文字列
//					inFindStartPos		trueの場合ヒットした場所の先頭 / falseはヒットした場所の次
//戻り値			場所 / 失敗 -1
/*****************************************************/
int TSuperString::FindString( const char * inBase	,
				 const char * inChallenge	,	bool inFindStartPos/*=false*/)
{
	int theLen ;
	int theHit = 0 ;

	for( theLen = 0; inBase[theLen]!='\0' ; theLen++ )
	{
		if (inBase[theLen] == inChallenge[theHit])
		{
			theHit++;
			if ( inChallenge[theHit]=='\0' ) 
			{//一致.
				//次の場所を返す.
				if (inFindStartPos == false)	return theLen + 1;
				//ヒットした場所の先頭を返す.
				else							return (theLen - theHit) + 1;
			}
		}
		else
		{	//最初から検査しなおし.
			theHit = 0;
		}
	}
	//見つからない.
	return -1;
}

/*****************************************************/
//次の行へ.
//引数				inBase				検索される文字列
//					inRetCode			リターンコード
//戻り値			場所 / 失敗 -1
/*****************************************************/
int TSuperString::NextLine( const char * inBase , const char * inRetCode/*="\r\n"*/ )
{
	return TSuperString::FindString( inBase	,	inRetCode	,	false );
}

