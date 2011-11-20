// RArchive.h: RArchive クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_)
#define AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotCloseException.h"
#include "RArchiveException.h"

enum __ARCHIVETYPE
{
	__ARCHIVETYPE_LZH,
};

typedef	HGLOBAL	HARC;

#define FNAME_MAX32		512

typedef struct 
{
	DWORD 			dwOriginalSize;		/* ファイルのサイズ */
 	DWORD 			dwCompressedSize;	/* 圧縮後のサイズ */
	DWORD			dwCRC;				/* 格納ファイルのチェックサム */
	UINT			uFlag;				/* 処理結果 */
										/* Status flag */
	UINT			uOSType;			/* 書庫作成に使われた OS */
	WORD			wRatio;				/* 圧縮率 */
	WORD			wDate;				/* 格納ファイルの日付(DOS 形式) */
	WORD 			wTime;				/* 格納ファイルの時刻(〃) */
	char			szFileName[FNAME_MAX32 + 1];	/* 書庫名 */
	char			dummy1[3];
	char			szAttribute[8];		/* 格納ファイルの属性(書庫固有) */
	char			szMode[8];			/* 格納ファイルの格納モード(〃) */
}	INDIVIDUALINFO;


#define M_INIT_FILE_USE			0x00000001L	/* レジストリの設定を使用 */
#define M_REGARDLESS_INIT_FILE	0x00000002L	/* 〃 を使用しない */
#define M_NO_BACKGROUND_MODE	0x00000004L	/* バックグラウンドを禁止 */
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	/* 厳格なファイル名サーチ */
#define M_CHECK_FILENAME_ONLY	0x00000200L	/* 〃を行わない */
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	/* ドライブ名から格納 */
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	/* 〃 を格納しない */
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* エラーメッセージを表示 */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* 〃を表示しない */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L /* 破損ヘッダの読み飛ばし */

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	/* 検索されたファイルを解凍 */
#define EXTRACT_NAMED_FILE		0x80000000L	/* 指定したファイルを解凍 */


class RArchive  
{
protected:
	RArchive()
	{
	}
public:
	virtual ~RArchive()
	{
	}
public:
	//!初期化できなかったら RDoNotInitaliseException
	//!関数が読み込めなかったら RDoNotInitaliseException
	virtual void Create() throw() = 0;

	//現在のバージョンを返します。
	//@return			現在のバージョン	  1 -> Version 0.01
	//										100 -> Version 1.00
	virtual WORD getVersion() = 0;

	//現在、動作中か否かを得ます。
	virtual bool IsRunning() = 0;

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
	virtual void Command(HWND inHWND,const char* inCmdLine, char* outOutput, WORD inSize)  throw() = 0;

	//ファイルが正しいアーカイヴであるかどうかチェック
	//@inFilename	ファイル名
	//@inMode		チェックモード
	//@return		正しい true
	virtual bool CheckArchive(const char* inFilename,const int inMode) = 0;

	//指定された書庫ファイルに格納されているファイル数を得ます。
	//@inFilename	ファイル名
	//@return		ファイル数
	virtual	int	GetFileCount(const char* inCmdLine) = 0;

	//アーカイヴを開く
	//@inHWND		関連付けるWindow
	//@inFilename	対象ファイル名
	//@inMode		開くときに使うオプション
	//@return		アーカイヴハンドル
	//!RIOCannotOpenException	開けません.
	virtual HARC OpenArchive(HWND inHWND, const char* inFilename,DWORD inMode) = 0;

	//OpenArchiveで開いたアーカイヴをクローズする.
	//@inHandle		アーカイヴハンドル
	//!RIOCannotOpenException	開けません.
	virtual void CloseArchive(HARC inHandle) = 0;

	//ファイル検索を開始
	//ioHandle		OpenArchiveで開いたハンドル
	//@inWildName	検索するファイル名　ワイルドカードも（・∀・）ｲｲ！
	//@outSubInfo	結果
	//@return		正常			true
	//				検索終了		false
	//失敗	RArchiveException
	virtual bool FindFirst(HARC ioHandle, const char* inWildName,INDIVIDUALINFO FAR *outSubInfo) = 0;

	//ファイル検索 2つめ以降.
	//@ioHandle		OpenArchiveで開いたハンドル
	//@outSubInfo	結果
	//@return		正常			true
	//				検索終了		false
	//失敗	RArchiveException
	virtual bool FindNext(HARC ioHandle,INDIVIDUALINFO FAR *outSubInfo) = 0;
};

#endif // !defined(AFX_RARCHIVE_H__72CC95A9_AF13_4C95_8CB1_1268DA1F99A9__INCLUDED_)
