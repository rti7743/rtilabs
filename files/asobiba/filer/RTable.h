// RTable.h: RTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_)
#define AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTable;
#include "Rfile.h"


enum __SORTOPTION
{
	__SORTOPTION_WITH_MUCH_DUPLICATION,		//重複が多い
	__SORTOPTION_WITH_LITTLE_DUPLICATION,	//重複が少ない
};

//ファイル一覧を格納するクラス(抽象クラス)
class RTable  
{
public:
	RTable(__FILECOLUMN_TYPE inSortKey)
	{
		RFilesTable = NULL;
		FileCount = 0;
		SortKey = inSortKey;
	}
	virtual ~RTable()
	{
		delete [] RFilesTable;
		RFilesTable = NULL;
	}
	//新規ファイルの追加 (Rfile は必ず new で作成すること)
	virtual void add(Rfile* inRfile) = 0;
	//ファイルの一覧を取得する.
	virtual const Rfile** getAllList() const = 0;
	//ファイルの一覧を作成する.
	virtual const void createAllList() = 0;
	//消滅.
	virtual void clear() = 0;
	//ソート
	virtual void Sort() = 0;
	//ファイル一覧の数を変えす.
	int getAllSize() const { return FileCount; }
	//ソートする時にキーにするやつ
	__FILECOLUMN_TYPE GetSortKey(){	return SortKey; }

protected:
	//表示するときに使う　すべての Rfile にランダムアクセスできるようにするための配列
	//getAllList で作成する.
	Rfile**	RFilesTable;
	//現在何個の要素をもっているか
	int		FileCount;
	//ソートキー
	__FILECOLUMN_TYPE SortKey;
};

#endif // !defined(AFX_RTABLE_H__7B63EBD7_E64C_49F7_A5DF_B6FFBBF85A0A__INCLUDED_)
