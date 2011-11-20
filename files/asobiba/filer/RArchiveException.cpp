// RArchiveException.cpp: RArchiveException クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveException.h"

/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_LZH		0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048

#define ERROR_END	ERROR_BROKEN_DATA

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
RArchiveException::RArchiveException(unsigned long inErrorCode,char* Format,...)
{
	setErrorCode(inErrorCode);
	setError(Format,(char*)(&Format+1));

	string adderror;
//UnLha32の API.txt よりコピペ(w
//著作権とか大丈夫かな ｶﾞｸｶﾞｸｶﾞｸﾌﾞﾙﾌﾞﾙﾌﾞﾙ.
	switch(inErrorCode)
	{
//■警告    ：通常，該当ファイルについての処理をスキップするだけで，基本的に
//		    実行を中止することはありません。 "-jse" スイッチでエラーとする
//			ことができます。
	case ERROR_DISK_SPACE:
		adderror = "ディスクの空きが足りません。";
		FatalError = false;
		break;
	case ERROR_READ_ONLY:
		adderror = "上書きしようとしているファイルがリードオンリーです。";
		FatalError = false;
		break;
	case ERROR_USER_SKIP:
		adderror = "ユーザーによって処理がスキップされました。";
		FatalError = false;
		break;
	case ERROR_FILE_CRC:
		adderror = "格納ファイルのチェックサムが合っていません。";
		FatalError = false;
		break;
	case ERROR_FILE_OPEN:
		adderror = "ファイルを開けませんでした。";
		FatalError = false;
		break;
	case ERROR_MORE_FRESH:
		adderror = "より新しいファイルが展開先に存在しています。";
		FatalError = false;
		break;
	case ERROR_NOT_EXIST:
		adderror = "ファイルは展開先に存在していません。";
		FatalError = false;
		break;
	case ERROR_ALREADY_EXIST:
		adderror = "ファイルは既に存在しています。";
		FatalError = false;
		break;
	//■エラー  ：致命的なエラーで多くはその時点で実行を中止します。
	case ERROR_MAKEDIRECTORY:
		adderror = "ディレクトリが作成できません。";
		FatalError = true;
		break;
	case ERROR_CANNOT_WRITE:
		adderror = "書き込みエラーが生じました。";
		FatalError = true;
		break;
	case ERROR_HUFFMAN_CODE:
		adderror = "格納ファイルのハフマンコードが壊れています。";
		FatalError = true;
		break;
	case ERROR_HEADER_CRC:
		adderror = "書庫のヘッダのチェックサムが合っていません。";
		FatalError = true;
		break;
	case ERROR_HEADER_BROKEN:
		adderror = "書庫のヘッダが壊れています。";
		FatalError = true;
		break;
	case ERROR_ARC_FILE_OPEN:
		adderror = "書庫を開く事が出来ません。";
		FatalError = true;
		break;
	case ERROR_NOT_ARC_FILE:
		adderror = "書庫のファイル名が指定されていません。";
		FatalError = true;
		break;
	case ERROR_CANNOT_READ:
		adderror = "ファイルの読み込み時にエラーが生じました。";
		FatalError = true;
		break;
	case ERROR_FILE_STYLE:
		adderror = "指定されたファイルは有効な LZH 書庫ではありません。";
		FatalError = true;
		break;
	case ERROR_COMMAND_NAME:
		adderror = "コマンド指定が間違っています。";
		FatalError = true;
		break;
	case ERROR_MORE_HEAP_MEMORY:
		adderror = "作業用のためのヒープメモリが不足しています。";
		FatalError = true;
		break;
	case ERROR_ENOUGH_MEMORY:
		adderror = "グローバルメモリが不足しています。";
		FatalError = true;
		break;
	case ERROR_ALREADY_RUNNING:
		adderror = "既に UNLHA32.DLL が動作中です。";
		FatalError = true;
		break;
	case ERROR_USER_CANCEL:
		adderror = "ユーザーによって処理が中断されました。";
		FatalError = true;
		break;
	case ERROR_HARC_ISNOT_OPENED:
		adderror = "書庫とハンドルを関連付ける前に FindFirst 等が呼ばれました。";
		FatalError = true;
		break;
	case ERROR_NOT_SEARCH_MODE:
		adderror = "FindFirstが呼ばれる前に FindNext が呼ばれました。";
		FatalError = true;
		break;
	case ERROR_NOT_SUPPORT:
		adderror = "サポートされていない API が使用されました。";
		FatalError = true;
		break;
	case ERROR_TIME_STAMP:
		adderror = "日付及び時間の指定形式が間違っています。";
		FatalError = true;
		break;
	case ERROR_TMP_OPEN:
		adderror = "作業ファイルが作成できません。";
		FatalError = true;
		break;
	case ERROR_LONG_FILE_NAME:
		adderror = "ディレクトリのパスが長すぎます。";
		FatalError = true;
		break;
	case ERROR_ARC_READ_ONLY:
		adderror = "読み込み専用属性の書庫に対する操作はできません。";
		FatalError = true;
		break;
	case ERROR_SAME_NAME_FILE:
		adderror = "すでに同じ名前のファイルが書庫に格納されています。";
		FatalError = true;
		break;
	case ERROR_NOT_FIND_ARC_FILE:
		adderror = "指定されたディレクトリには書庫がありませんでした。";
		FatalError = true;
		break;
	case ERROR_RESPONSE_READ:
		adderror = "レスポンスファイルの読み込み時にエラーが生じました。";
		FatalError = true;
		break;
	case ERROR_NOT_FILENAME:
		adderror = "ファイル名の指定が必要です。";
		FatalError = true;
		break;
	case ERROR_TMP_COPY:
		adderror = "作業ファイルの書庫への書き戻しができませんでした。";
		FatalError = true;
		break;
	case ERROR_EOF:
		adderror = "書庫のデータが欠落しています。";
		FatalError = true;
		break;
	case ERROR_ADD_TO_LARC:
		adderror = "この SFX への書庫操作は行えません。";
		FatalError = true;
		break;
	case ERROR_TMP_BACK_SPACE:
		adderror = "作業ファイルの書き戻しに必要なディスクの空きがありません。";
		FatalError = true;
		break;
	case ERROR_SHARING:
		adderror = "ファイルへのアクセスが許可されていません。";
		FatalError = true;
		break;
	case ERROR_NOT_FIND_FILE:
		adderror = "ファイルが見つかりません。";
		FatalError = true;
		break;
	case ERROR_LOG_FILE:
		adderror = "ログファイルのエラーです。";
		FatalError = true;
		break;
	case ERROR_NO_DEVICE:
		adderror = "デバイスが見つかりません。";
		FatalError = true;
		break;
	case ERROR_GET_ATTRIBUTES:
		adderror = "ファイル属性が取得できません。";
		FatalError = true;
		break;
	case ERROR_SET_ATTRIBUTES:
		adderror = "ファイル属性の設定ができません。";
		FatalError = true;
		break;
	case ERROR_GET_INFORMATION:
		adderror = "ファイル情報が取得できません。";
		FatalError = true;
		break;
	case ERROR_GET_POINT:
		adderror = "ファイルポインタが取得できません。";
		FatalError = true;
		break;
	case ERROR_SET_POINT:
		adderror = "ファイルポインタの設定ができません。";
		FatalError = true;
		break;
	case ERROR_CONVERT_TIME:
		adderror = "タイムスタンプの変換ができません。";
		FatalError = true;
		break;
	case ERROR_GET_TIME:
		adderror = "タイムスタンプの取得ができません。";
		FatalError = true;
		break;
	case ERROR_SET_TIME:
		adderror = "タイムスタンプの設定ができません。";
		FatalError = true;
		break;
	case ERROR_CLOSE_FILE:
		adderror = "ファイルを閉じることができませんでした。";
		FatalError = true;
		break;
	case ERROR_HEAP_MEMORY:
		adderror = "ヒープメモリが壊れています。";
		FatalError = true;
		break;
	case ERROR_HANDLE:
		adderror = "無効なハンドルです。";
		FatalError = true;
		break;
	case ERROR_TIME_STAMP_RANGE:
		adderror = "無効なタイムスタンプです。";
		FatalError = true;
		break;
	case ERROR_MAKE_ARCHIVE:
		adderror = "書庫の作成に失敗しました。";
		FatalError = true;
		break;
	case ERROR_NOT_CONFIRM_NAME:
		adderror = "不正な名前が書庫に記録されています。";
		FatalError = true;
		break;
	case ERROR_UNEXPECTED_EOF:
		adderror = "ヘッダが部分的に欠落しています。";
		FatalError = true;
		break;
	case ERROR_INVALID_END_MARK:
		adderror = "不正なエンドマークです。";
		FatalError = true;
		break;
	case ERROR_NO_END_MARK:
		adderror = "必要なエンドマークが付加されていません。";
		FatalError = true;
		break;
	case ERROR_HDR_INVALID_SIZE:
		adderror = "不正なヘッダサイズです。";
		FatalError = true;
		break;
	case ERROR_UNKNOWN_LEVEL:
		adderror = "扱えないヘッダ形式です。";
		FatalError = true;
		break;
	case ERROR_BROKEN_DATA:
		adderror = "格納ファイルが壊れています。";
		FatalError = true;
		break;
	default:
		adderror = "未知のエラーが発生しました。びっくりです。";
		FatalError = true;
		break;
	}

	setError( adderror + "\n" + getMessage() );
}

RArchiveException::~RArchiveException()
{
}

