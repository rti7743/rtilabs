// RGISSmallKawa.cpp: RGISSmallKawa クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallKawa.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISSmallKawa::RGISSmallKawa()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallKawa::~RGISSmallKawa()
{
}

void RGISSmallKawa::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//線/点の描画
void RGISSmallKawa::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallKawaDataOptimizMainData mainD = { 0 };
	SmallKawaDataOptimizUniqData d;

	ParticularLevel level = inDraw->getLevel();

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_KOUKYOU:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			//一条河川 と 二条かれ川
			if (0x55 == mainD.SB || 0x56 == mainD.SB)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB) , selectColor(mainD.SB) );
		}
	}
}

//文字列の描画
void RGISSmallKawa::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallKawaDataOptimizMainData mainD = { 0 };
	SmallKawaDataOptimizUniqData d;

	ParticularLevel level = inDraw->getLevel();
	RGISSmallDupKill dupKill;

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
//			inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			//一条河川 と 二条かれ川
//			if (0x55 == mainD.SB || 0x56 == mainD.SB)
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		}
	}
}


COLORREF RGISSmallKawa::selectColor(unsigned char inShubetsu) const
{
	return BBananaConfig::getInstance()->getColor(COLOR_MIZU);
/*
	if (0x55 == inShubetsu)				return COLOR_9;		//一条河川
	if (0x56 == inShubetsu)				return COLOR_9;		//二条河川
	if (0x57 == inShubetsu)				return COLOR_9;		//一条かれ川
	if (0x58 == inShubetsu)				return COLOR_9;		//二条かれ川
	if (0x59 == inShubetsu)				return COLOR_9;		//湖沼域内中心線

	ASSERT(0);
	return 0;
*/
}

COLORREF RGISSmallKawa::selectSize(unsigned char inShubetsu) const
{
	if (0x55 == inShubetsu)				return 1;		//一条河川
	if (0x56 == inShubetsu)				return 2;		//二条河川
	if (0x57 == inShubetsu)				return 1;		//一条かれ川
	if (0x58 == inShubetsu)				return 2;		//二条かれ川
	if (0x59 == inShubetsu)				return 2;		//湖沼域内中心線

	ASSERT(0);
	return 0;
}

//データの読み込み.
void RGISSmallKawa::readDataMain(RStdioMemory * ioFile, SmallKawaDataOptimizMainData * outData) const
{
	outData->SB = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//名前
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}


//データの読み込み.
void RGISSmallKawa::readDataUniq(RStdioMemory * ioFile, SmallKawaDataOptimizUniqData * outData) const
{
	//CVデータ(点)の取得
	outData->CVCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//点データの読み飛ばし
	outData->CV = ioFile->ptr();
	ioFile->fseek(  sizeof(unsigned short) * 2, SEEK_CUR );
	ioFile->fseek(  (outData->CVCount - 1) *  sizeof(unsigned char) * 2, SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);
}
