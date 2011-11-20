// RGISSmallKoukyou.cpp: RGISSmallKoukyou クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallKoukyou.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallKoukyou::RGISSmallKoukyou()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallKoukyou::~RGISSmallKoukyou()
{
}


void RGISSmallKoukyou::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}
/*
	this->Count = inDataCount;
	this->Points = new SmallKoukyouData[this->Count + 1];

	RStdioMemory memory;
	memory.Open(ioBuffer , inDataSize);
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		SmallKoukyouData* p = &this->Points[i];
		memory.fread( &p->SR , sizeof(unsigned char) );
		ASSERT(!memory.IsEof());

		memory.fread( &p->NameIndex , sizeof(unsigned short) );
		ASSERT(!memory.IsEof());

		p->PT.Create(&memory,inBlock);
	}

	ASSERT( memory.IsEof() );
}
*/

void RGISSmallKoukyou::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	//詳細のレベルが LEVEL_NOT_KOUKYOU より高ければ表示しない
	if (inDraw->getLevel() >= LEVEL_NOT_KOUKYOU ) return ;

	SmallKoukyouDataOptimizMainData mainD;
	SmallKoukyouDataOptimizUniqData d;
	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	ParticularLevel level = inDraw->getLevel();
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

		const char* name = inName->getPoint(d.NameIndex);

		switch(level)
		{
		case LEVEL_FULL:
			inDraw->Icon( inBlockStart, &d.xy , selectIcon(mainD.SR) , selectColor(mainD.SR) ,  inName->getPoint(d.NameIndex) );
			break;
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			if (mainD.SR > 0x84)
				inDraw->Icon( inBlockStart, &d.xy , selectIcon(mainD.SR) , selectColor(mainD.SR) ,  inName->getPoint(d.NameIndex) );
			break;
		}

//		inDraw->TextOut( inBlockStart, &d.xy ,selectColor(mainD.SR) ,  inName->getPoint(d.NameIndex) );
	}
}


//データの読み込み.
void RGISSmallKoukyou::readDataMain(RStdioMemory * ioFile, SmallKoukyouDataOptimizMainData * outData) const
{
	outData->SR = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

}

//データの読み込み.
void RGISSmallKoukyou::readDataUniq(RStdioMemory * ioFile, SmallKoukyouDataOptimizUniqData * outData) const
{
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->xy.x = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
	outData->xy.y = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
}

COLORREF RGISSmallKoukyou::selectColor(unsigned char inShubetsu) const
{
	return BBananaConfig::getInstance()->getColor(COLOR_MOJI);
/*
	if (0x81 == inShubetsu)				return COLOR_3;		//国の機関
	if (0x82 == inShubetsu)				return COLOR_3;		//地方公共団体
	if (0x83 == inShubetsu)				return COLOR_3;		//厚生機関
	if (0x84 == inShubetsu)				return COLOR_3;		//警察機関
	if (0x85 == inShubetsu)				return COLOR_3;		//消防署
	if (0x86 == inShubetsu)				return COLOR_3;		//学校
	if (0x87 == inShubetsu)				return COLOR_3;		//病院
	if (0x88 == inShubetsu)				return COLOR_3;		//郵便局

	ASSERT(0);
	return 0;
*/
}

int RGISSmallKoukyou::selectIcon(unsigned char inShubetsu) const
{
	if (0x81 == inShubetsu)				return 0;		//国の機関
	if (0x82 == inShubetsu)				return 0;		//地方公共団体
	if (0x83 == inShubetsu)				return 0;		//厚生機関
	if (0x84 == inShubetsu)				return 19;		//警察機関
	if (0x85 == inShubetsu)				return 16;		//消防署
	if (0x86 == inShubetsu)				return 18;		//学校
	if (0x87 == inShubetsu)				return 6;		//病院
	if (0x88 == inShubetsu)				return 20;		//郵便局

	ASSERT(0);
	return 0;
}
