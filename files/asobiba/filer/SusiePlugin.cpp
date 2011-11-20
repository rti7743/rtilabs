// SusiePlugin.cpp: SusiePlugin クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePlugin.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SusiePlugin::SusiePlugin()
{
}
SusiePlugin::~SusiePlugin()
{
}

//初期化かされていない場合は RDoNotInitaliseException です。
//そんな関数ない場合は RNotSupportException です。
void SusiePlugin::Create(const string inDllName) throw()
{
	Lib.Create(inDllName.c_str() );
	ExportGetPluginInfo = (GETPLUGININFO) Lib.GetProcAddress("GetPluginInfo");
	ExportIsSupported = (ISUPPORTED) Lib.GetProcAddress("IsSupported");
	ExportGetPictureInfo = (GETPICTUREINFO) Lib.GetProcAddress("GetPictureInfo");
	ExportGetPicture = (GETPICTURE) Lib.GetProcAddress("GetPicture");
}


//プラグインバージョンの取得
//エラー RSusieConferException
string SusiePlugin::GetPluginVersion() const throw()
{
	char buf[MAX_PATH+4];	//多めに確保
	int ret;
	try
	{
		ret = ExportGetPluginInfo(0 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"プラグインが例外を飛ばしました");	}
	buf[ret] = '\0';

	return buf;
}
//プラグインコピーライトの取得
string SusiePlugin::GetPluginCopyRight() const throw()
{
	char buf[MAX_PATH+4];	//多めに確保
	int ret;
	try
	{
		ret = ExportGetPluginInfo(1 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"プラグインが例外を飛ばしました");	}
	buf[ret] = '\0';

	return buf;
}
//プラグインが対応している拡張子 代表的な拡張子 ("*.JPG" "*.RGB;*.Q0" など)
//@inNo	0～
//ret 形式
//失敗 ""
string SusiePlugin::GetPluginSupportExt(int inNo) const throw()
{
	char buf[MAX_PATH+4];	//多めに確保
	int ret;
	try
	{
		ret = ExportGetPluginInfo(2 + inNo*2 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"プラグインが例外を飛ばしました");	}
	buf[ret] = '\0';

	return buf;
}

//プラグインが対応しているファイル形式名
//@inNo	0～
//ret ファイルタイプ
//失敗 ""
string SusiePlugin::GetPluginSupportFiletypeName(int inNo) const throw()
{
	char buf[MAX_PATH+4];	//多めに確保
	int ret;
	try
	{
		ret = ExportGetPluginInfo(3 + inNo*2 , buf , MAX_PATH);
	}
	catch(...){	throw RSusieConferException(0,"プラグインが例外を飛ばしました");	}
	buf[ret] = '\0';

	return buf;
}




//サポートされているかどうか調べる
bool SusiePlugin::IsSupported(char* ioFilename,char* io2KMemory) const
{
	int ret;
	try
	{
		ret = ExportIsSupported(ioFilename , (DWORD)io2KMemory);
	}
	catch(...){	return false;	}	//例外ではないので注意

	return (ret != 0);
}


//画像の情報の取得
//エラー RSusieConferException
void SusiePlugin::GetPictureInfo(char* ioFilename,	PictureInfo *outinfo) const throw()
{

	TRACE("%s\n" , GetPluginCopyRight().c_str() );
	int ret;

	try
	{
		ret = ExportGetPictureInfo(ioFilename,0 ,0	 , outinfo );
	}
	catch(...){	throw RSusieConferException(0,"プラグインが例外を飛ばしました");	}
	if ( ret == 2 ) ret = 0;	//SUSIEのボケのバグ対策.
	SusieResult( ret ,"GetPictureInfo");
}


//展開
//エラー RSusieConferException
void SusiePlugin::GetPicture(char* ioFilename,
			HLOCAL *outBinfo, HLOCAL *outBm,
			SusieCallable* inCallable) const throw()
{
	int ret;

	//ロード
	try
	{
		ret = ExportGetPicture(ioFilename,0,0 , 
			outBinfo , outBm ,	(FARPROC)SusieCallable::ProgressCallback , (DWORD)inCallable);
	}
	catch(...)
	{
		throw RSusieConferException(0,"プラグインが例外を飛ばしました");	
	}
	if ( ret == 2 ) ret = 0;	//SUSIEのボケのバグ対策.

	SusieResult( ret , "GetPicture");
}


//Susieの戻り値の処理
//エラー RSusieConferException
void SusiePlugin::SusieResult(int inRetCode,const char* inFuncName) const throw()
{
	//正常終了
	if (inRetCode == 0) return ;

	switch(inRetCode)
	{
	case -1:
		throw RSusieConferException(inRetCode,"%s: その機能はインプリメントされていない", inFuncName);
		break;
	case 1:
		throw RSusieConferException(inRetCode,"%s: コールバック関数が非0を返したので展開を中止した", inFuncName);
		break;
	case 2:
		throw RSusieConferException(inRetCode,"%s: 未知のフォーマット", inFuncName);
		break;
	case 3:
		throw RSusieConferException(inRetCode,"%s: データが壊れている", inFuncName);
		break;
	case 4:
		throw RSusieConferException(inRetCode,"%s: メモリーが確保出来ない", inFuncName);
		break;
	case 5:
		throw RSusieConferException(inRetCode,"%s: メモリーエラー（Lock出来ない、等）", inFuncName);
		break;
	case 6:
		throw RSusieConferException(inRetCode,"%s: ファイルリードエラー", inFuncName);
		break;
	case 8:
		throw RSusieConferException(inRetCode,"%s: 内部エラー", inFuncName);
		break;
	default:
		throw RSusieConferException(inRetCode,"%s: 定義外エラー", inFuncName);
		break;
	}
}

