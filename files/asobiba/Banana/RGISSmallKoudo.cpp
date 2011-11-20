// RGISSmallKoudo.cpp: RGISSmallKoudo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallKoudo.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISSmallKoudo::RGISSmallKoudo()
{
	this->Points = NULL;
	this->Count = 0;
}

RGISSmallKoudo::~RGISSmallKoudo()
{
	Delete();
}


void RGISSmallKoudo::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}


void RGISSmallKoudo::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,char* ioBuffer) throw(RException)
{
	this->Count = inDataCount;
	this->Points = new SmallKoudoData[this->Count + 1];

	RStdioMemory memory;
	memory.Open(ioBuffer , inDataSize);
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		SmallKoudoData* p = &this->Points[i];
		memory.fread( &p->Level , sizeof(unsigned char) );
		ASSERT(!memory.IsEof());

		p->CV.Create(&memory,inBlock);
	}

	ASSERT( memory.IsEof() );
}


//線/点の描画と塗りつぶし
void RGISSmallKoudo::Draw1(const RGISSmallDraw *inDraw)
{
	//線を書く
	for(int i = 0 ; i < this->Count ; i++ )
	{
		SmallKoudoData* p = &this->Points[i];
		inDraw->Polygon( &p->CV , selectSize(p->Level) , selectColor(p->Level) );
	}

}

//塗りつぶし
void RGISSmallKoudo::Draw2(const RGISSmallDraw *inDraw)
{
	//塗りつぶし
	for(int i = 0 ; i < this->Count ; i++ )
	{
		SmallKoudoData* p = &this->Points[i];
		inDraw->Paint( &p->CV ,  selectColor(p->Level) );
	}

}




COLORREF RGISSmallKoudo::selectColor(unsigned char inLevel) const
{
	if (inLevel == 10 ) return HCOLOR_1;
	if (inLevel == 11 ) return HCOLOR_2;
	if (inLevel == 12 ) return HCOLOR_3;
	if (inLevel == 13 ) return HCOLOR_4;
	if (inLevel == 14 ) return HCOLOR_5;
	if (inLevel == 15 ) return HCOLOR_6;
	if (inLevel == 16 ) return HCOLOR_7;
	if (inLevel == 17 ) return HCOLOR_8;
	if (inLevel == 18 ) return HCOLOR_9;
	if (inLevel == 19 ) return HCOLOR_10;
	if (inLevel == 20 ) return HCOLOR_11;
	return HCOLOR_12;
}

COLORREF RGISSmallKoudo::selectSize(unsigned char inLevel) const
{
	return 1;
}
