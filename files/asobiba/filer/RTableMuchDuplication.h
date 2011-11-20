// RTableMuchDuplication.h: RTableMuchDuplication クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_)
#define AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RTable.h"


//ファイル名をソートするためのクラス. (STLのコピーless)
struct FilenameLess : binary_function<Rfile, Rfile, bool> {
bool operator()(class Rfile *const _X, class Rfile *const _Y) const
//	{return (_X->getName() < _Y->getName() ); }
	{return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );}
};

//ソートキーに重複が多いソート.
template<class _T>
class RTableMuchDuplication : public RTable  
{
public:
	//昇順でソートする.
	bool IsAscending;
	//ファイル名のソート
	typedef deque<Rfile*> FILENAMESORT;
	//ソート本陣
	typedef map< _T , FILENAMESORT* > MAINSORT;
	//ティレクトリは優先してソートするので別にする.
	FILENAMESORT	DirectoryArc;
	//ソート結果を格納する
	MAINSORT Arc;
private:
	RTableMuchDuplication() : RTable(__FILECOLUMN_TYPE_END) {}	//呼べませんよ.
public:
	RTableMuchDuplication(__FILECOLUMN_TYPE inSortKey,bool IsAscending) : RTable(inSortKey)
	{
		this->IsAscending = IsAscending;
	}
	virtual ~RTableMuchDuplication()
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
			FILENAMESORT* p = Arc[ *(_T*)(inRfile->getSortKey()) ];
			if (p == NULL)
			{
				p = new FILENAMESORT;
				Arc[ *(_T*)(inRfile->getSortKey()) ] = p;
			}
			p->push_back(inRfile);
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
			for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
			{
				FILENAMESORT* name = (*ext).second;
				for(FILENAMESORT::iterator p = name->begin(); p != name->end() ; ++p)
				{
					ASSERT(i < FileCount);
					//Rfile 追加.
					this->RFilesTable[i++] = ( *p );
				}
			}
		}
		else
		{
			for(MAINSORT::reverse_iterator ext =  Arc.rbegin(); ext != Arc.rend() ; ++ext)
			{
				FILENAMESORT* name = (*ext).second;
				for(FILENAMESORT::reverse_iterator p = name->rbegin(); p != name->rend() ; ++p)
				{
					ASSERT(i < FileCount);
					//Rfile 追加.
					this->RFilesTable[i++] = ( *p );
				}
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
		for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
		{
			FILENAMESORT* name = (*ext).second;
			for(FILENAMESORT::iterator p = name->begin(); p != name->end() ; ++p)
			{
				//消し消し.
				delete (*p);
			}
			name->clear();
			delete name;
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
		//一般ファイル
		for(MAINSORT::iterator ext =  Arc.begin(); ext != Arc.end() ; ++ext)
		{
			FILENAMESORT* name = (*ext).second;
			std::sort(name->begin() , name->end() ,RTableMuchDuplication::Compre);
		}
		//ディレクトリ
		std::sort(DirectoryArc.begin() , DirectoryArc.end() ,RTableMuchDuplication::Compre);
	}

	//ファイル名でソートしる関数 std::sort が使用する.
	static bool Compre(class Rfile *const _X, class Rfile *const _Y)
	{
		return (stricmp( _X->getVars(__FILECOLUMN_TYPE_NAME).c_str() , _Y->getVars(__FILECOLUMN_TYPE_NAME).c_str() ) <0 );
	}

	//テスト.
	static void test()
	{
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");
		RTableMuchDuplication<string> a(__FILECOLUMN_TYPE_EXT,true);
		RfileLocalReport* rlr;

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ggg.dat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag4.dat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag3.pat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag2.aat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.eat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.gat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.mat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.nat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.pat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.fat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		rlr = new RfileLocalReport();
		rlr->Attach(&p,"ag.bat",100,100,100);
		rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
		a.add( rlr );

		a.Sort();

		a.createAllList(  );

		DisplayDebugPrintf d;
		d.setRfileItems( a.getAllSize() , a.getAllList( ) );
 
		string DebugString = "ag.bat ★ ファイル ag.eat ★ ファイル ag.fat ★ ファイル ag.gat ★ ファイル ag.mat ★ ファイル ag.nat ★ ファイル ag.pat ★ ファイル ag2.aat ★ ファイル ag3.pat ★ ファイル ag4.dat ★ ファイル ggg.dat ★ ファイル ";
		if ( d.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",d.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
};

#endif // !defined(AFX_RTABLEMUCHDUPLICATION_H__B64108AE_BFFB_4379_8FD0_A770DB054978__INCLUDED_)
