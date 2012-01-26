// XLFileUtil.cpp: XLFileUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLFileUtil.h"
#include "XLStringUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XLFileUtil::XLFileUtil()
{

}

XLFileUtil::~XLFileUtil()
{

}


//ファイルが存在するか?
bool XLFileUtil::Exist(const string & inFileName)
{
	const string newStr = XLStringUtil::Replace(inFileName , "/" , "\\");
	return (0xFFFFFFFF != GetFileAttributes(newStr.c_str() ));
}

//ファイルをすべて string に読み込む.
string XLFileUtil::FileToString(const string & inFileName)
{

	FILE * fp = fopen(inFileName.c_str() , "rb");
	//存在しない場合は空
	if (fp == NULL) return "";

	//ケツに持っていって.
	fseek(fp , 0 ,SEEK_END);

	//これでサイズがわかる.
	unsigned long size = ftell(fp);

	//先頭に戻す.
	fseek(fp , 0 ,SEEK_SET);

	//領域を確保して読み込む
	string ret;
	ret.resize(size + 1);
	fread( &ret[0] , 1 , size  , fp);
	ret[size] = '0';	//終端.

	fclose(fp);

	return ret;
}

//inStr を ファイルに書き込む
void XLFileUtil::StringToFile(const string & inStr , const string & inFileName)
{
	FILE * fp = fopen(inFileName.c_str() , "wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return ;
	}

	fwrite( inStr.c_str() , 1, inStr.size()  , fp);

	fclose(fp);
}

//inBuffer を ファイルに書き込む
void XLFileUtil::BufferToFile(const XLBuffer & inBuffer , const string & inFileName)
{
	FILE * fp = fopen(inFileName.c_str() , "wb");
	if (fp == NULL)
	{
		ASSERT(0);
		return ;
	}

	fwrite( &inBuffer[0] , 1, inBuffer.size()  , fp);

	fclose(fp);
}