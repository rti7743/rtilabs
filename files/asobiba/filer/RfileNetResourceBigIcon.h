// RfileNetResourceBigIcon.h: RfileNetResourceBigIcon クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_)
#define AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileNetResourceBigIcon;

#include "comm.h"

#include "RfileNetResource.h"
#include "Rfiles.h"
#include "FileModifyChacheLargeWin32.h"


class RfileNetResourceBigIcon : public RfileNetResource  
{
public:
	RfileNetResourceBigIcon()
	{
		this->FileModify = NULL;
	}
	virtual ~RfileNetResourceBigIcon()
	{
		FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
		fmc->Delete( this->FileModify );
		this->FileModify = NULL;
	}

	//登録します。
	virtual void Attach(const char* inName,const char* inComment,DWORD inDisplayType );

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
		case __FILECOLUMN_TYPE_COMMENT:			//コメント
			SortKey = (const void*) &this->Comment;
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
		return __RFILETYPE_SMBLOGON_BIGICON;
	}
	//ディレクトリですか?
	virtual bool IsDirectory() const 
	{
		//ディレクトリという概念がねー
		return false;
	}


private:
	string		Name;
	string		Comment;

	//ファイル修飾子(アイコン、ファイルタイプ)
	//共有ポインタでキャッシュしてます。
	//素人にはお勧めできない危険なルーチンです(w
	FileModifyData*	FileModify;


};

#endif // !defined(AFX_RFILENETRESOURCEBIGICON_H__607ADF58_51C9_4323_82D7_A1A6E2E0BCE5__INCLUDED_)
