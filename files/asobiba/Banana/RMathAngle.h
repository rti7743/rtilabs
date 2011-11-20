// RMathAngle.h: RMathAngle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_)
#define AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//1円 = 256度 で荒らすと一周しても 0 になるので安心.
typedef unsigned char RAngle ;
#define RevAngle(x) (0xff- (x) )

//360度形式を 256度形式に変換.
#define ConvertAngle360To256(x) ((x) * 256 / 360)


//ルックアップテーブルを用いた sin , cos
class RMathAngle  
{
private:
	RMathAngle();	//singleton
public:
	~RMathAngle();

	static const RMathAngle* getMathAngle();

	double Sin(RAngle inDeg) const
	{
		return SinTable[inDeg];
	}
	double Cos(RAngle inDeg) const
	{
		return CosTable[inDeg];
	}
private:
	double SinTable[256];
	double CosTable[256];
};
//固定小数ルックアップテーブルを用いた sin , cos
#define MY_FIXED_BIT	10	//固定小数の小数てんの部分のビット数
typedef long	RFIXED;

#define ItoF(x)	((x) << MY_FIXED_BIT)
#define FtoI(x)	((x) >> MY_FIXED_BIT)

class RMathAngleFixed
{
private:
	RMathAngleFixed();			//singleton
public:
	~RMathAngleFixed();

	static const RMathAngleFixed* getMathAngleFixed();

	RFIXED Sin(RAngle inDeg) const
	{
		return SinTable[inDeg];
	}
	RFIXED Cos(RAngle inDeg) const
	{
		return CosTable[inDeg];
	}
private:
	RFIXED SinTable[256];
	RFIXED CosTable[256];
};

extern int FastHypot(int a,int b);

#endif // !defined(AFX_RMATHANGLE_H__6883F47A_8B8D_40C1_A43D_CCE61B84B7E6__INCLUDED_)
