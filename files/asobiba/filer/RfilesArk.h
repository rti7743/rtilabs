// RfilesArk.h: RfilesArk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_)
#define AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableEasy.h"

#include "RfileLocalBigIcon.h"
#include "RfileLocalReport.h"

//圧縮ディスクファイル一覧の取得.
template <class _RFILE>
class RfileArk : public Rfiles  
{
private:
	//よべませんよ
	RfilesLocal() : Rfiles("") {}
public:
	RfilesLocal(string inPath) : Rfiles(inPath) {}
	virtual ~RfilesLocal(){}

	//一覧取得
	virtual void ls()
	{
		WIN32_FIND_DATA data;
		HANDLE p;

		RTable * rt = getWritebleTable();

		rt->clear();

		string wildpath = this->getPath() + "*.*";
		__FILECOLUMN_TYPE sortkey = rt->GetSortKey();
		p = ::FindFirstFile( wildpath.c_str() , &data);
		if (p != INVALID_HANDLE_VALUE)
		{
			do
			{
				// . または .. のファイルは無視.
				if ( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
					(strcmp(data.cFileName , ".") ==0 ||
					 strcmp(data.cFileName , "..")==0) )	continue;

				_RFILE* p = new _RFILE;
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
	virtual void ls(string inFilter)
	{
	}
	//削除
	virtual void rm(string inFilename)
	{
	}
	//リネーム
	virtual void rename(string inSrcFilename,string inDestFilename)
	{
	}
	//種類を返す.
	virtual __RFILETYPE		getType()	const
	{
		return __RFILETYPE_LOCALFILE_REPORT;
	}

	//RTableを作成します。
	//ls を使う前に呼んでください。
	//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
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
	static void test()
	{
		{
			RfilesLocal<RfileLocalReport> p("c:\\test\\");

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
			RfilesLocal<RfileLocalReport> p("c:\\t国崎最高!!");

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
};

#endif // !defined(AFX_RFILESARK_H__BC5F2C57_9FDB_4B34_8C14_680F7CCB186A__INCLUDED_)
