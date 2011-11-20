#ifndef	___LIM3D_MATERIAL__H	//重複防止
#define ___LIM3D_MATERIAL__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_SuperListClient.h>

//物体のライトの効果.
enum LightLevel
{
	NONE_LIGHTING,		//ライティングなし
	DEFUSE_LIGHTING,	//ディフューズ
	SPECULAR_LIGHTING,	//スペキュラー
	AMBIENT_LIGHTING,	//アンビエント
	LAMBERT_LIGHTING,	//ランバー (ディフューズ + アンビエント)
	PHONG_LIGHTING,		//フォン   (ディフューズ + アンビエント + スペキュラー)
};

//塗りつぶし方法.
enum DrawingLevel
{
	BETA_DRAW		 ,	//べた塗り
	TEXTURE_DRAW	 ,	//テクスチャー.
};


class TLim3DMaterial : public __TSuperListClient
{
public:
	TLim3DMaterial()
	{
		m_Drawing		= BETA_DRAW;						//塗りかた.
		m_Lighting		= NONE_LIGHTING;					//ライト効果.
	}

	/*****************/
	//シェーディング関係
	/*****************/
	//塗りつぶす方法.
	void			SetDrawing(DrawingLevel c){ m_Drawing = c;}
	DrawingLevel	GetDrawing(){ return m_Drawing ;}
	//色
	void			SetColor(DWORD c){ m_Color = c;}
	DWORD			GetColor(){ return m_Color ;}
	//テクスチャー
	void			SetTexture(BMD c){ m_Texter = c;}
	BMD				GetTexture(){ return m_Texter ;}

	/*****************/
	//ライト関係
	/*****************/
	//ライト効果
	void			SetLighting(LightLevel l){ m_Lighting = l;};
	LightLevel		GetLighting(){return m_Lighting;};
	//拡散反射係数 (0～1)
	void			SetDiffuseReflexFactor(Fixed n){ m_DiffuseReflexFactor = n;};	
	Fixed			GetDiffuseReflexFactor(){return m_DiffuseReflexFactor;};
	//鏡面反射係数 (0～1)
	void			SetSpecularReflexFactor(Fixed n){ m_SpecularReflexFactor = n;};
	Fixed			GetSpecularReflexFactor(){return m_SpecularReflexFactor;};
	//鏡面反射の強度係数(やわらかさ 0 ～)
	void			SetVelvetyReflexFactor(Fixed n){ m_VelvetyReflexFactor = n;};	
	Fixed			GetVelvetyReflexFactor(){return m_VelvetyReflexFactor;};
	//環境反射係数 (0～1)
	void			SetAmbientReflexFactor(Fixed n){ m_AmbientReflexFactor = n;};	
	Fixed			GetAmbientReflexFactor(){return m_AmbientReflexFactor;};


protected:
	DrawingLevel m_Drawing;						//塗りかた.
	DWORD	m_Color;						//ポリゴンの色.
    BMD m_Texter;						//テクスチャーマッピング

	LightLevel m_Lighting;						//ライト効果.
	Fixed m_DiffuseReflexFactor;			    //拡散反射係数 (0～1)
	Fixed m_SpecularReflexFactor;	            //鏡面反射係数 (0～1)
	Fixed m_VelvetyReflexFactor;			    //鏡面反射の強度係数(やわらかさ 0 ～)
	Fixed m_AmbientReflexFactor;	            //環境反射係数 (0～1)
private:
};

typedef class TLim3DMaterial*	MATERIAL;

#endif				//重複防止