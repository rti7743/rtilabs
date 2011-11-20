// MVFileList.h: MVFileList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVFILELIST_H__CE5F3F17_377D_406E_B457_7940AB7AB451__INCLUDED_)
#define AFX_MVFILELIST_H__CE5F3F17_377D_406E_B457_7940AB7AB451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RStdioFile.h"


struct ZipReadInfo
{
	char*				buffer;
	unsigned long		compressedSize;
	unsigned long		uncompressedSize;
	unsigned short		compressionMethod;
};


class MVFileList  
{
public:
	MVFileList();
	virtual ~MVFileList();

	void MVFileList::Update(const string & inZIPname) throw(RException);

	int getCount() const
	{
		return PKChain.size();
	}

	//ページの情報を取得します.
	void getInfo(int inIndex , ZipReadInfo * outInfo);

	static void MVFileList::test();



private:
	struct ZipPos
	{
		unsigned long		tell;	//ファイルの中での位置
		unsigned long		compressedSize;
		unsigned long		uncompressedSize;
		unsigned short		compressionMethod;
	};
	newVector<ZipPos>		PKChain;
	RStdioFile		File;


};

#endif // !defined(AFX_MVFILELIST_H__CE5F3F17_377D_406E_B457_7940AB7AB451__INCLUDED_)
