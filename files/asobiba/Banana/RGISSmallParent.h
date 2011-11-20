// RGISSmallParent.h: RGISSmallParent クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_)
#define AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISSmallDouro.h"
#include "RGISSmallTetudo.h"
#include "RGISSmallChimei.h"
#include "RGISSmallKawa.h"
#include "RGISSmallKoukyou.h"
#include "RGISSmallSuiikiKai.h"
#include "RGISSmallDouro.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallNameDatabase.h"
#include "RGISSmallAddress.h"
#include "RGISSmallKoudo2.h"
#include "RGISSmallUserData.h"
#include "RGISSmallParentImp.h"
#include "RGISSmallFindObject.h"
#include "RGISSmallTatemono.h"
#include "RGISSmallZyouti.h"
#include "RGISAllTest.h"

class RGISSmallParent  : public RGISSmallParentImp
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallParent();
	virtual ~RGISSmallParent();

	//シンボルの解決
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//データのセット
	virtual void setData(unsigned long inBlock ,char* ioBuffer , char * ioUserDataBuffer) ;
	//ユーザーデータのリロード.
	virtual void UserDataReload(char* ioUserDataBuffer) ;

	//こいつか所属するグループを求める.
	unsigned long getBlock() const
	{
		return Block;
	}
	//描画
	void Draw(RGISSmallDraw *inDraw ,const RECT & inClip);

	//駅を検索
	void FindEki(FindPool * outFindData , const string &inName , unsigned short inNameHash) const
	{
		Tetudo.FindEki(&BlockStart , outFindData , inName , inNameHash , &NameData);
	}
	//住所検索
	void FindAddress(FindPool * outFindData , const string &inName , int inBanchi) const
	{
		Address.FindAddress(&BlockStart , outFindData , inName , inBanchi , &AddressName);
	}
	//ユーザデータ検索
	int FindUserData(RGISSmallUserDataRead * outFindData ,int inDistance ,  const LRECT * inSeartchArea) const
	{
		return UserData.FindUserData(&BlockStart , outFindData ,inDistance, inSeartchArea);
	}

private:

private:
	unsigned long		Block;
	//地図データ.
	char*				Data;
	//ユーザ定義データ.
	char*				UserDataBuffer;

	LPOINT				BlockStart;

	RGISSmallDouro		Douro;
	RGISSmallTetudo		Tetudo;
	RGISSmallChimei		Chimei;
	RGISSmallKawa		Kawa;
	RGISSmallKoukyou	Koukyou;
	RGISSmallSuiikiKai	SuiikiKai;
	RGISSmallNameDatabase	NameData;
	RGISSmallAddress		Address;
	RGISSmallNameDatabase	AddressName;
	RGISSmallKoudo2			Koudo;
	RGISSmallUserData		UserData;

	RGISSmallTatemono		Tatemono;
	RGISSmallZyouti			Zyouti;
};

#endif // !defined(AFX_RGISSMALLPARENT_H__46A9C3F0_4BD8_44B4_BC76_92F3CA19AEC1__INCLUDED_)
