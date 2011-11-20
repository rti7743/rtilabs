// RfilesDebug.h: RfilesDebug クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_)
#define AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfilesDebug;

#include "comm.h"

#include "Rfiles.h"

class RfilesDebug  : public Rfiles
{
private:
public:
	RfilesDebug() {}
	virtual ~RfilesDebug(){}

	//一覧取得
	virtual void ls(RTable * rt){}
	//一覧取得
	virtual void ls(string inFilter,RTable * rt){}
	//削除
	virtual void rm(string inFilename){}
	//リネーム
	virtual void rename(string inSrcFilename,string inDestFilename){}
	//種類を返す.
	virtual __RFILETYPE		getRfileType()	const
	{
		return __RFILETYPE_DEBUG;
	}
	//RTableを作成します。
	//ls を使う前に呼んでください。
	//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw() {}
	//パスの取得
	virtual string getPath() const 
	{
		return "";
	}
	//パスの設定
	virtual void setPath(const string inPath)  
	{
		return ;
	}


};

#endif // !defined(AFX_RFILESDEBUG_H__FAF99701_109B_43BC_B5E7_8D53D326626C__INCLUDED_)
