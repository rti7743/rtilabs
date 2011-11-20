// RMmap.cpp: RMmap クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RMmap.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMmap::RMmap()
{
	this->MapFile = NULL;
	this->MapObject = NULL;
	this->MapMemory = NULL;
}

RMmap::~RMmap()
{
	if (this->MapObject)	Release();
}

void RMmap::Create(DWORD inMappingSize) throw(RWin32Exception)
{
	ASSERT(this->MapFile == NULL);	
	ASSERT(this->MapObject == NULL);
	ASSERT(this->MapMemory == NULL);

	this->MapObject = CreateFileMapping( INVALID_HANDLE_VALUE , NULL, PAGE_READWRITE , 0, inMappingSize	, NULL);
	if (this->MapObject == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "メモリマップの作成に失敗");

	this->MapMemory = MapViewOfFile(this->MapObject , FILE_MAP_ALL_ACCESS , 0, 0, 0);
	if (this->MapMemory == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "メモリマップの関連付けに失敗");
}

void RMmap::Create(const string inFilename ,DWORD inMappingSize, DWORD inOpenFlg) throw(RWin32Exception)
{
	ASSERT(this->MapFile == NULL);
	ASSERT(this->MapObject == NULL);
	ASSERT(this->MapMemory == NULL);

	this->MapFile = CreateFile( inFilename.c_str() ,
								GENERIC_READ | GENERIC_WRITE,
								NULL,NULL,inOpenFlg,
								FILE_FLAG_RANDOM_ACCESS,NULL );
	if (this->MapFile == INVALID_HANDLE_VALUE)
	{
		throw RWin32Exception(EXCEPTIONTRACE + "ファイル" + inFilename + "が開けません");
	}

	this->MapObject = CreateFileMapping( this->MapFile , NULL, PAGE_READWRITE, 0, inMappingSize	, NULL);
	if (this->MapObject == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "メモリマップの作成に失敗");

	this->MapMemory = MapViewOfFile(this->MapObject , FILE_MAP_ALL_ACCESS  , 0, 0, 0);
	if (this->MapMemory == NULL)	throw RWin32Exception(EXCEPTIONTRACE + "メモリマップの関連付けに失敗");
	DWORD err = GetLastError();
	if (err  == ERROR_ALREADY_EXISTS ) throw RWin32Exception(err,EXCEPTIONTRACE + "メモリマップの関連付けに失敗");
}

void RMmap::Release()
{
	ASSERT(this->MapObject != NULL);
	ASSERT(this->MapMemory != NULL);

	UnmapViewOfFile(this->MapMemory);
	CloseHandle(this->MapObject);

	if (this->MapFile)
	{
		CloseHandle(this->MapFile);
	}
	this->MapFile = NULL;
	this->MapObject = NULL;
	this->MapMemory = NULL;
}

void RMmap::Flush(DWORD inFlushSize)
{
	if (this->MapObject)
	{
		if ( !FlushViewOfFile(this->MapMemory,inFlushSize ) ) throw RWin32Exception(EXCEPTIONTRACE + "マッピングファイルのフラッシュに失敗");
	}
}

void RMmap::test()
{
	//4バイトのファイルをマップして 5バイト目を書き込んで、フラッシュしてみる
	{
		RMmap	map;
		RDebug::MemoryToFile("testdata/rmmap1.txt" , "abcd");
		map.Create( "testdata/rmmap1.txt" , 100 );
		char * p = (char*) map.getMemory();
		//読めます?
		ASSERT(p[0] == 'a');
		ASSERT(p[1] == 'b');
		ASSERT(p[2] == 'c');
		//かけます?
		p[4] = 'X';
		ASSERT(p[4] == 'X');
		map.Flush(0);
		map.Release();
		RDebug::FileVerify("testdata/rmmap1.txt" ,"testdata/rmmap1_1.txt" );
	}
}