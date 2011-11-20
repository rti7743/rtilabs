// RFiles.h: RFiles クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_)
#define AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Rfiles;

#include "comm.h"


#include "RTable.h"
#include "ReferenceCount.h"
#include "Rfile.h"
#include "RNotSupportException.h"


//仮想ディレクトリの最大深度.
const int MAX_DIMLIMITERES	=	128;


//RfilesWithCache で使用するファンクタ の実装部分
class CacheFileFnctional
{
public:
	//Rfile の作成.
	virtual Rfiles* New(__RFILETYPE inRfileType,const string inSrcPath) const throw() = 0;

	//ソースパスの取得
	virtual string getSrcPath(const string inPath) const = 0;
	//相対パスの取得
	virtual string getRelativePath(const string inPath) const = 0;

	//この inTargetFile を追加して・∀・）ｲｲ!!ですか?
	virtual bool IsAppend(const string inSrcPath, const Rfile* inTargetFile) const = 0;

	//RTable の作成.
	virtual RTable* ProxyCreateTable( const Rfiles* inSrcFiles,__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() = 0;
};

class Rfiles  : public ReferenceCount
{
private:
	//RTable ファイルを格納する場所.
	RTable*			Table;

public:
	Rfiles()
	{
		this->Table = NULL;
	}
protected:
	virtual ~Rfiles()
	{
		delete this->Table ;
	}
public:
	//ファイル名より拡張子を分離
	static	string Filename2Ext(const string inFilename)
	{
		const char* str = inFilename.c_str();
		for(int i = inFilename.length() - 1; str[i] != '.' && i >= 0 ; i--)
			;
		if (i < 0 )	return "";

		//拡張子を大文字に
		string ext = (char*)(str + i + 1);
		for(register int l = 0 ; ext[l] != '\0' ;l++) ext[l] = toupper(ext[l]);

		return ext;	// +1 は '.' の分です。
	}

	//一覧取得
	virtual void ls() = 0;
	//一覧取得
	virtual void ls(string inFilter) = 0;
	//削除
	virtual void rm(string inFilename) = 0;
	//リネーム
	virtual void rename(string inSrcFilename,string inDestFilename) = 0;
	//種類を返す.
	virtual __RFILETYPE		getRfileType()	const = 0;
	//RTableを作成します。
	//ls を使う前に呼んでください。
	//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw() = 0;
	//パスの取得
	virtual string getPath() const = 0;
	//パスの設定
	virtual void setPath(const string inPath) = 0;

	//書き込み不可能な RTableの取得.
	const RTable* getTable() const
	{
		ASSERT(this->Table != NULL);
		return this->Table;
	}

protected:
	//書き込み可能な RTableを取得
	RTable* getWritebleTable() const
	{
		ASSERT(this->Table != NULL);
		return this->Table;
	}
	//RTable を置き換える.
	void setTable(RTable* inRt)
	{
		ASSERT(inRt != NULL);

		delete this->Table;
		this->Table = inRt;
	}
};


#endif // !defined(AFX_RFILES_H__3E87AA04_2E48_457E_9234_622D9FB75F2C__INCLUDED_)
