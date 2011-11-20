// ImageListChacheWin32.h: ImageListChacheWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_)
#define AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDoNotInitaliseException.h"
#include "RImageList.h"

//ファイルを修飾するデータ(アイコン、ファイルタイプ)のそのもの.
class FileModifyData
{
private:
	FileModifyData(){}
protected:
	FileModifyData(int inIndex , string inFileType )
	{
		this->Index = inIndex;
		this->FileType = inFileType;
	}
public:
	int getIndex() const
		{		return this->Index;	}
	string getFileType() const
		{		return this->FileType;	}
	const string* getFileTypePointer() const		//危険すぎ
		{		return &this->FileType;	}	//
	//デバッグ
	void Dump()
	{
		printf("index as %d  " , Index);
		printf("%s  " , FileType.c_str());
		puts("");
	}
private:
	int Index;
	string FileType;
};

//ファイルを修飾するデータ(アイコン、ファイルタイプ)の参照回数付きで
//管理するクラス.
class FileModifyAdministrant : public FileModifyData
{
public:
	FileModifyAdministrant(int inIndex , string inFileType ) 
		: FileModifyData(inIndex,inFileType)
	{
		this->UseCount = 0;
	}
	//これ、使います。
	//使ういったら、使い終わったら needless を呼ぶように
	void Use()
	{
		this->UseCount++;
	}
	//必要ない
	void Needless()
	{
		this->UseCount--;
	}
	//消していいすか?
	bool IsDelete() const
	{
		return this->UseCount == 0;
	}
private:
	int UseCount;
};

//ファイルを修飾する(アイコン、ファイルタイプ)をキャッシュして、
//爆速で動作させるすげールーチン(w
class FileModifyChacheWin32  
{
public:
	//失敗は RDoNotInitaliseException 
	FileModifyChacheWin32(int inWidth,int inHegith,int inSystemChacheSize );
	virtual ~FileModifyChacheWin32();

	//検索.
	//見つからない場合は NULL.
	FileModifyData* Search(string inExt);
	//システムより取得
	FileModifyData* SearchSystem(int inPosition) const;

	//教える キャッシュしてほしいデータ
	FileModifyData* Tearch(string inExt,HICON inIcon , string inFileType);
	//教える キャッシュできないユニークなデータ
	FileModifyData* Tearch(HICON inIcon , string inFileType);

	//教える  キャッシュできないシステム系なデータ
	FileModifyData* TearchSystem(int inPosition , HICON inIcon , string inFileType);

	//使用する
	void Use(FileModifyData* inFileModifyData);

	//削除する
	void Delete(FileModifyData* inFileModifyData);

	//イメージリストの取得.
	//これにこのクラス外から書き込みした場合は結果は保証しない。
	const HIMAGELIST getImageList() const
	{
		return this->ImageList.getImageList();
	}
	//デバッグ
	void Dump();
private:
	//ターゲットを検索して、削除する
	//見つからない場合は false .... つまりシステム系のキャッシュです。
	bool SearchDelete(FileModifyAdministrant* inTarget);
	//システム系のキャッシュも含めて ALL あぼーん。
	void Clear();

private:
	//通常のキャッシュ
	typedef map<string , FileModifyAdministrant*> ICONCHACHE;
	ICONCHACHE			Chache;

	//キャッシュできないデータ ユニーク
	typedef deque<FileModifyAdministrant*> ICONONECHACHE;
	ICONONECHACHE		NoneChache;

	//キャッシュできないデータ システム
	FileModifyAdministrant**				NoneChacheSystem;
	int										NoneChacheSystemSize;

	RImageList			ImageList;
};

#endif // !defined(AFX_FILEMODIFYCHACHEWIN32_H__2D939D71_B954_4D8F_8684_8743164AB1CB__INCLUDED_)
