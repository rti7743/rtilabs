// RTableEasy.h: RTableEasy クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_)
#define AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RTable.h"




//ファイル名だけの簡易ソート
class RTableEasy : public RTable  
{
private:
	//昇順でソートする.
	bool IsAscending;
	//ファイル名のソート
	typedef deque<Rfile*> FILENAMESORT;
	typedef FILENAMESORT MAINSORT;
	//ティレクトリは優先してソートするので別にする.
	FILENAMESORT	DirectoryArc;
	//ソート結果を格納する
	MAINSORT		Arc;

private:
	RTableEasy() : RTable(__FILECOLUMN_TYPE_END) {}	//呼べませんよ.
public:
	RTableEasy(__FILECOLUMN_TYPE inSortKey,bool IsAscending) : RTable(inSortKey)
	{
		this->IsAscending = IsAscending;
	}
	virtual ~RTableEasy()
	{
		clear();
	}

	//新規ファイルの追加 (Rfile は必ず new で作成すること)
	virtual void add(Rfile* inRfile)
	{
		if ( inRfile->IsDirectory() )
		{
			DirectoryArc.push_back(inRfile);
		}
		else
		{
			Arc.push_back(inRfile);
		}
		this->FileCount ++;
	}
	//ファイルの一覧を作成する.
	virtual const void createAllList()
	{
		//前回残っている Rfile* を破棄.
		delete [] this->RFilesTable;
		//今回の分を確保.
		this->RFilesTable = new Rfile*[FileCount];

		int i = 0;
		if (this->IsAscending)
		{
			//ディレクトリ優先
			for(FILENAMESORT::iterator p = DirectoryArc.begin(); p != DirectoryArc.end() ; ++p)
			{
				ASSERT(i < FileCount);
				//Rfile 追加.
				this->RFilesTable[i++] = ( *p );
			}
			//一般ファイル.
			for(MAINSORT::iterator fnm =  Arc.begin(); fnm != Arc.end() ; ++fnm)
			{
				ASSERT(i < FileCount);
				//Rfile 追加.
				this->RFilesTable[i++] = ( *fnm );
			}
		}
		else
		{
			for(MAINSORT::reverse_iterator fnm =  Arc.rbegin(); fnm != Arc.rend() ; ++fnm)
			{
				ASSERT(i < FileCount);
				//Rfile 追加.
				this->RFilesTable[i++] = ( *fnm );
			}
			//ディレクトリ優先
			for(FILENAMESORT::reverse_iterator p = DirectoryArc.rbegin(); p != DirectoryArc.rend() ; ++p)
			{
				ASSERT(i < FileCount);
				//Rfile 追加.
				this->RFilesTable[i++] = ( *p );
			}
		}
		ASSERT(i == FileCount);
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
		//一般ファイル
		for(MAINSORT::iterator fnm =  Arc.begin(); fnm != Arc.end() ; ++fnm)
		{
			delete (*fnm);
		}
		Arc.clear();
		//ディレクトリ
		for(FILENAMESORT::iterator p = DirectoryArc.begin(); p != DirectoryArc.end() ; ++p)
		{
			delete (*p);
		}
		DirectoryArc.clear();

		//現在保持している Rfileの値のクリア.
		this->FileCount = 0;
	}

	//ソート 
	virtual void Sort()
	{
		std::sort(Arc.begin() , Arc.end() ,RTableEasy::Compre);
		std::sort(DirectoryArc.begin() , DirectoryArc.end() ,RTableEasy::Compre);
	}

	//ファイル名でソートしる関数 std::sort が使用する.
	static bool Compre(class Rfile *const _X, class Rfile *const _Y)
	{
		return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );
	}

	//テスト.
	static void test();

};

#endif // !defined(AFX_RTABLEEASY_H__BEB7BA82_0283_4E5E_BB9D_7332CA2AF1D4__INCLUDED_)
