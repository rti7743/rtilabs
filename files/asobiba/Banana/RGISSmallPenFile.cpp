// RGISSmallPenFile.cpp: RGISSmallPenFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallPenFile.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallPenFile::RGISSmallPenFile()
{
	this->LastXY.x = 0;
	this->LastXY.y = 0;
}

void RGISSmallPenFile::Load(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	int count = file.getSize() / (sizeof(unsigned long) * 2 + sizeof(unsigned char) + sizeof(unsigned char) );
	this->Vec.reserve(count);
	this->Vec.resize(count);

	for(int i = 0 ;  ! file.IsEof() ; i ++)
	{
		RGISSmallPenFilePoint * p = &this->Vec[i];
		file.fread(& (p->xy.x)  , sizeof(p->xy.x) );
		file.fread(& (p->xy.y)  , sizeof(p->xy.y) );
		file.fread(& (p->size)  , sizeof(p->size) );
		file.fread(& (p->color) , sizeof(p->color) );
	}
}

void RGISSmallPenFile::Save(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "wb");

	const RGISSmallPenFilePoint * p = &this->Vec[0];
	const RGISSmallPenFilePoint * e = &this->Vec[this->Vec.size()];
	for( ; p != e; p++)
	{
		file.fwrite(& (p->xy.x)  , sizeof(p->xy.x) );
		file.fwrite(& (p->xy.y)  , sizeof(p->xy.y) );
		file.fwrite(& (p->size)  , sizeof(p->size) );
		file.fwrite(& (p->color) , sizeof(p->color) );
	}
}

void RGISSmallPenFile::Push(const LPOINT * inXY , unsigned char inSize , unsigned char inColor)
{
	if (inColor == 0xff)
	{
		Erase(inXY,inSize);
	}
	else
	{
		RGISSmallPenFilePoint p;
		p.xy = *inXY;
		p.size = inSize;
		p.color = inColor;

		this->Vec.push_back(p);
	}
}


void RGISSmallPenFile::Erase(const LPOINT * inXY , unsigned char inSize)
{
	LRECT rc;
	int half = inSize / 2;
	rc.left = inXY->x - half;
	rc.top = inXY->y - half;
	rc.right = inXY->x + half;
	rc.bottom = inXY->y + half;

	//ヒットしたデータを後ろに集める.
	RGISSmallPenFilePoint * p = &this->Vec[0];
	RGISSmallPenFilePoint * e = &this->Vec[this->Vec.size()];
	for( ; p != e;)
	{
		if (rc.left  < p->xy.x && rc.right  > p->xy.x &&
			rc.right < p->xy.y && rc.bottom > p->xy.y )
		{
			RGISSmallPenFilePoint t;
			t = *p;			*p = *e;			*e = t;	//swap
			e --;
		}
		else
		{
			p++;
		}
	}
	this->Vec.resize( e - (&this->Vec[0]) );
}



void RGISSmallPenFile::Draw(RGISSmallDraw * ioDraw) const
{
	if (this->Vec.empty() ) return ;

	const RGISSmallPenFilePoint * p = &this->Vec[0];
	const RGISSmallPenFilePoint * e = &this->Vec[this->Vec.size()];
	for( ; p != e; p++)
	{
		ioDraw->TranslucenceBox(&p->xy , p->size , p->color);
	}
}

void RGISSmallPenFile::LastClear()
{
	this->LastXY.x = 0;
	this->LastXY.y = 0;
}

void RGISSmallPenFile::Line(const LPOINT * inXY , unsigned char inSize , unsigned char inColor)
{
	if (this->LastXY.x == 0 && this->LastXY.y == 0)
	{
		this->LastXY = *inXY;
		return ;
	}

	LPOINT xyMid;
	xyMid.x = inXY->x - (inSize / 2);
	xyMid.y = inXY->y - (inSize / 2);

	//最後の値から、現在の inXY までの直線を引きます.
	//移動量
	int dx,dy,xincr,yincr;
	if( this->LastXY.x > xyMid.x ) 
	{
		xincr=-1;
		dx = this->LastXY.x - xyMid.x; 
	}
	else
	{
		xincr=1;
		dx = xyMid.x - this->LastXY.x;
	}
	if( this->LastXY.y > xyMid.y ) 
	{
		yincr=-1;
		dy = this->LastXY.y - xyMid.y; 
	}
	else
	{
		yincr=1;
		dy = xyMid.y - this->LastXY.y; 
	}
	int e2x = 2 * dx;
	int e2y = 2 * dy;

	LPOINT xy = this->LastXY;

	if (dy < dx)
    {                    // 点線にしない為
        //X 方向 を 増加する場合
        int e = -dx;
        for(int i = 0 ; i <= dx ; i++)
        {
			Push(&xy , inSize , inColor);
            xy.x = xy.x + xincr;                 // X 方向

            e = e + e2y;             // DDA による補間
            if (e >= 0) 
            {
                xy.y = xy.y + yincr ;            // Y方向
                e = e - e2x;         // DDA による補間
            }
        }
    }
    else
    {
        // Y 方向 を 増加する場合
        int e = -dy;
        for(int i = 0 ; i <= dy ; i ++)
        {
			Push(&xy , inSize , inColor);
            xy.y = xy.y + yincr;                  // Y 方向
            e = e + e2x;              // DDA による補間
            if (e >= 0) 
            {
                xy.x = xy.x + xincr ;             // X 方向
                e = e - e2y;          // DDA による補間
            }
        }
    }
	this->LastXY = xyMid;
	
}


