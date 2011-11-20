// RDiskUtil.cpp: RDiskUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDiskUtil.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDiskUtil::RDiskUtil()
{

}

RDiskUtil::~RDiskUtil()
{

}

//ファイルが存在するか?
bool RDiskUtil::Exist(const string & inFileName)
{
	return (0xFFFFFFFF != GetFileAttributes(inFileName.c_str() ));
}


//カレントディレクトリの取得.
string RDiskUtil::getCurrentDirectory() 
{
#ifndef _WIN32_WCE
	char buffer[MAX_PATH];
	if ( ::GetCurrentDirectory(MAX_PATH, buffer) == 0) return "";
	return string(buffer) + "\\";
#else
	//WINCE にカレントディレクトリという概念がないため、
	//プロセスのパスをパースする.
	MY_USES_CONVERSION;

	unsigned short buffer[MAX_PATH];
	if ( ::GetModuleFileName(NULL , buffer , MAX_PATH) == 0) return "";
	string dir = _W2A(buffer);
	int pos = dir.rfind('\\');
	if (pos == -1) return dir;

	return dir.substr(0, pos )+ "\\";
#endif
}

