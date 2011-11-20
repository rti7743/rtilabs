// RGISSmallIntelligentCache.h: RGISSmallIntelligentCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_)
#define AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallFileRead.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallParent.h"
#include "RZlib.h"
#include "RGISAllTest.h"
#include "RGISSmallPenFile.h"

//キャッシュする地域データの数
const int  GIS_SMALL_CACHE_MAX	=	10;


class RGISSmallIntelligentCache  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallIntelligentCache();
	virtual ~RGISSmallIntelligentCache();

	//作成
	void Create(const string & inFilename , const string & inUserDataPath) throw(RException);

	void New(RDrawObject  * ioDraw  ,const RGISSmallDrawEffect* inEffect);

	//指定されたブロックを読み込み
	const RGISSmallParent* Load(unsigned long inBlock);
	//指定されたブロックをキャッシュとは関係なく読み込み(キャッシュを参照することはしない)
	bool LoadEx(unsigned long inBlock , RGISSmallParentImp* ioParentImp);

	void Find(RGISSmallHeaderImp* ioFindHeader);

	//ユーザーデータの編集.
	void UserDataModify(const LPOINT * inPoint , const int * inDeleteNumber , int inDeleteCount ,
					    const char * inData , int inSize,int inCount ) throw(RException);
	//ユーザーデータの検索.
	bool UserDataFind(const LRECT * inSearchArea , RGISSmallUserDataRead * outUserData ) throw(RException);


private:
	//キャッシュの破棄.
	void Destroy();
	//この範囲に含まれますか?
	bool isHit(const LRECT * inHitBox , unsigned long inBlock) const;
	//Hit検索に使いやすいように、あらかじめ計算しておきます。
	LRECT ConvertHitBox(const LRECT * inDisplay) const;
	//キャッシュに指定したキーがあるかどうか検索.
	bool findCache(unsigned long inKey);
	//ブロックからディスプレイ座標に変換.
	RECT BlockToDisplayRect(const RDrawObject *inDraw ,unsigned long inBlock , const RGISSmallDrawEffect* inEffect) const;
	//指定したブロックを読み込む 内部向け
	RGISSmallParent* RGISSmallIntelligentCache::LoadPrivate(unsigned long inBlock);

private:
	RZlib	Zlib;

	//データ
	RGISSmallParent*	Cache[GIS_SMALL_CACHE_MAX];
	//マップファイル.
	RGISSmallFileRead	File;
};

#endif // !defined(AFX_RGISSMALLINTELLIGENTCACHE_H__DF0E867C_897A_43F7_909B_F504D5E0E45E__INCLUDED_)
