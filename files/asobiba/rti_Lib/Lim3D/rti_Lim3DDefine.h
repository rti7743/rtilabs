#ifndef	___LIM3D_DEFINE__H	//重複防止
#define ___LIM3D_DEFINE__H

#include <./rti_define.h>

//頂点データ
struct VecWorldEye
{
	XYZFixed	Vec;	//理論的な位置
	XYZFixed	World;	//ワールドでの位置
	XYZFixed	Eye;	//視点での位置
};

//ポリゴンがもっているデータ
struct PolygonData
{
	int		Link;	//頂点の場所
	XY		Texture;//テクスチャ
};

//ポリゴンがもっているデータ
struct PolygonDataCoexist
{
	void*	Model;	//この頂点が属しているモデル
	int		Link;	//頂点の場所
	XY		Texture;//テクスチャ
};

//ポリゴンがもっているデータ
struct PolygonDataEx
{
	VecWorldEye*	LinkVecPointer;	//頂点の場所のポインタ.
	XY				Texture;//テクスチャ
};

//実際に絵画するときに使うデータ
struct DrawUse
{
	XYZFixed	Pos;	//視点座標の最終形態.
	XYZFixed	Display;//絵画する位置.
	XY			Texture;//テクスチャ座標.
};

#endif			//重複防止

