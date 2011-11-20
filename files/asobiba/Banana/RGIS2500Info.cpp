// RGIS2500Info.cpp: RGIS2500Info クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGIS2500Info.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS2500Info::RGIS2500Info(const RGIS2500PointArray* inArray,const string & inName )
{
	//CVの数の取得...
	unsigned short count = inArray->getCount();

	UPOINT sumPoint = {0};
	unsigned short trueCVcount = 0;
	//とりあえず、すべてのデータをブロックごとに切り分けてみます。
	for(int i = 0 ;  i < count ; i++)
	{
		//精度を /100 にした結果、隣り合う点がまったく同じ場所に配置されている無駄な個所があります。
		//それを排除してデータサイズを小さくします.
		UPOINT getpoint= inArray->getPoint(i) ;
		sumPoint.x += getpoint.x;
		sumPoint.y += getpoint.y;
	}

	this->XY.x = (unsigned long) (sumPoint.x / count);
	this->XY.y = (unsigned long) (sumPoint.y / count);

	this->Name = inName;
}


RGIS2500Info::RGIS2500Info(const LPOINT* inPoint,const string & inName )
{
	this->XY = *inPoint;

	this->Name = inName;
}

