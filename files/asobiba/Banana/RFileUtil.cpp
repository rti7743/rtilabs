// RFileUtil.cpp: RFileUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RFileUtil.h"
#include "RStdioFile.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFileUtil::RFileUtil()
{

}

RFileUtil::~RFileUtil()
{

}

//行数を数える.
int RFileUtil::LineCount(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	int i;
	for( i = 0  ; !file.IsEof() ; i++)
	{
		string r = file.ReadLine();
	}
	return i;
}


void RFileUtil::test()
{
	//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
	//テストルーチンは動かしません
	/*
	int r;
	ASSERT((r = RFileUtil::LineCount("test/01102SK.sal")) == 266);
	ASSERT((r = RFileUtil::LineCount("test/test.cm")) == 3);
	*/
}

