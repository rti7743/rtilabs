// RfileLocalBigIcon.h: RfileLocalBigIcon クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILELOCALBIGICON_H__EA54FF66_AEB6_44AE_9C43_2363B19AD160__INCLUDED_)
#define AFX_RFILELOCALBIGICON_H__EA54FF66_AEB6_44AE_9C43_2363B19AD160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileLocalBigIcon;

#include "comm.h"


#include "RfileLocal.h"
#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"

//ローカルファイルで大きいアイコン
class RfileLocalBigIcon : public RfileLocal
{
public:
	RfileLocalBigIcon()
	{
		this->FileModify = NULL;
	}
	virtual ~RfileLocalBigIcon()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//登録します。
	virtual void Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib);

	//値の取得.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const;
	//イメージの取得.
	virtual const int getImageIndex() const;

	//ソートキーを登録します。 Attchの後に呼んでください.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey)
	{
		switch(inKey)
		{
		case __FILECOLUMN_TYPE_NAME:			//名前
			SortKey = (const void*) &this->Name;
			break;
		case __FILECOLUMN_TYPE_EXT:				//タイプ
			SortKey = (const void*) this->FileModify->getFileTypePointer();
			break;
		case __FILECOLUMN_TYPE_SIZE:			//サイズ
			SortKey = (const void*) &this->Size;
			break;
		case __FILECOLUMN_TYPE_DATE:			//更新日付
			SortKey = (const void*) &this->Date;
			break;
		case __FILECOLUMN_TYPE_ATTRIB:			//属性
			SortKey = (const void*) &this->Attrib;
			break;
		default:
			//サポート対象外.
			ASSERT(0);
			abort();
		}
	}

	//種類を返す.
	virtual __RFILETYPE	getType() const	
	{
		return __RFILETYPE_LOCALFILE_BIGICON;
	}
	//ディレクトリですか?
	virtual bool IsDirectory() const 
	{
		return Btob(this->Attrib & FILE_ATTRIBUTE_DIRECTORY) ;
	}

	//特殊なファイル修飾子の取得.
	static FileModifyData* NormalLoadModifty(string inExt , string inFullFilename );
	//特殊なファイル修飾子の取得.
	static FileModifyData* SpecialLoadModifty(string inExt , string inFullFilename ,
									FileModifyChacheLargeWin32::Modifty inDefaultIconIndex );
	//ディレクトリのファイル修飾子の取得.
	static FileModifyData* DirectoryLoadModifty(string inFullFilename );

private:
	string		Name;
	__int64		Size;
	__int64		Date;
	DWORD		Attrib;

	//ファイル修飾子(アイコン、ファイルタイプ)
	//共有ポインタでキャッシュしてます。
	//素人にはお勧めできない危険なルーチンです(w
	FileModifyData*	FileModify;

};

#endif // !defined(AFX_RFILELOCALBIGICON_H__EA54FF66_AEB6_44AE_9C43_2363B19AD160__INCLUDED_)
