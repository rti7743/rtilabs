// RMathAngle.cpp: RMathAngle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RMathAngle.h"
#include <math.h>



//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMathAngle::RMathAngle()
{
    int i;
    for (i = 0 ; i < 256 ; i ++) 
    {
        SinTable[i] = sin(RRR(i));
        CosTable[i] = cos(RRR(i));
	}
}

RMathAngle::~RMathAngle()
{

}

const RMathAngle* RMathAngle::getMathAngle()
{
	static RMathAngle a;
	return &a;
}



RMathAngleFixed::RMathAngleFixed()
{
    int i;
    for (i = 0 ; i < 256 ; i ++) 
    {
        SinTable[i] = sin(RRR(i)) * ( 1 << MY_FIXED_BIT);
        CosTable[i] = cos(RRR(i)) * ( 1 << MY_FIXED_BIT);
	}
}

RMathAngleFixed::~RMathAngleFixed()
{

}

const RMathAngleFixed* RMathAngleFixed::getMathAngleFixed()
{
	static RMathAngleFixed a;
	return &a;
}

int FastHypot(int a,int b)
{
	a=abs(a);
	b=abs(b);
	{
		register w=( ( a-b ) >> 31 ) & ( a^b ); /* a>bなら w=0  */
		return   ( a^w )+( b^w )/2;
	}
}
