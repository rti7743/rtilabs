// SusiePluginWapper.h: SusiePluginWapper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_)
#define AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "SusiePlugin.h"

//Susieがロードしたイメージです。
class SusiePluginLoadedPicture
{
public:
	//とりあえず初期化と。
	SusiePluginLoadedPicture();
	//解放と。
	virtual ~SusiePluginLoadedPicture();

	//Bitmap情報の取得.
	const BITMAPINFO* getBitmapInfo();
	//Bitmap自体の取得,
	const void* getBitmap();

	void setPictureInfo(PictureInfo inPictureInfo)
	{
		this->Info = inPictureInfo;
	}
	void setPicture(HLOCAL inBinfo,HLOCAL inBm)
	{
		ASSERT(inBinfo != NULL);
		ASSERT(inBm != NULL);

		ASSERT(this->Binfo == NULL);
		ASSERT(this->Bm == NULL);

		this->Binfo = inBinfo;
		this->Bm = inBm;
	}


private:
	BITMAPINFO*	BitmapInfo;
	void*		Bitmap;

	friend class SusiePluginWapper;
		PictureInfo	Info;			//画像情報.
		HLOCAL		Binfo;			//Susie がここにロードします。
		HLOCAL		Bm;				//Susie がここにロードします。
};


//SusiePluginは素のままではとてもアホでメモリ関係の操作がやりにくいので
//それをラップするクラスです。
class SusiePluginWapper  
{
private:
	SusiePlugin	Plugin;
public:
	SusiePluginWapper();
	virtual ~SusiePluginWapper();

	//必ず呼べ!
	//初期化かされていない場合は RDoNotInitaliseException です。
	//そんな関数ない場合は RNotSupportException です。
	void Create(const string inDllName)
		{		Plugin.Create(inDllName);	}

	//プラグインバージョンの取得
	//エラー RSusieConferException
	string GetPluginVersion() const throw()
		{		return Plugin.GetPluginVersion();	}
	//プラグインコピーライトの取得
	//エラー RSusieConferException
	string GetPluginCopyRight() const throw()
		{		return Plugin.GetPluginCopyRight();	}
	//プラグインが対応している拡張子 代表的な拡張子 ("*.JPG" "*.RGB;*.Q0" など)
	//@inNo	0～
	//ret 形式
	//失敗 ""
	//エラー RSusieConferException
	string GetPluginSupportExt(int inNo) const throw()
		{		return Plugin.GetPluginSupportExt(inNo);	}
	//プラグインが対応しているファイル形式名
	//@inNo	0～
	//ret ファイルタイプ
	//失敗 ""
	//
	//エラー RSusieConferException
	string GetPluginSupportFiletypeName(int inNo) const throw()
		{		return Plugin.GetPluginSupportFiletypeName(inNo);	}

	//サポートされているかどうか調べる
	bool IsSupported(const string inFilename,char* io2KMemory) const ;

	//画像の情報の取得
	//エラー RSusieConferException
	void GetPictureInfo(const string inFilename,SusiePluginLoadedPicture *outPicture) const throw();

	//展開
	//エラー RSusieConferException
	void GetPicture(const string inFilename,
			SusiePluginLoadedPicture *outPicture,SusieCallable* inCallable) const throw();

};

#endif // !defined(AFX_SUSIEPLUGINWAPPER_H__6F5F8567_358E_4C66_82FD_F732C4B8F99B__INCLUDED_)
