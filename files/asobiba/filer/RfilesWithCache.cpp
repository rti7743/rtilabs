// RfilesWithCache.cpp: RfilesWithCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesWithCache.h"
#include "regex.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RfilesWithCache::RfilesWithCache(CacheFileFnctional* inCff , __RFILETYPE inRfileType , const string inPath) throw()
{
	ASSERT(inCff != NULL);

	this->Cff = inCff;
	this->RfileType = inRfileType;
	this->Path = inPath;

	//ソースパスの取得
	this->SrcPath = Cff->getSrcPath( Path );

	//オリジナル検索
	RCacheFS* cfs = RCacheFS::getInstance();
	Rfiles* orignal = cfs->Search( inRfileType,this->SrcPath );
	//ない場合は作成.
	if ( orignal == NULL )
	{
		orignal = Cff->New(inRfileType,this->SrcPath);
		cfs->Append( orignal );
		//新規作成なので MakeTable が呼ばれたときにテーブルを作成してあげないといけない。
		this->MostMakeRTable = true;
	}
	else
	{
		this->MostMakeRTable = false;
	}
	this->OrignalFiles = orignal;

	ASSERT(this->OrignalFiles != NULL);
}

RfilesWithCache::~RfilesWithCache()
{
	//もう使わないのでキャッシュに破棄を命じる.
	RCacheFS* cfs = RCacheFS::getInstance();
	cfs->Remove( this->OrignalFiles );

	delete Cff;
}


//一覧取得
void RfilesWithCache::ls()
{
	//この RTable に格納していきます。
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//キャッシュしている RTable を取得.
	const RTable * table = OrignalFiles->getTable();
	int len = table->getAllSize();
	if (len <= 0)
	{
		//lsが未実行なので実行してあげる.
		OrignalFiles->ls();
		//もう一度サイズの取得.
		len = table->getAllSize();
	}
	const Rfile** file = table->getAllList();

	//相対パスの取得.
	const string relativePath = Cff->getRelativePath(this->Path);

	for(int i = 0; i < len ; i++)
	{
		if ( relativePath == file[i]->getVars(__FILECOLUMN_TYPE_VIRTUALPATH) )
		{
			//コピー.
			Rfile* p = RfileFactory::Clone(file[i]);
			p->SortAttach( sortkey );
			rt->add( p );
		}
	}
	rt->Sort();
	rt->createAllList( );
}

//一覧取得
void RfilesWithCache::ls(string inFilter)
{
	//この RTable に格納していきます。
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//検索用正規表現を設定.
	boost::reg_expression<char> filterRegex  = inFilter.c_str();
	boost::match_results<const char*> results;

	//キャッシュしている RTable を取得.
	const RTable * table = OrignalFiles->getTable();
	int len = table->getAllSize();
	if (len <= 0)
	{
		//lsが未実行なので実行してあげる.
		OrignalFiles->ls();
		//もう一度サイズの取得.
		len = table->getAllSize();
	}
	const Rfile** file = table->getAllList();

	//相対パスの取得.
	const string relativePath = Cff->getRelativePath(this->Path);

	for(int i = 0; i < len ; i++)
	{
		if ( relativePath == file[i]->getVars(__FILECOLUMN_TYPE_VIRTUALPATH) )
		{
			if ( boost::regex_search(file[i]->getVars(__FILECOLUMN_TYPE_NAME) , results,filterRegex) == 0) continue;

			//コピー.
			Rfile* p = RfileFactory::Clone(file[i]);
			p->SortAttach( sortkey );
			rt->add( p );
		}
	}
	rt->Sort();
	rt->createAllList( );
}
//削除
void RfilesWithCache::rm(string inFilename)
{
}
//リネーム
void RfilesWithCache::rename(string inSrcFilename,string inDestFilename)
{
}
//種類を返す.
__RFILETYPE		RfilesWithCache::getRfileType()	const
{
	return this->OrignalFiles->getRfileType();
}
//種類を返す.
string		RfilesWithCache::getPath()	const
{
	return this->Path;
}
//パスの設定
void		RfilesWithCache::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTableを作成します。
//ls を使う前に呼んでください。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
void RfilesWithCache::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	//内部で保持している Rfiles* は新規に作ったので
	//RTable をもっていなかったりします?
	if ( this->MostMakeRTable )
	{
		this->OrignalFiles->MakeTable(inKey,IsAscending);
		this->MostMakeRTable = false;
	}
	//オリジナルに RTable を作成させる.
	setTable( Cff->ProxyCreateTable( this->OrignalFiles, inKey,IsAscending) );
}

//テスト.
void RfilesWithCache::test()
{
	{
		ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
		acff->ArchiveType = __ARCHIVETYPE_LZH;
		RfilesWithCache rfwc(acff, __RFILETYPE_ARC_BIGICON , "c:\\test3\\test.lzh::\\");
		rfwc.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		rfwc.ls();

		const RTable* rt = rfwc.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "OpenOffice.org Calc.lnk ★ ショートカット OpenOffice.org Draw.lnk ★ ショートカット OpenOffice.org Writer.lnk ★ ショートカット abc ★ フォルダ bb b ★ フォルダ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
	{
		ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
		acff->ArchiveType = __ARCHIVETYPE_LZH;
		RfilesWithCache rfwc(acff, __RFILETYPE_ARC_BIGICON , "c:\\test3\\test.lzh::\\abc\\");
		rfwc.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		rfwc.ls();

		const RTable* rt = rfwc.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1039.jpg ★ JPGファイル 2get.jpg ★ JPGファイル メラゾーマ.jpg ★ JPGファイル ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
}
