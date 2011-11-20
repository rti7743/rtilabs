// RfilesLzh.cpp: RfilesLzh クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesLzh.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//サポートしていないやつは RNotSupportException
//!初期化できなかったら RDoNotInitaliseException
//!関数が読み込めなかったら RDoNotInitaliseException
RfilesLzh::RfilesLzh(__ARCHIVETYPE inArchiveType,__RFILETYPE inRfileType,const string inPath) throw()
{
	RfileType = inRfileType;
	Path = inPath;

	ArchiveEngine = RArchiveFactory::New(inArchiveType);

	switch(RfileType)
	{
	case __RFILETYPE_ARC_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesLzh::指定された ファイル形式 %d は サポートしていません。",RfileType);
		break;
	}
}
//final
RfilesLzh::~RfilesLzh()
{
	delete ArchiveEngine;
}

//一覧取得
void RfilesLzh::ls()
{
	INDIVIDUALINFO data;
	HARC p;

	RTable * rt = getWritebleTable();

	rt->clear();

	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	//すでに登録したディレクトリ.
	set<string>	knownPath;

	try
	{
		ArchiveEngine->Create();

		//開きます。 開けなかった場合例外
		p = ArchiveEngine->OpenArchive(WindowsMain::getMainSystem()->getWindowHandle(), this->getPath().c_str() , M_ERROR_MESSAGE_OFF | M_RECOVERY_ON | M_NO_BACKGROUND_MODE | M_CHECK_FILENAME_ONLY);
		if ( ArchiveEngine->FindFirst(p , "*.*" , &data) )
		{
			do
			{

				string fullname = data.szFileName;

				// / を \\ にする.
				replace( fullname.begin() , fullname.end() , '/','\\');
				
				if (fullname[0] != '\\')	fullname = string("\\") + fullname;
				else			     		fullname = data.szFileName;

				//最後の 区切り文字を取得.
				int lastpos = MultiString::getLastDirectoryPos( fullname  );

				RfileArc* p = (RfileArc*) RfileFactory::New(RfileType);
				p->Attach(
					fullname.substr(0 ,lastpos + 1).c_str()   , 
					fullname.substr(lastpos + 1).c_str()	,
					data.dwOriginalSize ,
					MAKELONG(data.wDate , data.wTime) ,
					data.wRatio,
					data.szMode,
					data.szAttribute
					);
				p->SortAttach( sortkey );
				rt->add( p );

				//たまにディレクトリを圧縮してくれないやつがいるので
				//このパスのディレクトリが格納されているか調べて、格納されていなければ追加する.
				int dilimiter[MAX_DIMLIMITERES+1] ;	//+1は番兵
				int maxDilimiter = MultiString::setPathDelimiteres(fullname.substr(0 ,lastpos ).c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
				ASSERT(maxDilimiter <= MAX_DIMLIMITERES);
				dilimiter[maxDilimiter] = lastpos ;

				for( int pos = 0; pos <= (maxDilimiter-1) ; pos++)
				{
					const string folderdir  = fullname.substr(0 ,  dilimiter[pos] + 1 );
					const string foldername = fullname.substr(dilimiter[pos] + 1 , dilimiter[pos+1] - dilimiter[pos] - 1);

					const string unknownPath  = folderdir + foldername;

					if ( (!unknownPath.empty()) && (knownPath.find(unknownPath) == knownPath.end()) )
					{//ないらしい...
						knownPath.insert(unknownPath);
						
						RfileArc* p = (RfileArc*) RfileFactory::New(RfileType);
						p->Attach(
							folderdir.c_str()   , 
							foldername.c_str()	,
							0 ,
							0,
							0,
							"",
							"DIR"
							);
						p->SortAttach( sortkey );
						rt->add( p );
					}
				}
			}
			while( ArchiveEngine->FindNext( p , &data) );
		}
		ArchiveEngine->CloseArchive(p);
	}
	catch(RDoNotInitaliseException e)
	{
		TRACE("取得中に RDoNotInitaliseException が発生しました 理由:%s\n" , e.getMessage() );
	}
	catch(RNotSupportException e)
	{
		TRACE("取得中に RNotSupportException が発生しました 理由:%s\n" , e.getMessage() );
	}
	catch(RIOCannotOpenException e)
	{//OpenArchiveに失敗
		TRACE("取得中に RIOCannotOpenException が発生しました 理由:%s\n" , e.getMessage() );
	}
	catch(RIOCannotCloseException e)
	{//CloseArchiveに失敗
		TRACE("取得中に RIOCannotCloseException が発生しました 理由:%s\n" , e.getMessage() );
	}
	catch(RArchiveException e)
	{//取得中にエラー
		TRACE("取得中に RArchiveException が発生しました 理由:%s\n" , e.getMessage() );
	}

	rt->Sort();
	rt->createAllList( );
}
//一覧取得
void RfilesLzh::ls(string inFilter)
{
	//上位層がやってくれるのでここでは特に区別しません.
	ls();
}
//削除
void RfilesLzh::rm(string inFilename)
{
}
//リネーム
void RfilesLzh::rename(string inSrcFilename,string inDestFilename)
{
}
//種類を返す.
__RFILETYPE		RfilesLzh::getRfileType()	const
{
	return RfileType;
}
//パスの取得
string RfilesLzh::getPath() const 
{
	return Path;
}
//パスの設定
void		RfilesLzh::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTableを作成します。
//ls を使う前に呼んでください。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
void RfilesLzh::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	setTable( CreateTable(inKey,IsAscending) );
}

//RTableを作成します。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
RTable* RfilesLzh::CreateTable(__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() 
{
	switch(inKey)
	{
	case __FILECOLUMN_TYPE_NAME:
		return new RTableEasy(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_EXT:
		return new RTableMuchDuplication<string>(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_SIZE:
//			return new RTableLittleDuplication<__int64>(inKey,IsAscending) ;
		break;
	case __FILECOLUMN_TYPE_DATE:
//			return new RTableLittleDuplication<__int64>(inKey,IsAscending) ;
		break;
	default:
		throw RNotSupportException("指定されたタイプのカラム %d はサポートされていません",inKey);
		break;
	}
	return NULL;	//nop
}


//テスト.
void RfilesLzh::test()
{
	{
		RfilesLzh p(__ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON,"c:\\test3\\a.lzh");

		//一覧取得
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls(  );

		const RTable* rt = p.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1027728372.jpg ★ JPGファイル a ★ フォルダ b ★ フォルダ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
	{
		RfilesLzh p(__ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON,"c:\\test3\\test.lzh");

		//一覧取得
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);
		p.ls(  );

		const RTable* rt = p.getTable();

		//一覧を照合
		DisplayDebugPrintf ddp;
		ddp.setRfileItems( rt->getAllSize() , rt->getAllList() );

		string DebugString = "1039.jpg ★ JPGファイル 2get.jpg ★ JPGファイル メラゾーマ.jpg ★ JPGファイル 逝って良しケーキ.jpg ★ JPGファイル 名スレの予感.jpg ★ JPGファイル OpenOffice.org Calc.lnk ★ ショートカット OpenOffice.org Draw.lnk ★ ショートカット OpenOffice.org Writer.lnk ★ ショートカット abc ★ フォルダ bb b ★ フォルダ ";
		if ( ddp.DebugBuffer != DebugString )
		{
			TRACE("\n%s\n",ddp.DebugBuffer.c_str());
			ASSERT(0);
		}
	}
}
