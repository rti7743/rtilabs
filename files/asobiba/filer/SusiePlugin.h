// SusiePlugin.h: SusiePlugin クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_)
#define AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RSusieConferException.h"
#include "SusieCallable.h"
#include "RNotSupportException.h"
#include "RAutoLibraryWithException.h"

struct PictureInfo
{
	long left,top;    //画像を展開する位置
	long width;       //画像の幅(pixel)
	long height;      //画像の高さ(pixel)
	WORD x_density;   //画素の水平方向密度
	WORD y_density;   //画素の垂直方向密度
	short colorDepth; //１画素当たりのbit数
	HLOCAL hInfo;     //画像内のテキスト情報
};


class SusiePlugin  
{
private:
	//DLL超簡単に使うためのクラス.
	RAutoLibraryWithException  Lib;

	//int WINAPI GetPluginInfo(int infono,char* iobuf,int inbuflen)
	typedef int (WINAPI *GETPLUGININFO)    (int infono,char* iobuf,int inbuflen);
	GETPLUGININFO ExportGetPluginInfo;

	//int WINAPI IsSupported(LPSTR filename,DWORD dw)
	typedef int (WINAPI *ISUPPORTED)    (char* ioFilename,DWORD inDw);
	ISUPPORTED ExportIsSupported;

	//int WINAPI GetPictureInfo(char* inBuffer,long inLen,unsigned int inFlag,struct PictureInfo *outInfo);
	typedef int (WINAPI *GETPICTUREINFO)    (char* inBuffer,long inLen,unsigned int inFlag,struct PictureInfo *outInfo);
	GETPICTUREINFO ExportGetPictureInfo;

	//int WINAPI GetPicture(char* ioBuffer,long inLen,unsigned int inFlag,HANDLE *outHBInfo,HANDLE *outHBm,FARPROC inPrgressCallback,long inData)
	typedef int (WINAPI *GETPICTURE)    (char* ioBuffer,long inLen,unsigned int inFlag,HANDLE *outHBInfo,HANDLE *outHBm,FARPROC inPrgressCallback,long inData);
	GETPICTURE ExportGetPicture;

	//Susieの戻り値の処理
	//エラーは RSusieConferException
	void SusieResult(int inRetCode,const char* inFuncName)  const throw();

public:
	SusiePlugin() ;
	virtual ~SusiePlugin();

	//必ず呼べ!
	//初期化かされていない場合は RDoNotInitaliseException です。
	//そんな関数ない場合は RNotSupportException です。
	void Create(const string inDllName);

	//プラグインバージョンの取得
	//エラー RSusieConferException
	string GetPluginVersion() const throw();
	//プラグインコピーライトの取得
	//エラー RSusieConferException
	string GetPluginCopyRight() const throw();
	//プラグインが対応している拡張子 代表的な拡張子 ("*.JPG" "*.RGB;*.Q0" など)
	//@inNo	0～
	//ret 形式
	//失敗 ""
	//エラー RSusieConferException
	string GetPluginSupportExt(int inNo) const throw();
	//プラグインが対応しているファイル形式名
	//@inNo	0～
	//ret ファイルタイプ
	//失敗 ""
	//
	//エラー RSusieConferException
	string GetPluginSupportFiletypeName(int inNo) const throw();


	//サポートされているかどうか調べる
	bool IsSupported(char* ioFilename,char* io2KMemory) const ;

	//画像の情報の取得
	//エラー RSusieConferException
	void GetPictureInfo(char* ioFilename,PictureInfo *outinfo) const throw();

	//展開
	//エラー RSusieConferException
	void GetPicture(char* ioFilename,
			HLOCAL *outBinfo, HLOCAL *outBm,
			SusieCallable* inCallable) const throw();
};

#endif // !defined(AFX_SUSIEPLUGIN_H__4835897D_E8A8_45CB_9C99_C796974CFDFF__INCLUDED_)
