// RGISSmallDrawEffect.cpp: RGISSmallDrawEffect クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallDrawEffect.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallDrawEffect::RGISSmallDrawEffect()
{
	MathAngle = RMathAngleFixed::getMathAngleFixed();
}

RGISSmallDrawEffect::~RGISSmallDrawEffect()
{

}

void RGISSmallDrawEffect::InitDisplay(const LPOINT * inGlobalJiku ,int inScreenWidth,int inScreenHeight, unsigned int inScale,RAngle inDeg)
{
	ASSERT(inScale >= 0);

	this->Scale = inScale;
	this->Deg = inDeg;
	this->RevDeg = RevAngle(Deg);

	TRACE("Deg:%d\n",(int)Deg);

	Jiku = *inGlobalJiku;

	ScreenWidth = (inScreenWidth  / 2);
	ScreenHeight = (inScreenHeight / 2);

	//画面に描画されるグローバル座標の RECT を求めます.
//	int width  = (ScreenWidth  << Scale );
//	int height = (ScreenHeight << Scale );
	int width  = (ScreenWidth  << Scale );
	int height = (ScreenHeight << Scale );
	//四隅をすべて回転させて、その四隅の頂点をすべて内包する長方形を作ります.
	POINT lt , lb , rt , rb;
	lt.x	= (FtoI((-width) *MathAngle->Cos(Deg) - (-height) *MathAngle->Sin(Deg) ) + Jiku.x);
	lt.y	= (FtoI((-width) *MathAngle->Sin(Deg) + (-height) *MathAngle->Cos(Deg) ) + Jiku.y);
	lb.x	= (FtoI((-width) *MathAngle->Cos(Deg) - ( height) *MathAngle->Sin(Deg) ) + Jiku.x);
	lb.y	= (FtoI((-width) *MathAngle->Sin(Deg) + ( height) *MathAngle->Cos(Deg) ) + Jiku.y);
	rt.x	= (FtoI(( width) *MathAngle->Cos(Deg) - (-height) *MathAngle->Sin(Deg) ) + Jiku.x);
	rt.y	= (FtoI(( width) *MathAngle->Sin(Deg) + (-height) *MathAngle->Cos(Deg) ) + Jiku.y);
	rb.x	= (FtoI(( width) *MathAngle->Cos(Deg) - ( height) *MathAngle->Sin(Deg) ) + Jiku.x);
	rb.y	= (FtoI(( width) *MathAngle->Sin(Deg) + ( height) *MathAngle->Cos(Deg) ) + Jiku.y);
	GlobalDrawDisplay.left = lt.x;
	if (GlobalDrawDisplay.left > lb.x) GlobalDrawDisplay.left = lb.x;
	if (GlobalDrawDisplay.left > rt.x) GlobalDrawDisplay.left = rt.x;
	if (GlobalDrawDisplay.left > rb.x) GlobalDrawDisplay.left = rb.x;
	GlobalDrawDisplay.right = lt.x;
	if (GlobalDrawDisplay.right < lb.x) GlobalDrawDisplay.right = lb.x;
	if (GlobalDrawDisplay.right < rt.x) GlobalDrawDisplay.right = rt.x;
	if (GlobalDrawDisplay.right < rb.x) GlobalDrawDisplay.right = rb.x;
	GlobalDrawDisplay.top = lt.y;
	if (GlobalDrawDisplay.top > lb.y) GlobalDrawDisplay.top = lb.y;
	if (GlobalDrawDisplay.top > rt.y) GlobalDrawDisplay.top = rt.y;
	if (GlobalDrawDisplay.top > rb.y) GlobalDrawDisplay.top = rb.y;
	GlobalDrawDisplay.bottom = lt.y;
	if (GlobalDrawDisplay.bottom < lb.y) GlobalDrawDisplay.bottom = lb.y;
	if (GlobalDrawDisplay.bottom < rt.y) GlobalDrawDisplay.bottom = rt.y;
	if (GlobalDrawDisplay.bottom < rb.y) GlobalDrawDisplay.bottom = rb.y;

	TRACE("GlobalDrawDisplay %d,%d - %d,%d\n" , GlobalDrawDisplay.left - Jiku.x,GlobalDrawDisplay.top - Jiku.y,
		GlobalDrawDisplay.right - Jiku.x,GlobalDrawDisplay.bottom - Jiku.y);
}

//表示する点の座標変換
POINT RGISSmallDrawEffect::Attach(const LPOINT * inXY) const
{
	POINT xy ;
	xy.x = inXY->x - Jiku.x;
	xy.y = inXY->y - Jiku.y;
	xy.x = (xy.x >> Scale);
	xy.y = (xy.y >> Scale);
	POINT xy2 ;
	xy2.x = (FtoI(xy.x*MathAngle->Cos(Deg) - xy.y*MathAngle->Sin(Deg )) + ScreenWidth) ;
	xy2.y = (FtoI(xy.x*MathAngle->Sin(Deg) + xy.y*MathAngle->Cos(Deg) ) + ScreenHeight) ;

	return xy2;
}

//表示する点の座標変換
LPOINT RGISSmallDrawEffect::RevAttach(const POINT * inXY) const
{
	POINT xy ;
	xy.x = inXY->x - ScreenWidth;
	xy.y = inXY->y - ScreenHeight;
	POINT xy2 ;
	xy2.x = (FtoI(xy.x*MathAngle->Cos(RevDeg) - xy.y*MathAngle->Sin(RevDeg )) ) ;
	xy2.y = (FtoI(xy.x*MathAngle->Sin(RevDeg) + xy.y*MathAngle->Cos(RevDeg )) ) ;

	LPOINT xy3 ;
	xy3.x = (xy2.x << Scale);
	xy3.y = (xy2.y << Scale);

	xy3.x = xy3.x + Jiku.x ;
	xy3.y = xy3.y + Jiku.y ;

	return xy3;
}



//グローバル座標を表示する点の座標に変換
RECT RGISSmallDrawEffect::ConvertShowDisplay(const LRECT * inRect) const
{
	RECT rc ;
	rc.left		= inRect->left	- Jiku.x;
	rc.top		= inRect->top	- Jiku.y;
	rc.right	= inRect->right	- Jiku.x;
	rc.bottom	= inRect->bottom- Jiku.y;

	rc.left		= (rc.left >> Scale);
	rc.top		= (rc.top >> Scale);
	rc.right	= (rc.right >> Scale);
	rc.bottom	= (rc.bottom >> Scale);

	RECT rc2 ;
	rc2.left	=	(FtoI(rc.left	*MathAngle->Cos(Deg) - rc.top		*MathAngle->Sin(Deg )) + ScreenWidth) ;
	rc2.top		=	(FtoI(rc.left	*MathAngle->Sin(Deg) + rc.top		*MathAngle->Cos(Deg) ) + ScreenHeight) ;
	rc2.right	=	(FtoI(rc.right	*MathAngle->Cos(Deg) - rc.bottom	*MathAngle->Sin(Deg )) + ScreenWidth) ;
	rc2.bottom	=	(FtoI(rc.right	*MathAngle->Sin(Deg) + rc.bottom	*MathAngle->Cos(Deg) ) + ScreenHeight) ;

	return rc2;
}

//メッシュ間隔
const int MESH_INTERVAL	=	256;
//一ラインにいくつメッシュがあるか?
const int MESH_LINE		=	(65536/MESH_INTERVAL);

#include "RDrawObject.h"	//暫定


void RGISSmallDrawEffect::Boxes(int inScreenWidth,int inScreenHeight,unsigned short * image , unsigned char * inLevel    ,const LPOINT * inBlockStart,const COLORREF16 * inPallet) const
{

	const RFIXED rcos = MathAngle->Cos(RevDeg);
	const RFIXED rsin = MathAngle->Sin(RevDeg);


	const unsigned short mizuColor = BBananaConfig::getInstance()->getColor(COLOR_MIZU);

	RFIXED yx =  ( - ScreenWidth) *rcos -  ( - ScreenHeight) *rsin ;
	RFIXED yy =  ( - ScreenWidth) *rsin +  ( - ScreenHeight) *rcos ;
	for( int sy = 0 ; sy < inScreenHeight ; sy ++ )
	{
		RFIXED xx = yx;
		RFIXED xy = yy;
		for( int sx = 0 ; sx < inScreenWidth ; sx ++ )
		{
			if (*image == mizuColor )
			{
				unsigned long x = ((unsigned long) (FtoI(xx)) << Scale) + Jiku.x;
				unsigned long y = ((unsigned long) (FtoI(xy)) << Scale) + Jiku.y;

				if (x >= inBlockStart->x && y >= inBlockStart->y &&
					x <= inBlockStart->x+65536 && y <= inBlockStart->y+65536 )
				{
					int count = ((x - inBlockStart->x) / MESH_INTERVAL) + ((y - inBlockStart->y) / MESH_INTERVAL * MESH_LINE);
//					ASSERT(count >= 0 && count <= 65536);

					*image = inPallet[inLevel[count]];
				}

			}

			image ++;

			xx += rcos;
			xy += rsin;
		}
		yx -= rsin;
		yy += rcos;
	}


}



//拡大したときにはしょるレベルの決定
ParticularLevel RGISSmallDrawEffect::getLevel() const
{
	if (Scale < 4)			return LEVEL_FULL;
	else if (Scale < 5)		return LEVEL_NOT_BANCHI;
	else if (Scale < 6)		return LEVEL_NOT_CHIMEI;
	else if (Scale < 7)		return LEVEL_NOT_KOUKYOU;
	else if (Scale < 8)		return LEVEL_NOT_LOWLEVEL;
	else return LEVEL_NOT_MIDDLELEVEL;
}

