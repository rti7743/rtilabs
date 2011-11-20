// PathInterface.h: PathInterface クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_)
#define AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PathInterface  ;

#include "comm.h"
#include "Rfile.h"
#include "Rfiles.h"
#include "RFixedColumn.h"
#include "Display.h"


//実パスの格納.
class PathInterface  
{
public:
	PathInterface()	{	}
	virtual ~PathInterface()	{	}

	//パスの取得.
	string getPath() const
	{
		return this->Path;
	}

	//種類を返す.
	virtual __RFILETYPE getType() const = 0;
	//このディスプレイはサポートしてますか?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const = 0;
	//最も推奨するディスプレイは、何ですか?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const = 0;
	//最も推奨するディスプレイでのカラムを教えてください
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const = 0;
	//対応する Rfiles の作成.
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const = 0;
	//一つディレクトリを上がる
	virtual string UpDirectory() const = 0;
protected:
	//パスを設定.
	void	setPath(string inPath)
	{
		this->Path = inPath;
	}

private:
	string		Path;
};

#endif // !defined(AFX_PATHINTERFACE_H__A916C423_B594_467B_9F68_9CEBACD3D8E0__INCLUDED_)
