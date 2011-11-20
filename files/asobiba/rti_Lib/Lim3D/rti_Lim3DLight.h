#ifndef	___LIM3D_LIGHT__H	//重複防止
#define ___LIM3D_LIGHT__H

#include <./Lim3D/rti_Lim3DPoint.h>
#include <./Lim3D/rti_Matrix.h>
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./Lim3D/rti_Lim3DMaterial.h>
#include <./Lim3D/rti_SuperListClient.h>

//ライト基本型
class TLim3DLight : public __TSuperListClient
{
public:
	//======= メソッド ===========
	//ライティング
	DWORD Lighting(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//光源のあかり(神岸にあらず)を設定
	void SetLightColor(DWORD c) { m_RGB = c ;} 
	//ディフューズ
	DWORD Diffuse(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//スペキュラー
	DWORD Specular(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//アンビエント
	DWORD Ambient(POLYGON inPoly , TLim3DMaterial* inMaterial);
	//ランバー
	DWORD Lambert(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//フォン
	DWORD Phong(POLYGON inPoly,XYZFixed const *inPos , TLim3DMaterial* inMaterial);
	//視点の回転角度を指定.
	void			SetRotate(Angle x,Angle y,Angle z);
	//視点の場所を指定.
	void			SetPos(Fixed x,Fixed y,Fixed z);
	//視点の回転角度を取得.
	void			GetRotate(Angle *x,Angle *y,Angle *z);
	//視点の場所を取得.
	void			GetPos(Fixed *x,Fixed *y,Fixed *z);
	//視点の回転角度を増やす.
	void			AddRotate(int x,int y,int z);
	//視点の場所を移動する.
	void			AddPos(Fixed x,Fixed y,Fixed z);
	//場所情報クラスへのベタアクセス
	TLim3DPoint*	GetPoint();
protected:
	//このライトによる色を作る.
	DWORD MulColor(DWORD inOrignalRGB,Fixed inParsent);

	TLim3DPoint				m_Point;			//位置
	DWORD					m_RGB;				//色.
	
	//逆光線ベクトルを求める.
	void GetRevLightVector(XYZFixed* outRevLightVector , XYZFixed const *inPos);
private:
};

//モデル定義
typedef TLim3DLight* LIGHT;


#endif				//重複防止
