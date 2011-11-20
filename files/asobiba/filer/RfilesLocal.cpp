// RfilesLocal.cpp: RfilesLocal クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileFactory.h"
#include "RfilesLocal.h"
#include "RNotFoundException.h"
#include "regex.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//初期化できなかった場合は RNotSupportException を投げる.
RfilesLocal::RfilesLocal(__RFILETYPE inRfileType , const string inPath) 
{
	RfileType = inRfileType;
	Path = inPath;

	switch(RfileType)
	{
	case __RFILETYPE_LOCALFILE_REPORT:
	case __RFILETYPE_LOCALFILE_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesLocal::指定された ファイル形式 %d は サポートしていません。",RfileType);
		break;
	}
}
//final
RfilesLocal::~RfilesLocal()
{
}

//一覧取得
void RfilesLocal::ls()
{
	WIN32_FIND_DATA data;
	HANDLE p;

	RTable * rt = getWritebleTable();

	rt->clear();

	string wildpath = this->Path + "*.*";
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
	p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			// . または .. のファイルは無視.
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
				(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) )
			)	continue;

			RfileLocal* p = (RfileLocal*) RfileFactory::New(RfileType);
			p->Attach(this , data.cFileName , 
				INT64CONVERT(data.nFileSizeHigh , data.nFileSizeLow)  ,
				INT64CONVERT(data.ftCreationTime.dwHighDateTime , data.ftCreationTime.dwLowDateTime) ,
				data.dwFileAttributes);
			p->SortAttach( sortkey );
			rt->add( p );
		}
		while( ::FindNextFile( p , &data) );
		::FindClose(p);
	}

	rt->Sort();
	rt->createAllList( );
}
//一覧取得
void RfilesLocal::ls(string inFilter)
{
	WIN32_FIND_DATA data;
	HANDLE p;

	RTable * rt = getWritebleTable();

	//検索用正規表現を設定.
	boost::reg_expression<char> filterRegex  = inFilter.c_str();
	boost::match_results<const char*> results;

	rt->clear();

	string wildpath = this->Path + "*.*";
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
	p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			// . または .. のファイルは無視.
			if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
				(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) )
			)	continue;

			if ( boost::regex_search(data.cFileName , results,filterRegex) == 0) continue;

			RfileLocal* p = (RfileLocal*) RfileFactory::New(RfileType);
			p->Attach(this , data.cFileName , 
				INT64CONVERT(data.nFileSizeHigh , data.nFileSizeLow)  ,
				INT64CONVERT(data.ftCreationTime.dwHighDateTime , data.ftCreationTime.dwLowDateTime) ,
				data.dwFileAttributes);
			p->SortAttach( sortkey );
			rt->add( p );
		}
		while( ::FindNextFile( p , &data) );
		::FindClose(p);
	}

	rt->Sort();
	rt->createAllList( );
}
//削除
void RfilesLocal::rm(string inFilename)
{
}
//リネーム
void RfilesLocal::rename(string inSrcFilename,string inDestFilename)
{
}
//種類を返す.
__RFILETYPE		RfilesLocal::getRfileType()	const
{
	return RfileType;
}
//パスの取得
string RfilesLocal::getPath() const 
{
	return Path;
}
//パスの設定
void		RfilesLocal::setPath(const string inPath)  
{
	this->Path = inPath;
}
//RTableを作成します。
//ls を使う前に呼んでください。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
void RfilesLocal::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	switch(inKey)
	{
	case __FILECOLUMN_TYPE_NAME:
		setTable( new RTableEasy(inKey,IsAscending) );
		break;
	case __FILECOLUMN_TYPE_EXT:
		setTable( new RTableMuchDuplication<string>(inKey,IsAscending) );
		break;
	case __FILECOLUMN_TYPE_SIZE:
//			setTable( new RTableLittleDuplication<__int64>(inKey,IsAscending) );
		break;
	case __FILECOLUMN_TYPE_DATE:
//			setTable( new RTableLittleDuplication<__int64>(inKey,IsAscending) );
		break;
	default:
		throw RNotSupportException("指定されたタイプのカラム %d はサポートされていません",inKey);
		break;
	}
	return;
}

//テスト.
void RfilesLocal::test()
{
	{
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");

		//一覧取得
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls(  );

		const RTable* rt = p.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1039.jpg ★ JPGファイル 2get.jpg ★ JPGファイル giko2a.jpg ★ JPGファイル NHKにモナー.jpg ★ JPGファイル NHKにモナー2.jpg ★ JPGファイル おにぎりわっしょい.jpg ★ JPGファイル サッカーで（ﾟдﾟ）ﾊｧ.jpg ★ JPGファイル ホームラン級のバカ.jpg ★ JPGファイル メラゾーマ.jpg ★ JPGファイル 稲垣メンバー.jpg ★ JPGファイル 激しく同意.jpg ★ JPGファイル 逝って良しケーキ.jpg ★ JPGファイル 名スレの予感.jpg ★ JPGファイル 名無しさん@ゴーゴーゴー.jpg ★ JPGファイル ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}

	}
	{
		//取得できないディレクトリーを設定してみる
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\t国崎最高!!");

		//一覧取得
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls();

		const RTable* rt = p.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "";

		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
}
