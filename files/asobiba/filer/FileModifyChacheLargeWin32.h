// FileModifyChacheLargeWin32.h: FileModifyChacheLargeWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_)
#define AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileModifyChacheWin32.h"
#include "RLock.h"

//大きなファイルのキャッシュ.
class FileModifyChacheLargeWin32  
{
public:
	enum Modifty
	{
		__SYSTEMICON_EXEFILE,
		__SYSTEMICON_NORMALFILE,
		__SYSTEMICON_NORMALFOLDER,
		__SYSTEMICON_SHAREMARK,
		__SYSTEMICON_SHORTCUTMARK,
		__SYSTEMICON_NETRESOURCE_DOMAIN,
		__SYSTEMICON_NETRESOURCE_WORKGROUP,
		__SYSTEMICON_NETRESOURCE_SERVER,
		__SYSTEMICON_NETRESOURCE_SHAREPOINT_DISK,
		__SYSTEMICON_MAX,				//登録されている数.
	};
	static const char*	const FOLDER_STRING; //= "フォルダ";
	static const char*	const EXEFILE_STRING; //= "実行ファイル";
	static const char*	const FILE_STRING; //= "ファイル";
private:
	FileModifyChacheLargeWin32();
public:
	virtual ~FileModifyChacheLargeWin32();
public:
	//singleton
	static FileModifyChacheLargeWin32* getInstance();

	//検索.
	//見つからない場合は NULL.
	FileModifyData* Search(string inExt)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Search(inExt);	
		if (p)
		{
			this->ChacheSystem->Use(p);
		}
		return p;
	}
	//システムより取得
	FileModifyData* SearchSystem(int inPosition)
	{
		RAutoLock al(&Lock);
		FileModifyData* p =  this->ChacheSystem->SearchSystem(inPosition);	

		ASSERT(p);
		this->ChacheSystem->Use(p);

		return p;
	}

	//教える キャッシュしてほしいデータ
	FileModifyData* Tearch(string inExt,HICON inIcon , string inFileType)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Tearch(inExt,inIcon,inFileType);
		this->ChacheSystem->Use(p);

		return p;
	}
	//教える キャッシュできないユニークなデータ
	FileModifyData* Tearch(HICON inIcon , string inFileType)
	{
		RAutoLock al(&Lock);
		FileModifyData* p = this->ChacheSystem->Tearch(inIcon,inFileType);	
		this->ChacheSystem->Use(p);

		return p;
	}

	//不要になったので削除
	void Delete(FileModifyData* inFileModifyData)
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Delete(inFileModifyData);
	}
	//コピー
	FileModifyData* Copy(FileModifyData* inFileModifyData)
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Use(inFileModifyData);
		return inFileModifyData;
	}

	//イメージリストの取得.
	//これにこのクラス外から書き込みした場合は結果は保証しない。
	const HIMAGELIST getImageList()
	{
		RAutoLock al(&Lock);
		return this->ChacheSystem->getImageList();
	}


	//システムアイコンとミックスしてユニークなアイコンの作成.
	//共有フォルダとか、ショーットカットとかについますぅ。
/*	不明.
	FileModifyData* MakeMix(HICON inIcon,int inPosition)
	{
	}
*/

	//デバッグ.
	void Dump()
	{
		RAutoLock al(&Lock);
		this->ChacheSystem->Dump();
	}

private:
	FileModifyChacheWin32*	ChacheSystem;
	RLock					Lock;
};

#endif // !defined(AFX_FILEMODIFYCHACHELARGEWIN32_H__68DE0520_BF64_4D43_86DC_15FF832C7594__INCLUDED_)
