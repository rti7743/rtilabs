// RGISSmallUserData.cpp: RGISSmallUserData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallUserData.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallUserData::RGISSmallUserData()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallUserData::~RGISSmallUserData()
{
}



void RGISSmallUserData::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//文字列の描画
void RGISSmallUserData::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	RGISSmallUserDataRead d;
	const char* data = Data;

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		data = d.Create(inBlockStart , data , DataSize , i);

		//名前があったら表示する.
		try
		{
			const char * atData = d.FindAttributeStr(USERDATA_NAME);
			unsigned char icon = d.FindAttributeC(USERDATA_ICON);
			switch(inDraw->getLevel())
			{
			case LEVEL_FULL:
			case LEVEL_NOT_BANCHI:
			case LEVEL_NOT_CHIMEI:
			case LEVEL_NOT_KOUKYOU:
				inDraw->Icon(d.getXY() , icon , 1, atData );
				break;
			case LEVEL_NOT_LOWLEVEL:
			case LEVEL_NOT_MIDDLELEVEL:
				break;
			}
		}
		catch(RNotFoundException e)
		{
		}
	}
}



//ユーザーデータの検索.
int RGISSmallUserData::FindUserData(const LPOINT *inBlockStart ,RGISSmallUserDataRead * outFindData ,int inDistance , const LRECT * inRect  ) const
{
	int mostnear = inDistance;
	const char* data = Data;

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		RGISSmallUserDataRead d;
		data = d.Create(inBlockStart , data , DataSize , i);

		if ( 	d.getXY()->x >= inRect->left && d.getXY()->x <= inRect->right && 
				d.getXY()->y >= inRect->top  && d.getXY()->y <= inRect->bottom  )
		{
			int distance = FastHypot( d.getXY()->x - inRect->left ,d.getXY()->y -  inRect->top);
			if (distance < mostnear)
			{
				mostnear = distance;
			}
		}
	}

	return mostnear;
}





//作成.
const char* RGISSmallUserDataRead::Create(const LPOINT * inBlockStart ,const char * inData , int inDataSize , int inSerial)
{
	Serial = inSerial;

	RStdioMemory memory;
	memory.Open(inData );

	XY.x = memory.readGetUS( ) + inBlockStart->x;
	ASSERT(memory.ftell() <= inDataSize);
	XY.y = memory.readGetUS( ) + inBlockStart->y;
	ASSERT(memory.ftell() <= inDataSize);

	memset(Attribute , 0 , sizeof(Attribute) );

	unsigned char type;
	unsigned char size;
	do
	{
		//属性データの読み込み.
		type = memory.readGetUC( );
		size = memory.readGetUC( );

		if (type >= 0 && type <= 63)			//文字列
		{
			Attribute[type].str = memory.ptr( );
			memory.fseek(  sizeof(char) * size, SEEK_CUR );
		}
		else if (type >= 64 && type <= 127)		//int
		{
			ASSERT(size == 4);
			Attribute[type].lval = memory.readGetUL( );
		}
		else if (type >= 128 && type <= 191)	//short
		{
			ASSERT(size == 2);
			Attribute[type].sval = memory.readGetUS( );
		}
		else if (type >= 192 && type <= 254)	//char
		{
			ASSERT(size == 1);
			Attribute[type].cval = memory.readGetUC( );
		}
	}			
	while(type != 0xff);


	return memory.ptr();
}

/*
X		ul
Y		ul
size	uc
color	uc
*/
