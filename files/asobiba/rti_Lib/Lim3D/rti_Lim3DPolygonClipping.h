#ifndef	___LIM3D_CLIPPING__H	//重複防止
#define ___LIM3D_CLIPPING__H

#include <./Lim3D/rti_Lim3DDefine.h>

//クリップ平面の位置.
#define Z_CLIPPING	15

class TLim3DPolygonClipping
{
public:
	static int DoClipping(DrawUse * du , const PolygonDataEx* pdp , int Many);
protected:
	//四角形において 1点クリップされる.	>>五角形になる.
	static bool ClippingShiftIntoPentagon
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//四角形において 2点クリップされる.	>>四角形になる.
	static bool ClippingShiftIntoSquare
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//四角形において 3点クリップされる.	>>三角形になる.
	static bool ClippingShiftIntoTriangle
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//三角形において 1点クリップされる.	>>四角形になる.
	static bool ClippingTriangleShiftIntoSquare
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//三角形において 2点クリップされる.	>>三角形になる.
	static bool ClippingTriangleShiftIntoTriangle
		(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo);
	//pdp1 と pdp2 の間にある クリップ平面 (Z_CLIPPING) による分割.
	static void LineCut( DrawUse * du ,
					const PolygonDataEx* pdp1 , const PolygonDataEx* pdp2 );
	//pdp を du にふつーにコピーする.
	static void NormalCopy( DrawUse * du ,	const PolygonDataEx* pdp );
private:
};

#endif				//重複防止