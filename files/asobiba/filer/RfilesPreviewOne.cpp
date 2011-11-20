// RfilesPreviewOne.cpp: RfilesPreviewOne クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesPreviewOne.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RfilesPreviewOne::RfilesPreviewOne(string inPath)
{
	Path = inPath;
}
RfilesPreviewOne::~RfilesPreviewOne(){}

//一覧取得
void RfilesPreviewOne::ls()
{
	RTable * rt = getWritebleTable();

	rt->clear();

	RfilePreviewOne* p = (RfilePreviewOne*)RfileFactory::New(__RFILETYPE_PREVIEWONE);
	p->Attach( this->getPath().c_str() );
	rt->add( p );

//		rt->Sort();
	rt->createAllList( );
}
//一覧取得
void RfilesPreviewOne::ls(string inFilter)
{
	//区別無し
	ls();
}
//削除
void RfilesPreviewOne::rm(string inFilename)
{
	//サポートしません
}
//リネーム
void RfilesPreviewOne::rename(string inSrcFilename,string inDestFilename)
{
	//サポートしません
}
//種類を返す.
__RFILETYPE		RfilesPreviewOne::getRfileType()	const
{
	return __RFILETYPE_PREVIEWONE;
}
//パスの取得
string RfilesPreviewOne::getPath() const 
{
	return Path;
}
//パスの設定
void		RfilesPreviewOne::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTableを作成します。
//ls を使う前に呼んでください。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
void RfilesPreviewOne::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	//ファイルが一つしかないからソートといわれても(w
	setTable( new RTableOnlyOne );
}

//テスト.
void RfilesPreviewOne::test()
{
	{
		RfilesPreviewOne p("c:\\test\\2get.jpg");
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);

		try
		{
			//一覧取得
			p.ls(  );
		}
		catch(...)
		{
			return;
		}

		const RTable* rt = p.getTable();

		//絶対一つしかないし。
		ASSERT( rt->getAllSize() == 1);

		//一つに指定したファイル名が入っているかチェック.
		const Rfile* *rfiles = rt->getAllList();
		const Rfile* one = rfiles[0];

		if (one->getVars(__FILECOLUMN_TYPE_NAME) != "c:\\test\\2get.jpg")
		{
			ASSERT(0);
		}
	}
}
