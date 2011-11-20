// RImageList.h: RImageList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_)
#define AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include <SHLOBJ.H>

#include "RDoNotInitaliseException.h"

class RImageList  
{
public:
	RImageList();
	virtual ~RImageList();

	//イメージリストの作成
	//作成失敗は RDoNotInitaliseException .
	void Create(int inWidth,int inHegith,int inInitCount,int inMax) throw();
	//イメージリストの破棄
	void Destroy();

	//追加
	int Add(HICON inIcon);
	//指定した index を削除
	//勝手につめたりしないので、ママも安心です。
	void Remove(int inIndex);
	//全部消す.
	void RemoveAll();

	//イメージリストの取得.
	//これにこのクラス外から書き込みした場合は結果は保証しない。
	const HIMAGELIST getImageList() const
	{
		return this->ImageList;
	}

private:
	//消された位置を記録している.
	typedef deque<int>	REMOVELIST;
	REMOVELIST			RemoveList;

	HIMAGELIST			ImageList;
};

#endif // !defined(AFX_RIMAGELIST_H__0501EA72_A166_4D62_8EA6_7BC2F9319E41__INCLUDED_)
