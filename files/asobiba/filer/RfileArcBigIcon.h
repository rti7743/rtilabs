// RfileArcBigIcon.h: RfileArcBigIcon クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_)
#define AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileArcBigIcon;

#include "comm.h"


#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"
#include "RfileArc.h"

//圧縮ファイルで大きいアイコン.
class RfileArcBigIcon : public RfileArc  
{
public:
	RfileArcBigIcon()
	{
		this->FileModify = NULL;
	}
	RfileArcBigIcon(const RfileArcBigIcon& inSrc)
	{
		Path = inSrc.Path;
		Name = inSrc.Name;
		Size = inSrc.Size;
		Date = inSrc.Date;
		Ratio = inSrc.Ratio;
		Mode = inSrc.Mode;

		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		FileModify = fmc->Copy( inSrc.FileModify );
	}
	virtual ~RfileArcBigIcon()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//登録します。
	virtual void Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute);

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
		case __FILECOLUMN_TYPE_RATIO:			//圧縮レート(圧縮率)
			SortKey = (const void*) &this->Ratio;
			break;
		case __FILECOLUMN_TYPE_ARCMODE:			//圧縮モード
			SortKey = (const void*) &this->Mode;
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
		return __RFILETYPE_ARC_BIGICON;
	}
	//ディレクトリですか?
	virtual bool IsDirectory() const 
	{
		//ディレクトリという概念がねー
		return false;
	}

	//特殊なファイル修飾子の取得.
	static FileModifyData* NormalLoadModifty(string inExt , string inFullFilename );
	//ディレクトリのファイル修飾子の取得.
	static FileModifyData* DirectoryLoadModifty();

private:
	string		Path;	//アーカイバ内のパス
	string		Name;
	DWORD		Size;
	DWORD		Date;
	WORD		Ratio;
	string		Mode;

	//ファイル修飾子(アイコン、ファイルタイプ)
	//共有ポインタでキャッシュしてます。
	//素人にはお勧めできない危険なルーチンです(w
	FileModifyData*	FileModify;


};

#endif // !defined(AFX_RFILEARCBIGICON_H__D040C42C_6610_4D8D_9E7A_5AD0E4AF614B__INCLUDED_)
