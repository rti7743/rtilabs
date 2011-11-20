#ifndef	___LIM3D_POLYGON__H	//重複防止
#define ___LIM3D_POLYGON__H

#include <windows.h>
#include <./Lim3D/rti_Lim3DLinkData.h>
#include <./Lim3D/rti_Lim3DPolygonClipping.h>
#include <./Lim3D/rti_Lim3DPolygonDraw.h>
#include <./Lim3D/rti_SuperListClient.h>


//シェーディング効果
enum ShadingLevel
{
	NONE_SHADING ,	//シェーディングなし.
	FLAT_SHADING ,	//フラットシェーディング
	GROW_SHADING ,	//グローシェーディング
};

class TLim3DModel;
class TLim3DLinkData;

class TLim3DPolygon : public __TSuperListClient
{
public:
	//コンストラクタ
	TLim3DPolygon()
	{
		m_Shading		= NONE_SHADING;						//シェーディング
		m_Material		= NULL;
	}
	/*****************/
	//ユーザーのための高レベルメソッド.
	/*****************/
	//絵画
	void		Draw();
	//つながり情報セット.
	bool		SetLink(PolygonData *_c , int Many , bool SizeNotChange = true);
	//つながり情報セット.
	bool		AddLink(PolygonData *_c , int Many , bool SizeNotChange = true);
	//つながり情報セット.
	bool		SetLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange = true);
	//つながり情報セット.
	bool		AddLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange = true);
	//つながりクリア
	void		ClearLink();
	//テクスチャーの再マッピング(AUTO).
	bool		ReMapTexture();
	//テクスチャーの再マッピング.
	bool		ReMapTexture(int X1 ,int Y1,
						int X2,int Y2, int X3,int Y3, int X4,int Y4);
	//法線ベクトルの取得.
	XYZFixed*	GetHousenVector(){return &m_HousenVector;}
	//重心の取得.
	XYZFixed*	GetAvg(){return &m_AvgPoint;}
	//視点座標での重心の Z 
	Fixed	GetEyeAvgZ(){return m_Eye_Avg_Z;}
	/*****************/
	//ユーザーのための低レベルメソッド.
	/*****************/
	//つながりデータをベタで取得.
	TLim3DLinkData*	GetLinkData();
	//親のモデル取得.
	TLim3DModel* GetParenceModel();


	//実際絵画部分
	void TrueDraw();

	/*****************/
	//シェーディング関係
	/*****************/
	//シェーディング
	void			SetShading(ShadingLevel c){ m_Shading = c;}
	ShadingLevel	GetShading(){ return m_Shading ;}
	//マテリアル関係.
	void			SetMaterial(TLim3DMaterial* c){ m_Material = c;}
	TLim3DMaterial*	GetMaterial(){ return m_Material ;}


protected:
	ShadingLevel		m_Shading;					//シェーディング
	TLim3DMaterial*		m_Material;					//マテリアル.

	//親モデル.
	//TLim3DModel*	m_ParenceModel;
	//つながりデータ.
	TLim3DLinkData	m_LinkData;
	//クリッピング
	TLim3DPolygonClipping m_PolygonClipping;
	//絵画
	TLim3DPolygonDraw	m_PolygonDraw;

	//法線ベクトルの算出.
	void ClasHousenVector(PolygonDataEx* inUsing);
	//裏面かどうか判断. true = 裏面
	bool IsBackstairs(PolygonDataEx* inUsing);
	//つながり情報の登録.
	bool LinkEntry(PolygonData *_c , int Many , bool SizeNotChange , bool IsSet);
	//つながり情報の登録.
	bool LinkEntry(PolygonDataCoexist *_c , int Many , bool SizeNotChange , bool IsSet);

	XYZFixed m_AvgPoint;						//このポリゴンの重心.(world)
	Fixed m_Eye_Avg_Z;							//このポリゴンの視点座標での重心のZ.
	XYZFixed m_HousenVector;					//このポリゴンの法線ベクトル(world/単位)

private:
};

//モデル定義
typedef TLim3DPolygon* POLYGON;


#include <./Lim3D/rti_Lim3DPolygonCaretaker.h>


#endif				//重複防止