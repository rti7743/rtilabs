// FileModifyChacheWin32.cpp: FileModifyChacheWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "FileModifyChacheWin32.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FileModifyChacheWin32::FileModifyChacheWin32(int inWidth,int inHegith ,int inSystemChacheSize)
{
	this->ImageList.Create(inWidth , inHegith ,  inSystemChacheSize , 1000);

	//システムキャッシュの領域の作成.
	this->NoneChacheSystem = new FileModifyAdministrant*[inSystemChacheSize];
	this->NoneChacheSystemSize = inSystemChacheSize;
}

FileModifyChacheWin32::~FileModifyChacheWin32()
{
	Clear();
	this->ImageList.Destroy();
}

//検索.
//見つからない場合は NULL
FileModifyData* FileModifyChacheWin32::Search(string inExt)
{
	return Chache[inExt];
}

//システムより取得
FileModifyData* FileModifyChacheWin32::SearchSystem(int inPosition) const
{
	ASSERT(inPosition >= 0 && inPosition < this->NoneChacheSystemSize);
	return this->NoneChacheSystem[inPosition];
}

//教える
FileModifyData* FileModifyChacheWin32::Tearch
				(string inExt,HICON inIcon , string inFileType)
{
	//実はすでに誰かが教えていた場合...
	FileModifyData* p = this->Chache[inExt];
	if ( p != NULL ) return p;

	const int index = this->ImageList.Add(inIcon);
	ASSERT(index != -1);
#ifdef _DEBUG
	//重複したindexを割り当てないように監視.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ( (*it).second != NULL)
			{
				ASSERT( (*it).second->getIndex() != index );
			}
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT( (*t)->getIndex() != index );
		}
	}
#endif
	FileModifyAdministrant* r = new FileModifyAdministrant(	index,inFileType	);
#ifdef _DEBUG
	//重複した値を割り当てないように監視.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			ASSERT(r != (*it).second );
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT(r != (*t) );
		}
	}
#endif
	this->Chache[inExt] = r;
	return r;
}

//教える
FileModifyData* FileModifyChacheWin32::Tearch(HICON inIcon , string inFileType)
{
	const int index = this->ImageList.Add( inIcon);
	ASSERT(index != -1);
#ifdef _DEBUG
	//重複したindexを割り当てないように監視.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ( (*it).second != NULL)
			{
				ASSERT( (*it).second->getIndex() != index );
			}
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT( (*t)->getIndex() != index );
		}
	}
#endif
	FileModifyAdministrant* r = new FileModifyAdministrant( index ,inFileType	);
#ifdef _DEBUG
	//重複した値を割り当てないように監視.
	{
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			ASSERT(r != (*it).second );
		}
		for ( ICONONECHACHE::iterator t = NoneChache.begin() ;	t != NoneChache.end() ; t++)
		{
			ASSERT(r != (*t) );
		}
	}
#endif
	NoneChache.push_back(r);

	return r;
}


//教える  キャッシュできないシステム系なデータ
FileModifyData* FileModifyChacheWin32::TearchSystem(int inPosition , HICON inIcon , string inFileType)
{
	ASSERT(inPosition >= 0 && inPosition < this->NoneChacheSystemSize);

	FileModifyAdministrant* r = new FileModifyAdministrant
		(	this->ImageList.Add( inIcon),inFileType	);
	this->NoneChacheSystem[inPosition] = r;

	return r;
}


//使用する
void FileModifyChacheWin32::Use(FileModifyData* inFileModifyData)
{
	((FileModifyAdministrant*)inFileModifyData)->Use();
}

//削除する
void FileModifyChacheWin32::Delete(FileModifyData* inFileModifyData)
{
	FileModifyAdministrant* p =((FileModifyAdministrant*)inFileModifyData);
	
	p->Needless();
	if (p->IsDelete())
	{
		//キャッシュより検索して削除します。
		if ( SearchDelete(p) )
		{//キャッシュにヒットしないということは systemのキャッシュだということです。
			
			//イメージリストより削除.
			this->ImageList.Remove(p->getIndex() );
			//実態を削除.
			delete p;
		}
	}
}

//ターゲットを検索して、削除する
//見つからない場合は false .... つまりシステム系のキャッシュです。
bool FileModifyChacheWin32::SearchDelete(FileModifyAdministrant* inTarget)
{
	ASSERT(inTarget != NULL);

	//検索して キャッシュより削除
	for ( ICONCHACHE::iterator i = this->Chache.begin() ;	i != this->Chache.end() ; ++i)
	{
		if ( ((*i).second) == inTarget)
		{
			this->Chache.erase(i);
			return true;
		}
	}
	//検索して ユニークキャッシュより削除
	for ( ICONONECHACHE::iterator t = this->NoneChache.begin() ;	t != this->NoneChache.end() ; ++t)
	{
		if ( (*t) == inTarget)
		{
	#ifdef _DEBUG
			for ( ICONCHACHE::iterator x = this->Chache.begin() ;	x != this->Chache.end() ; ++x)
			{
				ASSERT ((*x).second != inTarget) ;
			}
	#endif
			this->NoneChache.erase(t);
			return true;
		}
	}
#ifdef _DEBUG
	//本当にシステムなのかチェックします。
	for ( int n = 0 ;	n < this->NoneChacheSystemSize ; n++ )
	{
		if ( this->NoneChacheSystem[n] == inTarget)
		{
			//OK
			return false;
		}
	}
	puts("システムにもないファイル修飾子を削除しようとしました。\nキャッシュの統合性が壊れています。");
	ASSERT(0);
#else
	return false;
#endif
}

//システム系のキャッシュも含めて ALL あぼーん。
void FileModifyChacheWin32::Clear()
{
	//キャッシュ逝ってよし
	for ( ICONCHACHE::iterator i = this->Chache.begin() ;	i != this->Chache.end() ; ++i)
	{
		delete (*i).second;
	}
	this->Chache.clear();

	//ユニークなキャッシュも逝ってよし
	for ( ICONONECHACHE::iterator t = this->NoneChache.begin() ;	t != this->NoneChache.end() ; ++t)
	{
		delete (*t);
	}
	this->NoneChache.clear();

	for ( int n = 0 ;	n < this->NoneChacheSystemSize ; n++ )
	{
		delete this->NoneChacheSystem[n];
	}

	//システムも逝ってよし
	delete [] this->NoneChacheSystem;

	//イメージリストをクリア.
	this->ImageList.RemoveAll();
}

//デバッグ
void FileModifyChacheWin32::Dump()
{
	{
		TRACE("\n============== icon cache dump ================-\n");
		for ( ICONCHACHE::iterator it = Chache.begin() ;	it != Chache.end() ; it++)
		{
			if ((*it).second != NULL)
			{
				printf( "--------ext as %s " , ((*it).first).c_str() );
				((*it).second)->Dump();
			}
		}
	}
		TRACE("\n============== icon nonecache dump ================-\n");
	{
		for ( ICONONECHACHE::iterator it = NoneChache.begin() ;	it != NoneChache.end() ; it++)
		{
			(*it)->Dump();
		}
		TRACE("\n============== ============= ================-\n");
	}

}

