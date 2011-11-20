#include <./Lim3D/rti_Lim3DMaster.h>


//ライティング
DWORD TLim3DLight::Lighting(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	//ライトの効果によって振り分け.
	switch( inMaterial->GetLighting() )
	{
	case NONE_LIGHTING:		//ライティングなし
		break;
	case DEFUSE_LIGHTING:	//ディフューズ
		return Diffuse( inPoly ,  inPos , inMaterial);
		break;
	case SPECULAR_LIGHTING:	//スペキュラー
		return Specular( inPoly ,  inPos , inMaterial);
		break;
	case AMBIENT_LIGHTING:	//アンビエント
		return Ambient( inPoly , inMaterial);
		break;
	case LAMBERT_LIGHTING:	//ランバー (ディフューズ + アンビエント)
		return Lambert( inPoly ,  inPos , inMaterial);
		break;
	case PHONG_LIGHTING:	//フォン   (ディフューズ + アンビエント + スペキュラー)
		return Phong( inPoly ,  inPos , inMaterial);
		break;
	}
	return 0;
}

//ディフューズ
DWORD TLim3DLight::Diffuse(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	/*
	*  ^     ^
	*   \ cos|
	*    \ Θ|
	*revL \  |N       revL は、左上に向かってます.
	*      \ |        N    は、上にむかってます.
	*       \|
	*------------------                      
	*       ポリゴンだよん
	*/
	XYZFixed		theRevLight;			//逆ライトベクトル
	//pos に対するこのライトの逆ライトベクトルを求める.
    GetRevLightVector(&theRevLight,inPos);
	Fixed theDiffuse;
	{
		register Fixed	theCosA;
		// revLight ･ HousenVector  で 明るさ(-1～1)が求まります.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
        if (theCosA < 0)
        {	//光はあたってません.
    		return 0;
        }
		theDiffuse = FixedMul( theCosA, inMaterial->GetDiffuseReflexFactor() );
	}
	if (theDiffuse <= 0)
	{	//すべて 0
   		return 0;
	}
	if (theDiffuse >= IF(1) ) 
	{	//そのまんまの色
		return m_RGB;
	}

	//このライトによる色を作る.
	return MulColor(m_RGB,theDiffuse);
}

//スペキュラー
DWORD TLim3DLight::Specular(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	/*
	* スペキュラーはちぃっとややこしいです.
	*       ^   ^N  ^       N は、法線ベクトル 上に向かってます.
	*       L\  |  \R	   	L は、逆光線ベクトル 左上に向かってます.
	*         \ | \         R は、反射光ベクトル 右上に向かってます.
	*          \|\          まず、  R を求めます.
	*  -------------------
	*    ポリゴンの面
	*Tag1
	*
	*
	*    ^cosa^
	*     \   |N          L･N により、 Cos a を求める.
	*     L\  |           
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag2
	*         ^
	*         |
	*    ^ ---|P		L * CosAにより、 N に L を投影します.
	*     \   |         すると、 N の大きさは、 CP となります.
	*     L\  |N        CP を 2 倍します.   
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag3
	*
	*
	*         ^	           2倍したCP  と、 L を足し算します.
	*        /|            すると、ベクトル G が求まります.
	*     G / |            つまり、二等辺三角形がかけるわけです.
	*      /  |
	*     /   |            そして、 なんと、 G は、 R と同じになるのです.
	*    ^ ---|P		    ベクトルは方向をあらわすものなので、位置は関係ありません.
	*     \   |         
	*     L\  |N        
	*       \ |
	*        \|
	* --------C----------------
	*
	*Tag4
 	*   ^    ^   
	*  N|  R/   /         R と E (視点ベクトル) の内積を求めます.
	*   |  /b /E          すると、
	*   | / /  Cos b      Cos b が求まります.
	*   |/<               
	*---------------------
	*
	*Tag5               
	*     その後、 CosB^n * ks となり、スペキュラーの強さが求まる.
	*     n は、物体の柔らかさです.  小さいとスペキュラーの範囲が広がり、
	*     大きければ、範囲は狭くなります.
	*
	*/
	XYZFixed  theRevLight;					//逆ライトベクトル
	XYZFixed  theR;							//ベクトルR
	XYZFixed  theRevE;						//逆視点ベクトル
	//カメラの位置.
	XYZFixed* theEyePos = (TLim3DMaster::GetSelectEye())->GetPoint()->GetPos();
	//inPos に対するこのライトの逆ライトベクトルを求める.
    GetRevLightVector(&theRevLight,inPos);
	//inPos に対するこの視点の逆視点ベクトルを求めます.
	VectorSub( &theRevE , theEyePos ,  inPos);
	Fixed theSpecular;
	{
		register Fixed	theCosA;
		// theRevLight ･ 法線  で 明るさ(-1～1)が求まります.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
		if (theCosA < 0)
		{	//すべて 0
	   		return 0;
		}
		//ベクトルCP
		XYZFixed theCP;	
		//theCosA * theRevLightにより CP を求めます.
		VectorMul( &theCP , &theRevLight , theCosA);
		//ベクトル CP を 2 倍します.
		theCP.x *= 2;		theCP.y *= 2;		theCP.z *= 2;
		//ベクトル theCP + theRevLight により、ベクトルR (ベクトルG) を求めます.
		VectorAdd( &theR , &theCP , &theRevLight);
		//theR , theRevE の正規化.
        Normalize(&theR);        Normalize(&theRevE);
		//ベクトル theR と、逆視点ベクトルにより、 theCosB (変数はCosA (^^;) を求める.
        theCosA = DotProduct(&theR,&theRevE);
		if (theCosA < 0)
		{	//すべて 0
	   		return 0;
		}
		//CosB ^ 物体の軟らかさ(0～) * 鏡面反射係数
		theSpecular = FixedMul(fxpow(theCosA,inMaterial->GetVelvetyReflexFactor() )
												, inMaterial->GetSpecularReflexFactor() );
	}
	if (theSpecular <= 0)
	{	//すべて 0
   		return 0;
	}
	if (theSpecular >= IF(1) ) 
	{	//そのまんまの色
   		return m_RGB;
	}
	//このライトによる色を作る.
	return MulColor(m_RGB,theSpecular);
}

//アンビエント
DWORD TLim3DLight::Ambient(POLYGON inPoly , TLim3DMaterial* inMaterial)
{
	//このライトによる色を作る.
	return MulColor(m_RGB, inMaterial->GetAmbientReflexFactor() );
}

//ランバー
DWORD TLim3DLight::Lambert(POLYGON inPoly,
					XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	XYZFixed		theRevLight;			//逆ライトベクトル
	//pos に対するこのライトの逆ライトベクトルを求める.
    GetRevLightVector(&theRevLight,inPos);
	Fixed theLambert;
	{
		register Fixed	theCosA;
		// revLight ･ HousenVector  で 明るさ(-1～1)が求まります.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );
        if (theCosA < 0)
        {	//光はあたってません.
    		return 0;
        }
		theLambert = FixedMul( theCosA, inMaterial->GetDiffuseReflexFactor() ) 
											+ inMaterial->GetAmbientReflexFactor();
	}
	if (theLambert <= 0)
	{	//すべて 0
   		return 0;
	}
	if (theLambert >= IF(1) ) 
	{	//そのまんまの色
   		return m_RGB;
	}

	//このライトによる色を作る.
	return MulColor(m_RGB,theLambert);
}

//フォン
DWORD TLim3DLight::Phong(POLYGON inPoly,
				XYZFixed const *inPos , TLim3DMaterial* inMaterial)
{
	XYZFixed  theRevLight;					//逆ライトベクトル
	XYZFixed  theR;							//ベクトルR
	XYZFixed  theRevE;						//逆視点ベクトル
	//カメラの位置.
	XYZFixed* theEyePos = (TLim3DMaster::GetSelectEye())->GetPoint()->GetPos();
	//inPos に対するこのライトの逆ライトベクトルを求める.
    GetRevLightVector(&theRevLight,inPos);
	//inPos に対するこの視点の逆視点ベクトルを求めます.
	VectorSub( &theRevE , theEyePos ,  inPos);
	Fixed thePhong = 0;
	while(1)
	{
		register Fixed	theCosA;
		// theRevLight ･ 法線  で 明るさ(-1～1)が求まります.
        theCosA = DotProduct(&theRevLight,inPoly->GetHousenVector() );

		if (theCosA < 0)	break;
		thePhong = FixedMul( theCosA , inMaterial->GetDiffuseReflexFactor() )
											+ inMaterial->GetAmbientReflexFactor();
		//ベクトルCP
		XYZFixed theCP;	
		//theCosA * theRevLightにより CP を求めます.
		VectorMul( &theCP , &theRevLight , theCosA);
		//ベクトル CP を 2 倍します.
		theCP.x *= 2;		theCP.y *= 2;		theCP.z *= 2;
		//ベクトル theCP + theRevLight により、ベクトルR (ベクトルG) を求めます.
		VectorAdd( &theR , &theCP , &theRevLight);
		//theR , theRevE の正規化.
        Normalize(&theR);        Normalize(&theRevE);
		//ベクトル theR と、逆視点ベクトルにより、 theCosB (変数はCosA (^^;) を求める.
        theCosA = DotProduct(&theR,&theRevE);
		if (theCosA < 0)	break;

		//CosB ^ 物体の軟らかさ(0～) * 鏡面反射係数
		thePhong = FixedMul(fxpow(theCosA,inMaterial->GetVelvetyReflexFactor() )
												, inMaterial->GetSpecularReflexFactor() );
		break;
	}
	if (thePhong <= 0)
	{	//すべて 0
		return 0;
	}
	if (thePhong >= IF(1) ) 
	{	//そのまんまの色
		return m_RGB;
	}
	//このライトによる色を作る.
	return MulColor(m_RGB,thePhong);
}


//ライトの回転角度を指定.
void			TLim3DLight::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point.SetAngleX(x);
	m_Point.SetAngleY(y);
	m_Point.SetAngleZ(z);
}
//ライトの場所を指定.
void			TLim3DLight::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(x);
	m_Point.SetPosY(y);
	m_Point.SetPosZ(z);
}
//ライトの回転角度を取得.
void			TLim3DLight::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point.GetAngleX();
	*y = m_Point.GetAngleY();
	*z = m_Point.GetAngleZ();
}
//ライトの場所を取得.
void			TLim3DLight::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point.GetPosX();
	*y = m_Point.GetPosY();
	*z = m_Point.GetPosZ();
}
//ライトの回転角度を増やす.
void			TLim3DLight::AddRotate(int x,int y,int z)
{
	m_Point.SetAngleX(m_Point.GetAngleX() + x);
	m_Point.SetAngleY(m_Point.GetAngleY() + y);
	m_Point.SetAngleZ(m_Point.GetAngleZ() + z);
}
//ライトの場所を移動する.
void			TLim3DLight::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(m_Point.GetPosX() + x);
	m_Point.SetPosY(m_Point.GetPosY() + y);
	m_Point.SetPosZ(m_Point.GetPosZ() + z);
}

//場所情報クラスへのベタアクセス
TLim3DPoint* TLim3DLight::GetPoint()
{
	return &m_Point;
}

//逆光線ベクトルを求める.
void TLim3DLight::GetRevLightVector(XYZFixed* outRevLightVector , XYZFixed const *inPos)
{
	//ライトの位置より現在値を引いて、逆視点ベクトルを求めます.
	VectorSub(outRevLightVector, GetPoint()->GetPos() , inPos);
	//それを正規化します.
	Normalize(outRevLightVector);
}

//このライトによる色を作る.
DWORD TLim3DLight::MulColor(DWORD inOrignalRGB,Fixed inParsent)
{
	int theR,theG,theB;

	theR = (inOrignalRGB & 0x00ff0000) >> 16;
	theG = (inOrignalRGB & 0x0000ff00) >>  8;
	theB = (inOrignalRGB & 0x000000ff)      ;

	theR = FI(theR*inParsent);
	theG = FI(theG*inParsent);
	theB = FI(theB*inParsent);

	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	return (theR << 16) + (theG << 8) + (theB);
}


