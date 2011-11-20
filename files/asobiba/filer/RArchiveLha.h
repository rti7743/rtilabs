// RArchiveLha.h: RArchiveLha クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_)
#define AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RArchive.h"

//LHA を解凍 / 圧縮するためのクラス.
//これ以上、継承禁止
class RArchiveLha : public RArchive  
{
public:
	RArchiveLha();
	virtual ~RArchiveLha();

	//!初期化できなかったら RDoNotInitaliseException
	//!関数が読み込めなかったら RDoNotInitaliseException
	virtual void Create() throw();

	//現在のバージョンを返します。
	//@return			現在のバージョン	  1 -> Version 0.01
	//										100 -> Version 1.00
	virtual WORD getVersion() ;

	//現在、動作中か否かを得ます。
	virtual bool IsRunning() ;

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
	virtual void Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize) throw();

	//ファイルが正しいアーカイヴであるかどうかチェック
	//@inFilename	ファイル名
	//@inMode		チェックモード
	//@return		正しい true
	virtual bool CheckArchive(const char* inFilename,const int inMode) ;

	//指定された書庫ファイルに格納されているファイル数を得ます。
	//@inFilename	ファイル名
	//@return		ファイル数
	virtual	int	GetFileCount(const char* inCmdLine) ;

	//アーカイヴを開く
	//@inHWND		関連付けるWindow
	//@inFilename	対象ファイル名
	//@inMode		開くときに使うオプション
	//@return		アーカイヴハンドル
	//!RIOCannotOpenException	開けません.
	virtual HARC OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode) throw();

	//OpenArchiveで開いたアーカイヴをクローズする.
	//@inHandle		アーカイヴハンドル
	//!RIOCannotOpenException	開けません.
	virtual void CloseArchive(HARC inHandle) throw();

	//ファイル検索を開始
	//ioHandle		OpenArchiveで開いたハンドル
	//@inWildName	検索するファイル名　ワイルドカードも（・∀・）ｲｲ！
	//@outSubInfo	結果
	//@return		正常			true
	//				検索終了		false
	//失敗	RArchiveException
	virtual bool FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo) throw();

	//ファイル検索 2つめ以降.
	//@ioHandle		OpenArchiveで開いたハンドル
	//@outSubInfo	結果
	//@return		正常			true
	//				検索終了		false
	//失敗	RArchiveException
	virtual bool FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo) throw();

private:
	//DLL超簡単に使うためのクラス.
	RAutoLibraryWithException  Lib;

//お礼にたっぷり DLL からインポートするにゃん
	//WORD WINAPI UnlhaGetVersion(VOID)
	typedef WORD (WINAPI *UNLHAGETVERSION)    (VOID);
	UNLHAGETVERSION UnlhaGetVersion;

	//BOOL WINAPI UnlhaGetRunning(VOID)
	typedef BOOL (WINAPI *UNLHAGETRUNNING)    (VOID);
	UNLHAGETRUNNING UnlhaGetRunning;

	//BOOL WINAPI UnlhaGetBackGroundMode(VOID)
	typedef BOOL (WINAPI *UNLHAGETBACKGROUNDMODE)    (VOID);
	UNLHAGETBACKGROUNDMODE UnlhaGetBackGroundMode;

	//BOOL WINAPI UnlhaSetBackGroundMode(const BOOL _bBackGroundMode)
	typedef BOOL (WINAPI *UNLHASETBACKGROUNDMODE)    (const BOOL _bBackGroundMode);
	UNLHASETBACKGROUNDMODE UnlhaSetBackGroundMode;

	//BOOL WINAPI UnlhaGetCursorMode(VOID)
	typedef BOOL (WINAPI *UNGETCURSORMODE)    (VOID);
	UNGETCURSORMODE UnlhaGetCursorMode;

	//BOOL WINAPI UnlhaSetCursorMode(const BOOL _CursorMode)
	typedef BOOL (WINAPI *UNLHASETCOUSORMODE)    (const BOOL _CursorMode);
	UNLHASETCOUSORMODE UnlhaSetCursorMode;

	//WORD WINAPI UnlhaGetCursorInterval(VOID)
	typedef WORD (WINAPI *UNLHAGETCURSORINTERNAL)    (VOID);
	UNLHAGETCURSORINTERNAL UnlhaGetCursorInterval;

	//BOOL WINAPI UnlhaSetCursorInterval(const WORD _Interval)
	typedef BOOL (WINAPI *UNLHASETCURSORINTERNAL)    (const WORD _Interval);
	UNLHASETCURSORINTERNAL UnlhaSetCursorInterval;

	//int WINAPI Unlha(const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize)
	typedef int (WINAPI *UNLHA)    (const HWND _hwnd, LPCSTR _szCmdLine,	LPSTR _szOutput, const DWORD _dwSize);
	UNLHA Unlha;

	//BOOL WINAPI UnlhaCheckArchive(LPCSTR _szFileName,	const int _iMode)
	typedef BOOL (WINAPI *UNLHACHECKARCHIVE)    (LPCSTR _szFileName,	const int _iMode);
	UNLHACHECKARCHIVE UnlhaCheckArchive;

	//int WINAPI UnlhaGetFileCount(LPCSTR _szArcFile)
	typedef int (WINAPI *UNLHAGETFILECOUNT)    (LPCSTR _szArcFile);
	UNLHAGETFILECOUNT UnlhaGetFileCount;

	//BOOL WINAPI UnlhaConfigDialog(const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode)
	typedef BOOL (WINAPI *UNLHACONFIGDIALOG)    (const HWND _hwnd,	LPSTR _szOptionBuffer, const int _iMode);
	UNLHACONFIGDIALOG UnlhaConfigDialog;

	//HARC WINAPI UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode)
	typedef HARC (WINAPI *UNLHAOPENARCHIVE)    (const HWND _hwnd, LPCSTR _szFileName,const DWORD _dwMode);
	UNLHAOPENARCHIVE UnlhaOpenArchive;

	//int WINAPI _export UnlhaCloseArchive(HARC _harc)
	typedef int (WINAPI *UNLHACLOSEARCHIVE)    (HARC _harc);
	UNLHACLOSEARCHIVE UnlhaCloseArchive;

	//int WINAPI UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo)
	typedef int (WINAPI *UNLHAFINDFIRST)    (HARC _harc, LPCSTR _szWildName,	INDIVIDUALINFO FAR *lpSubInfo);
	UNLHAFINDFIRST UnlhaFindFirst;

	//int WINAPI UnlhaFindNext(HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo)
	typedef int (WINAPI *UNLHAFINDNEXT)    (HARC _harc,	INDIVIDUALINFO FAR *_lpSubInfo);
	UNLHAFINDNEXT UnlhaFindNext;



};

#endif // !defined(AFX_RARCHIVELHA_H__49736B50_D0AE_4169_8D70_4251264DFD05__INCLUDED_)
