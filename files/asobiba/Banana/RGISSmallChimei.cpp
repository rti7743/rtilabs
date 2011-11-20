// RGISSmallChimei.cpp: RGISSmallChimei クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallChimei.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallChimei::RGISSmallChimei()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallChimei::~RGISSmallChimei()
{
}


void RGISSmallChimei::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}
/*
	this->Count = inDataCount;
	this->Points = new SmallChimeiData[this->Count + 1];

	RStdioMemory memory;
	memory.Open(ioBuffer , inDataSize);
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		SmallChimeiData* p = &this->Points[i];
		memory.fread( &p->SR , sizeof(unsigned char) );
		ASSERT(!memory.IsEof());

		memory.fread( &p->NameIndex , sizeof(unsigned short) );
		ASSERT(!memory.IsEof());

		p->PT.Create(&memory,inBlock);
	}

	ASSERT( memory.IsEof() );
}
*/

void RGISSmallChimei::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	//詳細のレベルが LEVEL_NOT_CHIMEI より高ければ表示しない
	if (inDraw->getLevel() >= LEVEL_NOT_CHIMEI ) return ;

	SmallChimeiDataOptimizMainData mainD;
	SmallChimeiDataOptimizUniqData d;
	mainD.LoopCount = 0;

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

		inDraw->TextOut( inBlockStart, &d.xy , selectColor(mainD.SR) ,  inName->getPoint(d.NameIndex) );
	}
}

COLORREF RGISSmallChimei::selectColor(unsigned char inShubetsu) const
{
	return BBananaConfig::getInstance()->getColor(COLOR_MOJI);
/*
	if (0x91 == inShubetsu)				return COLOR_3;		//自然地名
	if (0x92 == inShubetsu)				return COLOR_3;		//土地の利用景
	if (0x93 == inShubetsu)				return COLOR_3;		//居住地名

	ASSERT(0);
	return 0;
*/
}

//データの読み込み.
void RGISSmallChimei::readDataMain(RStdioMemory * ioFile, SmallChimeiDataOptimizMainData * outData) const
{
	outData->SR = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}
//データの読み込み.
void RGISSmallChimei::readDataUniq(RStdioMemory * ioFile, SmallChimeiDataOptimizUniqData * outData) const
{
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->xy.x = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
	outData->xy.y = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);
}
