// RGISSmallType.cpp: RGISSmallType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallType.h"

const int NEXT_LINE_BLOCK	= 4096;


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/*
RGISSmallNode::RGISSmallNode()
{
	this->Count = 0;
	this->Points = NULL;
}
*/
RGISSmallNode::~RGISSmallNode()
{
	Delete();
}


void RGISSmallNode::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}


void RGISSmallNode::Create(RStdioMemory * inMemory, unsigned long inBlock)
{
//	Delete();

	unsigned char ndCount;
	inMemory->fread(&ndCount , sizeof(ndCount) );
	if (ndCount == 0) return ;

	this->Count = ndCount;
	this->Points = new unsigned char[ndCount];

	unsigned char node;
	for(int i = 0;  i < ndCount ; i++)
	{
		inMemory->fread( &node , sizeof(unsigned char) );

		this->Points[i] = node;
	}
}

/*
RGISSmallType::RGISSmallType()
{
	this->Count = 0;
	this->Points = NULL;
}
*/
RGISSmallType::~RGISSmallType()
{
	Delete();
}


void RGISSmallType::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}


void RGISSmallType::Create(RStdioMemory * inMemory, unsigned long inBlock)
{
//	Delete();

	unsigned char cvCount;
	inMemory->fread(&cvCount , sizeof(cvCount) );
	if (cvCount == 0) return ;

	this->Count = cvCount;
	this->Points = new LPOINT[cvCount];

	unsigned short x;
	unsigned short y;
	for(int i = 0;  i < cvCount ; i++)
	{
		inMemory->fread( &x , sizeof(unsigned short) );
		inMemory->fread( &y , sizeof(unsigned short) );

		this->Points[i] = BlockToPoint(x,y,inBlock);
	}
}


LPOINT RGISSmallType::BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock) 
{
	LPOINT p;
	p.x = ((unsigned long)inBlock % NEXT_LINE_BLOCK) * (unsigned long)65536 + inX;
	p.y = ((unsigned long)inBlock / NEXT_LINE_BLOCK) * (unsigned long)65536 + inY;

	//オーバーフローチェック.
	ASSERT( (inBlock % NEXT_LINE_BLOCK) <= ((unsigned __int64)inBlock % NEXT_LINE_BLOCK) * (unsigned __int64)65536 + inX );
	ASSERT( (inBlock / NEXT_LINE_BLOCK) <= ((unsigned __int64)inBlock / NEXT_LINE_BLOCK) * (unsigned __int64)65536 + inY );

	//int64は必要ですか?
	ASSERT( ( (inBlock % NEXT_LINE_BLOCK) * 65536 + inX) == p.x);
	ASSERT( ( (inBlock / NEXT_LINE_BLOCK) * 65536 + inY) == p.y);

	return p;
}

LPOINT RGISSmallType::AvgCV(const LPOINT * inBlockStart , const char* inCVData , int inCVCount)
{
	RStdioMemory memory;
	memory.Open(inCVData);

	LPOINT point; 
	const int avg = inCVCount / 2;	//ちょうど真中当たり??

	int x;
	int y;
	x = memory.readGetUS();
	y = memory.readGetUS();
	//目的の場所まで移動.
	for(int i = 1 ; i < avg ; i++)
	{
		x += memory.readGetC();
		y += memory.readGetC();
	}
	point.x = inBlockStart->x + x;
	point.y = inBlockStart->y + y;

	return point;
}



//ブロックに分断
unsigned long RGISSmallType::selectBlock(const LPOINT * inPT) 
{
	return (inPT->y / 65536) * (NEXT_LINE_BLOCK) + (inPT->x /65536);
}



/*
RGISSmallPoint::RGISSmallPoint()
{
	this->Point.x = 0;
	this->Point.y = 0;
}
*/
RGISSmallPoint::~RGISSmallPoint()
{
}




//このクラスにメモリの管理を委譲する
//以後 inSrcArray は、このクラスの管理となるので、勝手に破棄しないこと!!
void RGISSmallPoint::Create(RStdioMemory * inMemory, unsigned long inBlock)
{
	unsigned short x;
	unsigned short y;
	inMemory->fread( &x , sizeof(unsigned short) );
	inMemory->fread( &y , sizeof(unsigned short) );

	this->Point = RGISSmallType::BlockToPoint(x,y,inBlock);
}

