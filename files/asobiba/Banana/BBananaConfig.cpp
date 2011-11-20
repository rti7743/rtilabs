// BBananaConfig.cpp: BBananaConfig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BBananaConfig.h"
#include "RConfigFile.h"
#include "BConf.h"
#include "RGraphics.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BBananaConfig::BBananaConfig()
{

}

BBananaConfig::~BBananaConfig()
{

}


BBananaConfig* BBananaConfig::getInstance()
{
	static BBananaConfig conf;
	return &conf;
}

void BBananaConfig::Load()
{
	string currentDirectory = BConf::getConf()->getCurrentDirectory();

	RConfigFile confFile;
	confFile.Load(currentDirectory + "banana.conf");

	//ユーザ定義のデータが入っているディレクトリ
	this->UserDataDirectory	= confFile.LoadOrDefault("UserDataDirectory" , currentDirectory + "userdata\\" );
	//MAPファイル
	this->MapFilename	= confFile.LoadOrDefault("MapFilename" , currentDirectory + "dummy.map" );

	this->Point.x	= confFile.LoadOrDefault("X" ,  51286263L );
	this->Point.y	= confFile.LoadOrDefault("Y" ,  4279274433L );
	this->Scale		= confFile.LoadOrDefault("Scale" , 3 );
	this->Deg		= confFile.LoadOrDefault("Deg" , 0 );
	this->RotateFlg	= confFile.LoadOrDefault("RotateFlg" , true );

	this->Color[COLOR_MOJI]				= confFile.LoadOrDefault("COLOR_MOJI" ,				(RGB24TOBGB16(RGB(255,255,255 ))) );
	this->Color[COLOR_CAPTION]			= confFile.LoadOrDefault("COLOR_CAPTION" ,			(RGB24TOBGB16(RGB(0,0,0 ))) );
	this->Color[COLOR_PEN1]				= confFile.LoadOrDefault("COLOR_PEN1" ,				(RGB24TOBGB16(RGB(255,0,0))) );
	this->Color[COLOR_PEN2]				= confFile.LoadOrDefault("COLOR_PEN2" ,				(RGB24TOBGB16(RGB(0,255,0))) );
	this->Color[COLOR_PEN3]				= confFile.LoadOrDefault("COLOR_PEN3" , 			(RGB24TOBGB16(RGB(0,0,255))) );
	this->Color[COLOR_PEN4]				= confFile.LoadOrDefault("COLOR_PEN4" , 			(RGB24TOBGB16(RGB(255,255,0))) );
	this->Color[COLOR_PEN5]				= confFile.LoadOrDefault("COLOR_PEN5" , 			(RGB24TOBGB16(RGB(0,255,255))) );
	this->Color[COLOR_PEN6]				= confFile.LoadOrDefault("COLOR_PEN6" , 			(RGB24TOBGB16(RGB(255,255,102))) );
	this->Color[COLOR_TRACKLOG]			= confFile.LoadOrDefault("COLOR_TRACKLOG" ,			(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_TATEMORO]			= confFile.LoadOrDefault("COLOR_TATEMORO" ,			(RGB24TOBGB16(RGB(30,0,0 ))) );
	this->Color[COLOR_JYOUTI]			= confFile.LoadOrDefault("COLOR_JYOUTI" ,			(RGB24TOBGB16(RGB(0,10,0 ))) );
	this->Color[COLOR_TEIENRO]			= confFile.LoadOrDefault("COLOR_TEIENRO" ,			(RGB24TOBGB16(RGB(191,191,191 ))) );
	this->Color[COLOR_ISHIDAN]			= confFile.LoadOrDefault("COLOR_ISHIDAN" ,			(RGB24TOBGB16(RGB(191,191,191 ))) );
	this->Color[COLOR_DOURO]			= confFile.LoadOrDefault("COLOR_DOURO" ,			(RGB24TOBGB16(RGB(191,191,191 ))) );
	this->Color[COLOR_KOUSOKU_DOURO]	= confFile.LoadOrDefault("COLOR_KOUSOKU_DOURO" ,	(RGB24TOBGB16(RGB(0,255,0 ))) );
	this->Color[COLOR_EKI]				= confFile.LoadOrDefault("COLOR_EKI" ,				(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_TETUDO_JR]		= confFile.LoadOrDefault("COLOR_TETUDO_JR" ,		(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_TETUDO]			= confFile.LoadOrDefault("COLOR_TETUDO" ,			(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_ROMENDENSYA]		= confFile.LoadOrDefault("COLOR_ROMENDENSYA" ,		(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_TIKATETU]			= confFile.LoadOrDefault("COLOR_TIKATETU" ,			(RGB24TOBGB16(RGB(0,128,0 ))) );
	this->Color[COLOR_TETUDOU_SONOTA]	= confFile.LoadOrDefault("COLOR_TETUDOU_SONOTA" ,	(RGB24TOBGB16(RGB(255,0,0 ))) );
	this->Color[COLOR_MIZU]				= confFile.LoadOrDefault("COLOR_MIZU" ,				(RGB24TOBGB16(RGB(0,0,255 ))) );
	this->Color[COLOR_KOUDO200]			= confFile.LoadOrDefault("COLOR_KOUDO200" ,			(RGB24TOBGB16(RGB(255,255,211))) );
	this->Color[COLOR_KOUDO400]			= confFile.LoadOrDefault("COLOR_KOUDO400" ,			(RGB24TOBGB16(RGB(219,219,183 ))) );
	this->Color[COLOR_KOUDO600]			= confFile.LoadOrDefault("COLOR_KOUDO600" ,			(RGB24TOBGB16(RGB(211,204,193 ))) );
	this->Color[COLOR_KOUDO800]			= confFile.LoadOrDefault("COLOR_KOUDO800" ,			(RGB24TOBGB16(RGB(194,255,134 ))) );
	this->Color[COLOR_KOUDO1000]		= confFile.LoadOrDefault("COLOR_KOUDO1000" ,		(RGB24TOBGB16(RGB(146,192,0 ))) );
	this->Color[COLOR_KOUDO1500]		= confFile.LoadOrDefault("COLOR_KOUDO1500" ,		(RGB24TOBGB16(RGB(140,192,108 ))) );
	this->Color[COLOR_KOUDO2000]		= confFile.LoadOrDefault("COLOR_KOUDO2000" ,		(RGB24TOBGB16(RGB(130,170,0 ))) );
	this->Color[COLOR_KOUDO2500]		= confFile.LoadOrDefault("COLOR_KOUDO2500" ,		(RGB24TOBGB16(RGB(77,166,25 ))) );
	this->Color[COLOR_KOUDO3000]		= confFile.LoadOrDefault("COLOR_KOUDO3000" ,		(RGB24TOBGB16(RGB(144,63,1 ))) );
	this->Color[COLOR_KOUDO4000]		= confFile.LoadOrDefault("COLOR_KOUDO4000" ,		(RGB24TOBGB16(RGB(170,0,0 ))) );
	this->Color[COLOR_KOUDO5000]		= confFile.LoadOrDefault("COLOR_KOUDO5000" ,		(RGB24TOBGB16(RGB(170,0,0 ))) );
	this->Color[COLOR_KOUDOOVER]		= confFile.LoadOrDefault("COLOR_KOUDOOVER" ,		(RGB24TOBGB16(RGB(170,0,0 ))) );

}

void BBananaConfig::UpMapDirectory(const string & inMapFilename , const string & inUserDataDirectory)
{
	this->MapFilename		=	inMapFilename;
	this->UserDataDirectory	=	inUserDataDirectory;
}

void BBananaConfig::UpNowPoint(const LPOINT & inNowXY , int inScale , int inDeg)
{
	this->Point		= inNowXY;
	this->Scale		= inScale;
	this->Deg		= inDeg;
}

void BBananaConfig::Save()
{
	string currentDirectory = BConf::getConf()->getCurrentDirectory();

	RConfigFile confFile;
	confFile.Load(currentDirectory + "banana.conf");

	confFile.Upsert("MapFilename" , this->MapFilename);
	confFile.Upsert("UserDataDirectory" , this->UserDataDirectory);
	confFile.Upsert("X" , this->Point.x);
	confFile.Upsert("Y" , this->Point.y);
	confFile.Upsert("Scale" , this->Scale);
	confFile.Upsert("Deg" , this->Deg);
	confFile.Upsert("RotateFlg" , this->RotateFlg);

	confFile.Upsert("COLOR_MOJI" , num2str(this->Color[COLOR_MOJI]) );
	confFile.Upsert("COLOR_CAPTION" , num2str(this->Color[COLOR_CAPTION]) );
	confFile.Upsert("COLOR_PEN1" , num2str(this->Color[COLOR_PEN1]) );
	confFile.Upsert("COLOR_PEN2" , num2str(this->Color[COLOR_PEN2]) );
	confFile.Upsert("COLOR_PEN3" , num2str(this->Color[COLOR_PEN3]) );
	confFile.Upsert("COLOR_PEN4" , num2str(this->Color[COLOR_PEN4]) );
	confFile.Upsert("COLOR_PEN5" , num2str(this->Color[COLOR_PEN5]) );
	confFile.Upsert("COLOR_PEN6" , num2str(this->Color[COLOR_PEN6]) );
	confFile.Upsert("COLOR_TRACKLOG" , num2str(this->Color[COLOR_TRACKLOG]) );
	confFile.Upsert("COLOR_TATEMORO" , num2str(this->Color[COLOR_TATEMORO]) );
	confFile.Upsert("COLOR_JYOUTI" , num2str(this->Color[COLOR_JYOUTI]) );
	confFile.Upsert("COLOR_TEIENRO" , num2str(this->Color[COLOR_TEIENRO]) );
	confFile.Upsert("COLOR_ISHIDAN" , num2str(this->Color[COLOR_ISHIDAN]) );
	confFile.Upsert("COLOR_DOURO" , num2str(this->Color[COLOR_DOURO]) );
	confFile.Upsert("COLOR_KOUSOKU_DOURO" , num2str(this->Color[COLOR_KOUSOKU_DOURO]) );
	confFile.Upsert("COLOR_EKI" , num2str(this->Color[COLOR_EKI]) );
	confFile.Upsert("COLOR_TETUDO_JR" , num2str(this->Color[COLOR_TETUDO_JR]) );
	confFile.Upsert("COLOR_TETUDO" , num2str(this->Color[COLOR_TETUDO]) );
	confFile.Upsert("COLOR_ROMENDENSYA" , num2str(this->Color[COLOR_ROMENDENSYA]) );
	confFile.Upsert("COLOR_TIKATETU" , num2str(this->Color[COLOR_TIKATETU]) );
	confFile.Upsert("COLOR_TETUDOU_SONOTA" , num2str(this->Color[COLOR_TETUDOU_SONOTA]) );
	confFile.Upsert("COLOR_MIZU" , num2str(this->Color[COLOR_MIZU]) );
	confFile.Upsert("COLOR_KOUDO200" , num2str(this->Color[COLOR_KOUDO200]) );
	confFile.Upsert("COLOR_KOUDO400" , num2str(this->Color[COLOR_KOUDO400]) );
	confFile.Upsert("COLOR_KOUDO600" , num2str(this->Color[COLOR_KOUDO600]) );
	confFile.Upsert("COLOR_KOUDO800" , num2str(this->Color[COLOR_KOUDO800]) );
	confFile.Upsert("COLOR_KOUDO1000" , num2str(this->Color[COLOR_KOUDO1000]) );
	confFile.Upsert("COLOR_KOUDO1500" , num2str(this->Color[COLOR_KOUDO1500]) );
	confFile.Upsert("COLOR_KOUDO2000" , num2str(this->Color[COLOR_KOUDO2000]) );
	confFile.Upsert("COLOR_KOUDO2500" , num2str(this->Color[COLOR_KOUDO2500]) );

	confFile.Upsert("COLOR_KOUDO3000" , num2str(this->Color[COLOR_KOUDO3000]) );
	confFile.Upsert("COLOR_KOUDO4000" , num2str(this->Color[COLOR_KOUDO4000]) );
	confFile.Upsert("COLOR_KOUDO5000" , num2str(this->Color[COLOR_KOUDO5000]) );
	confFile.Upsert("COLOR_KOUDOOVER" , num2str(this->Color[COLOR_KOUDOOVER]) );
	confFile.Save(currentDirectory + "banana.conf");
}
