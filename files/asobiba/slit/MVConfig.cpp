// MVConfig.cpp: MVConfig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVConfig.h"
#include "RConfigFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVConfig::MVConfig()
{
	this->CurrentDirectory = CommonCurrentDirectory();
}

MVConfig::~MVConfig()
{

}


MVConfig* MVConfig::getInstance()
{
	static MVConfig conf;
	return &conf;
}

//カレントディレクトリの取得.
string MVConfig::CommonCurrentDirectory() const
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

void MVConfig::Load()
{
	RConfigFile confFile;
	confFile.Load(CurrentDirectory + "slit.conf");

	//最後に開いたファイル名
	this->LastFilename = confFile.LoadOrDefault("LastFilename" , string("") );
	//最後に開いたページ
	this->LastPage = confFile.LoadOrDefault("LastPage" , 0 );

	//閉じる前の状態の記録
	this->LastMemory	= confFile.LoadOrDefault("LastMemory" ,  true );
	//ジャストフィット
	this->JustFit	= confFile.LoadOrDefault("JustFit" ,  false );
	//スクロールカウント
	this->ScrollCount	= confFile.LoadOrDefault("ScrollCount" ,  2 );
	//スクロールタイプ
	this->ScrollType	= (SCROLL_TYPE) confFile.LoadOrDefault("ScrollType" ,  0 );
	
}

void MVConfig::SavePage(const string & inFilename , int inPage)
{
	if (this->LastMemory)
	{
		this->LastFilename		=	inFilename;
		this->LastPage			=	inPage;
	}
	else
	{
		this->LastFilename		=	"";
		this->LastPage			=	0;
	}
}

void MVConfig::setFlg(bool isLastMemory ,bool isJustFit,int inScrollCount , SCROLL_TYPE inScrollType)
{
	this->LastMemory	= isLastMemory;
	this->JustFit		= isJustFit;
	this->ScrollCount	= inScrollCount;
	this->ScrollType	= inScrollType;
}


void MVConfig::Save()
{
	RConfigFile confFile;
	confFile.Load(CurrentDirectory + "slit.conf");

	confFile.Upsert("LastFilename" , this->LastFilename);
	confFile.Upsert("LastPage" , this->LastPage);
	confFile.Upsert("LastMemory" , this->LastMemory);
	confFile.Upsert("JustFit" , this->JustFit);
	confFile.Upsert("ScrollCount" , this->ScrollCount);
	confFile.Upsert("ScrollType" , (int)this->ScrollType);
	


	confFile.Save(CurrentDirectory + "slit.conf");

}
