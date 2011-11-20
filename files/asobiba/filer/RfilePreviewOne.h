// RfilePreviewOne.h: RfilePreviewOne クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_)
#define AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RfileLocal.h"

//プレビュー用の一つのファイルを表す。
class RfilePreviewOne : public Rfile
{
public:
	RfilePreviewOne()
	{
		//ソートなんてしないし。
		SortKey = NULL;
	}
	virtual ~RfilePreviewOne()
	{
	}

	//登録します。
	void Attach(const char* inName)
	{
		this->Name = inName;
	}
	//ソートキーを登録します。 Attchの後に呼んでください.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey)
	{
	}

	//値の取得.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const
	{
		if (inType == __FILECOLUMN_TYPE_NAME) return Name;
		return "";
	}
	//イメージの取得.
	virtual const int getImageIndex() const
	{
		ASSERT(0);
		return 0;
	}

	//種類を返す.
	virtual __RFILETYPE	getType() const	
	{
		return __RFILETYPE_PREVIEWONE;
	}
	//ディレクトリですか?
	virtual bool IsDirectory() const 
	{
		return false ;
	}
private:
	string	Name;
};

#endif // !defined(AFX_RFILEPREVIEWONE_H__34A51F10_0671_4A75_A119_E0E0A9D1FC93__INCLUDED_)
