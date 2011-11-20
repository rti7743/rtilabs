// RGISSmallPenFile.h: RGISSmallPenFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPENFILE_H__0BF57974_C64A_4FDD_AF1C_232BFCB0A052__INCLUDED_)
#define AFX_RGISSMALLPENFILE_H__0BF57974_C64A_4FDD_AF1C_232BFCB0A052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallDraw.h"

enum
{
	PEN_TYPE_NONE		=	0,
	PEN_TYPE_1			=	1,
	PEN_TYPE_2			=	2,
	PEN_TYPE_3			=	3,
	PEN_TYPE_4			=	4,
	PEN_TYPE_5			=	5,
	PEN_TYPE_6			=	6,
	PEN_TYPE_TRACKLOG	=	7,
	PEN_TYPE_ERASE		=	0xff,
};

class RGISSmallPenFile
{
	struct RGISSmallPenFilePoint
	{
		LPOINT	xy;
		unsigned char size;
		unsigned char color;
	};
	typedef vector<RGISSmallPenFilePoint> RGISSmallPenFilePointVector;

	LPOINT LastXY;

	RGISSmallPenFilePointVector	Vec;
private:
	void Push(const LPOINT * inXY , unsigned char inSize , unsigned char inColor);
	void Erase(const LPOINT * inXY , unsigned char inSize);
public:
	RGISSmallPenFile();
	void Load(const string & inFilename) throw(RException);
	void Save(const string & inFilename) throw(RException);
	void Line(const LPOINT * inXY , unsigned char inSize , unsigned char inColor );
	void LastClear();

	void Draw(RGISSmallDraw * ioDraw) const;
};

#endif // !defined(AFX_RGISSMALLPENFILE_H__0BF57974_C64A_4FDD_AF1C_232BFCB0A052__INCLUDED_)
