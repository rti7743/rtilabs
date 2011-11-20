// RTableOnlyOne.h: RTableOnlyOne クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_)
#define AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RTable.h"

//一つだけの要素のテーブル
//一つだけだからソートも何もないです。
class RTableOnlyOne : public RTable  
{
private:
	//唯一のデータ
	Rfile*		UniqueConstituent;
public:
	RTableOnlyOne() : RTable(__FILECOLUMN_TYPE_END)
	{
		this->UniqueConstituent = NULL;
	}
	virtual ~RTableOnlyOne()
	{
		clear();
	}

	//新規ファイルの追加 (Rfile は必ず new で作成すること)
	virtual void add(Rfile* inRfile)
	{
		ASSERT(this->FileCount < 1);

		this->UniqueConstituent = inRfile;
		this->FileCount = 1;
	}
	//ファイルの一覧を作成する.
	virtual const void createAllList()
	{
		//前回残っている Rfile* を破棄.
		delete [] this->RFilesTable;

		ASSERT(this->FileCount == 1);

		//今回の分を確保.
		this->RFilesTable = new Rfile*[1];

		//Rfile 追加.
		this->RFilesTable[0] = ( this->UniqueConstituent );
	}

	//ファイルの一覧を取得する.
	//createAllList を読んだ後で呼び出してください!!
	virtual const Rfile** getAllList() const
	{
		ASSERT(this->RFilesTable != NULL);

		//const Rfile* なので書き換えはできませんよ。
		return (const Rfile**)this->RFilesTable;
	}

	//消滅.
	virtual void clear()
	{
		//データの破棄.
		delete this->UniqueConstituent;
		//現在保持している Rfileの値のクリア.
		this->FileCount = 0;
	}

	//ソート 
	virtual void Sort()
	{
		//一つしか要素がないし。
		//やることねー.
	}
};

#endif // !defined(AFX_RTABLEONLYONE_H__90A90160_E66F_451C_B26E_63BA998DAC85__INCLUDED_)
