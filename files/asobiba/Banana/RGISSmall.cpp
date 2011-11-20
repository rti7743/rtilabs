// RGISSmall.cpp: RGISSmall クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmall.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmall::RGISSmall()
{
}

RGISSmall::~RGISSmall()
{
}


void RGISSmall::Create(const string & inFilename , const string & inUserDataPath) throw(RException)
{
	File.Create(inFilename , inUserDataPath);
}


bool RGISSmall::Load(unsigned long inBlock)
{
	return File.Load(inBlock , &Parents);
}

void RGISSmall::Draw(const LRECT * inGlobalDisplay ,const LRECT * inDisplay , RDrawObject  * ioDraw , const RGISSmallDrawEffect* inEffect)
{
	RGISSmallDraw draw(ioDraw , inEffect);

	RECT a ={ 0 ,0 ,640 , 480};
	Parents.Draw(&draw,a);
}
