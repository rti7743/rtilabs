#ifndef	___LIM3D_DRAW__H	//重複防止
#define ___LIM3D_DRAW__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Lim3DMaterial.h>

class TLim3DPolygon;

class TLim3DPolygonDraw
{
public:
	void DoDraw(TLim3DPolygon* Poly , DrawUse* du,int Many);
protected:
	//ライティングなし,テクスチャなし.
	void NonLightNonTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//ライティングなし,テクスチャあり.
	void NonLightTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//ライティングあり,テクスチャなし.
	void LightNonTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
	//ライティングあり,テクスチャあり.
	void LightTexture
		(TLim3DPolygon* Poly , DrawUse* du,int Many, TLim3DMaterial* inMaterial);
private:
	//ライティングされた色を求めます.
	int ClasFillLightingColor
		(int inRGB , TLim3DPolygon* inPoly , XYZFixed* const inPos, TLim3DMaterial* inMaterial);
};

#include <./Lim3D/rti_Lim3DPolygon.h>

#endif				//重複防止