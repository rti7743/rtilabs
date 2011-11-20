// RArchiveLha.cpp: RArchiveLha クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveLha.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RArchiveLha::RArchiveLha()
{
}

RArchiveLha::~RArchiveLha()
{

}

//!初期化できなかったら RDoNotInitaliseException
//!関数が読み込めなかったら RNotSupportException
void RArchiveLha::Create() throw()
{
	Lib.Create("UnLha32.dll");

	//お礼にたっぷり DLL からインポートするにゃん

	//WORD WINAPI UnlhaGetVersion(VOID)
	UnlhaGetVersion = (UNLHAGETVERSION) Lib.GetProcAddress("UnlhaGetVersion");

	//BOOL WINAPI UnlhaGetRunning(VOID)
	UnlhaGetRunning = (UNLHAGETRUNNING) Lib.GetProcAddress("UnlhaGetRunning");

	//BOOL WINAPI UnlhaGetBackGroundMode(VOID)
	UnlhaGetBackGroundMode = (UNLHAGETBACKGROUNDMODE) Lib.GetProcAddress("UnlhaGetBackGroundMode");

	//BOOL WINAPI UnlhaSetBackGroundMode(const BOOL _bBackGroundMode)
	UnlhaSetBackGroundMode = (UNLHASETBACKGROUNDMODE) Lib.GetProcAddress("UnlhaSetBackGroundMode");

	//BOOL WINAPI UnlhaGetCursorMode(VOID)
	UnlhaGetCursorMode = (UNGETCURSORMODE) Lib.GetProcAddress("UnlhaGetCursorMode");

	//BOOL WINAPI UnlhaSetCursorMode(const BOOL _CursorMode)
	UnlhaSetCursorMode = (UNLHASETCOUSORMODE) Lib.GetProcAddress("UnlhaSetCursorMode");

	//WORD WINAPI UnlhaGetCursorInterval(VOID)
	UnlhaGetCursorInterval = (UNLHAGETCURSORINTERNAL) Lib.GetProcAddress("UnlhaGetCursorInterval");

	//BOOL WINAPI UnlhaSetCursorInterval(const WORD _Interval)
	UnlhaSetCursorInterval = (UNLHASETCURSORINTERNAL) Lib.GetProcAddress("UnlhaSetCursorInterval");

	//int WINAPI Unlha(const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize)
	Unlha = (UNLHA) Lib.GetProcAddress("Unlha");

	//BOOL WINAPI UnlhaCheckArchive(LPCSTR _szFileName,	const int _iMode)
	UnlhaCheckArchive = (UNLHACHECKARCHIVE) Lib.GetProcAddress("UnlhaCheckArchive");

	//int WINAPI UnlhaGetFileCount(LPCSTR _szArcFile)
	UnlhaGetFileCount = (UNLHAGETFILECOUNT) Lib.GetProcAddress("UnlhaGetFileCount");

	//BOOL WINAPI UnlhaConfigDialog(const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode)
	UnlhaConfigDialog = (UNLHACONFIGDIALOG) Lib.GetProcAddress("UnlhaConfigDialog");

	//HARC WINAPI UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode)
	UnlhaOpenArchive = (UNLHAOPENARCHIVE) Lib.GetProcAddress("UnlhaOpenArchive");

	//int WINAPI _export UnlhaCloseArchive(HARC _harc)
	UnlhaCloseArchive = (UNLHACLOSEARCHIVE) Lib.GetProcAddress("UnlhaCloseArchive");

	//int WINAPI UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo)
	UnlhaFindFirst = (UNLHAFINDFIRST) Lib.GetProcAddress("UnlhaFindFirst");

	//int WINAPI UnlhaFindNext(HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo)
	UnlhaFindNext = (UNLHAFINDNEXT) Lib.GetProcAddress("UnlhaFindNext");
}

//現在のバージョンを返します。
//@return			現在のバージョン	  1 -> Version 0.01
//										100 -> Version 1.00
WORD RArchiveLha::getVersion() 
{
	return UnlhaGetVersion();
}

//現在、動作中か否かを得ます。
bool RArchiveLha::IsRunning() 
{
	return Btob( UnlhaGetRunning() );
}

//処理の実行
//@inHWND		関連付けるWindowHandle;
//@inCmdLine	コマンドの文字列。 
//@outOutput	結果を返すためのバッファ。LHA.EXE がコン
//				ソールに表示するのと同様のものが，このバッファに格納さ
//				れます。
//@inSize		バッファのサイズ。返す結果のサイズより指定
//				されたサイズが小さい場合は指定サイズまで出力される。
//				現バージョンでは 64KB までの制限がある。
//!RArchiveException
void RArchiveLha::Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize) throw()
{
	int ret = Unlha(inHWND,inCmdLine,outOutput,inSize);
	if (ret != 0)
	{
		throw RArchiveException(ret , "RArchiveLha::Commandでエラーが発生しました。" );
	}
}

//ファイルが正しいアーカイヴであるかどうかチェック
//@inFilename	ファイル名
//@inMode		チェックモード
//@return		正しい true
bool RArchiveLha::CheckArchive(const char* inFilename,const int inMode) 
{
	return Btob( UnlhaCheckArchive(inFilename,inMode) );
}

//指定された書庫ファイルに格納されているファイル数を得ます。
//@inFilename	ファイル名
//@return		ファイル数
int	RArchiveLha::GetFileCount(const char* inCmdLine) 
{
	return UnlhaGetFileCount(inCmdLine);
}

//アーカイヴを開く
//@inHWND		関連付けるWindow
//@inFilename	対象ファイル名
//@inMode		開くときに使うオプション
//@return		アーカイヴハンドル
//!RIOCannotOpenException	開けません.
HARC RArchiveLha::OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode)  throw()
{
	HARC handle = UnlhaOpenArchive(inHWND,inFilename,inMode);
	if (handle == NULL)
		throw RIOCannotOpenException("RArchiveLha::アーカイヴ %s を開くことができませんでした",inFilename);
	return handle;
}

//OpenArchiveで開いたアーカイヴをクローズする.
//@inHandle		アーカイヴハンドル
//!RIOCannotCloseException	開けません.
void RArchiveLha::CloseArchive(HARC inHandle) 
{
	if ( UnlhaCloseArchive(inHandle) != 0)
		throw RIOCannotCloseException("RArchiveLha::クローズに失敗しました");
}

//ファイル検索を開始
//ioHandle		OpenArchiveで開いたハンドル
//@inWildName	検索するファイル名　ワイルドカードも（・∀・）ｲｲ！
//@outSubInfo	結果
//@return		正常			true
//				検索終了		false
//失敗	RArchiveException
bool RArchiveLha::FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo)  throw()
{
	int ret = UnlhaFindFirst(ioHandle,inWildName,outSubInfo) ;
	switch(ret)
	{
	case 0:
		return true;
	case -1:
		return false;
	default:
		throw RArchiveException(ret,"RArchiveLha::FindFirstエラーが発生しました");
	}
	return false;
}

//ファイル検索 2つめ以降.
//@ioHandle		OpenArchiveで開いたハンドル
//@outSubInfo	結果
//@return		正常			true
//				検索終了		false
//失敗	RArchiveException
bool RArchiveLha::FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo)  throw()
{
	int ret = UnlhaFindNext(ioHandle,outSubInfo) ;
	switch(ret)
	{
	case 0:
		return true;
	case -1:
		return false;
	default:
		throw RArchiveException(ret,"RArchiveLha::FindFirstエラーが発生しました");
	}
	return false;
}
