// RDebug.cpp: RDebug クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDebug::RDebug()
{

}

RDebug::~RDebug()
{

}

/**
 * MemoryToFile:		メモリをファイルに
 *
 * @param inFilename	ファイル名
 * @param inMemory		メモリ
 * @param inSize		メモリサイズ
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,const void* inMemory , int inSize)
{
	FILE * fp = fopen( inFilename.c_str() ,"wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return;
	}

	fwrite( inMemory  ,sizeof(char) , inSize , fp);

	fclose(fp);
}

/**
 * MemoryToFile:		メモリをファイルに
 *
 * @param inFilename	ファイル名
 * @param inMemory		メモリ
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,const char* inMemory )
{
	RDebug::MemoryToFile(inFilename , inMemory , strlen(inMemory) );
}

/**
 * MemoryToFile:		メモリをファイルに
 *
 * @param inFilename	ファイル名
 * @param inMemory		メモリ
 * @return void 
 */
void RDebug::MemoryToFile(const string inFilename ,string inMemory )
{
	RDebug::MemoryToFile(inFilename , inMemory.c_str() , inMemory.size() );
}

/**
 * FileCheck:			二つのファイルが同じかどうかチェック.
 *
 * @param inSrc			ファイル名1
 * @param inDest		ファイル名2
 * @return void 
 */
void RDebug::FileVerify(const string inSrc,const string inDest)
{
	FILE* src = fopen(inSrc.c_str() , "rb" );
	ASSERT(src);
	FILE* dest = fopen(inDest.c_str() , "rb" );
 	ASSERT(dest);

	string error;

	char* srcBuffer = new char[1024];
	char* destBuffer = new char[1024];
	while(1)
	{
		int srclen = fread( srcBuffer , 1 , 1024 , src);
		int destlen = fread( destBuffer , 1 , 1024 , dest);

		if (srclen == 0 && destlen == 0) break;

		if (srclen != destlen)
		{
			fclose(src);			fclose(dest);
			//サイズ不一致
			ASSERT(0);
			break;
		}
		if ( memcmp(srcBuffer ,  destBuffer ,srclen) != 0)
		{
			fclose(src);			fclose(dest);
			//内容不一致
			ASSERT(0);
			break;
		}
	}
	fclose(src);			fclose(dest);
	delete []  srcBuffer ;
	delete [] destBuffer ;
}
