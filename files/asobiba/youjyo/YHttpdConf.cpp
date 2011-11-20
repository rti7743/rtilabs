// YHttpdConf.cpp: YHttpdConf クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YHttpdConf.h"
#include "RConfigFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YHttpdConf::YHttpdConf()
{

}

YHttpdConf::~YHttpdConf()
{

}

//設定ファイル.
void YHttpdConf::Load(const string inFilename)
{
	RConfigFile	Setting;

	//設定をロードするのですよー.
	Setting.Load(inFilename);

	/*ChangeLog.txt:2003/01/24	httpd.conf で ListenPort が指定されたとしても無視していたので修正.
	*/
	//listen するポート
	this->ListenPort	= Setting.LoadOrDefault("ListenPort" , 80 );
	//サーバの名前
	this->ServerName	= Setting.LoadOrDefault("ServerName" , string("127.0.0.1") );

	//ドキュメントルート
	char currentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH , currentDirectory);
	this->DocumentRoot	= Setting.LoadOrDefault("DocumentRoot" , string(currentDirectory)  + "/htdocs" );

	this->Version = "ょぅι゛ょ1.0α3+ (02/02/22)";
}

/**
 * getInstance:		Singletonインターフェースの作成.
 *
 * @return YHttpdConf* 
 */
YHttpdConf* YHttpdConf::getInstance()
{
	static YHttpdConf p;
	return &p;
}

