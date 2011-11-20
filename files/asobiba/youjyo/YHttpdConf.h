// YHttpdConf.h: YHttpdConf クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_)
#define AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//サーバの設定ファイルです。
//singleton.
//複数のスレッドから読み出しますが、　読み込みしかしないのでロックなしです!!
class YHttpdConf  
{
private:	//singleton,
	YHttpdConf();
public:
	virtual ~YHttpdConf();

	void Load(const string inFilename);

	//ListenPort.
	int getListenPort() const
	{
		return this->ListenPort;
	}
	//サーバ名.
	const string getServerName() const
	{
		return this->ServerName;
	}
	//ドキュメントルートのパス.
	const string getDocumentRoot() const
	{
		return this->DocumentRoot;
	}

	const string getVersion() const
	{
		return this->Version;
	}


	/**
	 * getInstance:		Singletonインターフェースの作成.
	 *
	 * @return YHttpdConf* 
	 */
	static YHttpdConf* getInstance();
private:
	int		ListenPort;
	string	ServerName;
	string	DocumentRoot;
	string	Version;
};

#endif // !defined(AFX_YHTTPDCONF_H__19304D6A_1592_44D0_B408_50599C015609__INCLUDED_)
