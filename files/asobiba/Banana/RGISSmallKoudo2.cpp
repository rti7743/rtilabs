// RGISSmallKoudo2.cpp: RGISSmallKoudo2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallKoudo2.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISSmallKoudo2::RGISSmallKoudo2()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallKoudo2::~RGISSmallKoudo2()
{
}

void RGISSmallKoudo2::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}

//線/点の描画と塗りつぶし
void RGISSmallKoudo2::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	if (this->Data == NULL) return ;

	inDraw->Boxes((unsigned char*) this->Data , inBlockStart , 
		BBananaConfig::getInstance()->getColors() + COLOR_MIZU );
}

//塗りつぶし
void RGISSmallKoudo2::Draw2(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
}




