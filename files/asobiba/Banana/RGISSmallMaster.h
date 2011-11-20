// RGISSmallMaster.h: RGISSmallMaster クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLMASTER_H__211C107A_7229_4B32_B347_97B21CA21692__INCLUDED_)
#define AFX_RGISSMALLMASTER_H__211C107A_7229_4B32_B347_97B21CA21692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDrawObject.h"
#include "RDIBBitmap.h"
#include "RGISSmall.h"
#include "RGISSmallIntelligentCache.h"
#include "RGISSmallFindEki.h"
#include "RGISSmallFindAddress.h"
#include "RGISCaption.h"
#include "RGISSmallPenFile.h"
#include "RGISSmallFindCityToNumberConvert.h"

class RGISSmallMaster  
{
public:
	RGISSmallMaster();
	virtual ~RGISSmallMaster();

	void Create(const string &inMapFilename , const string & inUserDataPath ,
				RDIBBitmap * poolDlib , RDIBBitmap * poolCaption,const RGISSmallShareImage* poolShareImage , 
				const RGISSmallPenFile	* inPenFile) throw(RException);
	void Draw();
	void FindEki(const string &inName , RGISSmallFindEki * ioFind);
	void FindAddress(unsigned long inCode ,const string &inName , unsigned long inBanchi , RGISSmallFindAddress * ioFind);
	void AllCityToCodeTable(RGISSmallFindCityToNumberConvert * ioFind);


	LPOINT getPos() const
	{
		return NowPoint;
	}
	int getScale() const
	{
		return Scale;
	}
	RAngle getDeg() const
	{
		return Deg;
	}
	void setPos(const LPOINT & inPoint) 
	{
		this->NowPoint = inPoint;
	}
	void setScale(int inScale , RAngle inDeg) 
	{
		if (inScale < 0 || inScale > 7) return ;

		this->Scale = inScale;
		this->Deg = inDeg;
	}

	const RGISSmallDrawEffect* getEffect() const
	{
		return &GISSmallEffect;
	}

	//ユーザーデータの編集.
	void UserDataModify(const LPOINT * inPoint , 
		const int * inDeleteNumber , int inDeleteCount ,
		const char * inData , int inSize,int inCount ) throw(RException)
	{
		GISSmallCahce.UserDataModify(inPoint , inDeleteNumber , inDeleteCount , inData ,inSize ,  inCount);
	}

	//ユーザーデータの追加.
	bool UserDataFind(const LRECT * inRect  , RGISSmallUserDataRead * outFindData ) throw(RException)
	{
		return GISSmallCahce.UserDataFind(inRect , outFindData);
	}



	//この場所を画面の中心に持ってきます。
	void LookAt(const LPOINT & inPoint);

	void MoveAt(int inX,int inY);
private:
	void NoCacheDraw();

	RGISSmallIntelligentCache	GISSmallCahce;

	const RGISSmallPenFile*	PenFile;

	RGISSmallDrawEffect		GISSmallEffect;
	RDrawObject				DrawObject;
	RGISCaption				Caption;

	LPOINT	NowPoint;
	int		Width;
	int		Height;
	int		Scale;
	RAngle	Deg;
};

#endif // !defined(AFX_RGISSMALLMASTER_H__211C107A_7229_4B32_B347_97B21CA21692__INCLUDED_)
