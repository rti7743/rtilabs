// BConf.cpp: BConf クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BConf.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BConf::BConf()
{
	this->CurrentDirectory = this->CommonCurrentDirectory();
}

BConf::~BConf()
{
}



BConf* BConf::getConf()
{
	static BConf conf;

	return &conf;
}

//カレントディレクトリの取得.
string BConf::CommonCurrentDirectory() const
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
