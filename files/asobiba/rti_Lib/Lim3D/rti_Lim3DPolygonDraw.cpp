
#include <./Lim3D/rti_Lim3DMaster.h>
#include <./Lim3D/rti_Lim3DPolygonDraw.h>
#include <./Graphics/rti_Graphics.h>
#include <./Graphics/rti_Graphics_Triangle.h>

//ポリゴン絵画
void TLim3DPolygonDraw::DoDraw(TLim3DPolygon* Poly , DrawUse* du,int Many)
{
	TLim3DMaterial*		theMaterial	;		//マテリアル取得.
	theMaterial = Poly->GetMaterial() ;

	switch( Poly->GetShading() )
	{
	case NONE_SHADING:					//ライトは無視
		switch ( theMaterial->GetDrawing() )
		{
		case BETA_DRAW:	
			NonLightNonTexture(Poly , du , Many , theMaterial);
			break;
		case TEXTURE_DRAW:
			NonLightTexture(Poly , du , Many , theMaterial);
			break;
		}
		break;
	case FLAT_SHADING:					//ライト有効 ふらっと.
		switch ( theMaterial->GetDrawing() )
		{
		case BETA_DRAW:
			LightNonTexture(Poly , du , Many , theMaterial);
			break;
		case TEXTURE_DRAW:
			LightTexture(Poly , du , Many , theMaterial);
			break;
		}
		break;
	}

}

//ライティングなし,テクスチャなし.
void TLim3DPolygonDraw::NonLightNonTexture
				(TLim3DPolygon* Poly , DrawUse* du,int Many , TLim3DMaterial* inMaterial)
{
	/*
	int FillColor = inMaterial->GetColor() ;
	TZBuffer * ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.
	BMD		   OutBuffer = TLim3DMaster::GetOutBuffer();	//出力先.

	XYZ Pos[5];
	Pos[0].x = FI(du[0].Display.x);	Pos[0].y = FI(du[0].Display.y);	Pos[0].z = FI(du[0].Display.z);
	Pos[1].x = FI(du[1].Display.x);	Pos[1].y = FI(du[1].Display.y);	Pos[1].z = FI(du[1].Display.z);
	Pos[2].x = FI(du[2].Display.x);	Pos[2].y = FI(du[2].Display.y);	Pos[2].z = FI(du[2].Display.z);
	Pos[3].x = FI(du[3].Display.x);	Pos[3].y = FI(du[3].Display.y);	Pos[3].z = FI(du[3].Display.z);
	Pos[4].x = FI(du[4].Display.x);	Pos[4].y = FI(du[4].Display.y);	Pos[4].z = FI(du[4].Display.z);
    switch(Many)
    {
    case 5:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[2], Pos[3], Pos[0],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[3], Pos[4], Pos[0],
                     FillColor,  ZBuffer		);
        break;
    case 4:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[2], Pos[3], Pos[0],
                     FillColor,  ZBuffer		);
        break;
    case 3:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer	);
        break;
    }
//	DEBUG3("Drawing %d" , Many);
	*/
	_BetaData BetaData;
	BetaData.OutBmd = TLim3DMaster::GetOutBuffer();	//出力先.
	BetaData.rgb = inMaterial->GetColor() ;
	BetaData.ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.

    switch(Many)
    {
    case 5:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[2].Display , (_XYZP*)&du[3].Display , (_XYZP*)&du[0].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[3].Display , (_XYZP*)&du[4].Display , (_XYZP*)&du[0].Display , &BetaData);
        break;
    case 4:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[2].Display , (_XYZP*)&du[3].Display , (_XYZP*)&du[0].Display , &BetaData);
        break;
    case 3:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
        break;
    }
}

//ライティングなし,テクスチャあり.
void TLim3DPolygonDraw::NonLightTexture
				(TLim3DPolygon* Poly , DrawUse* du,int Many , TLim3DMaterial* inMaterial)
{
	BMD Texture = inMaterial->GetTexture() ;
	TZBuffer * ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.
	BMD		   OutBuffer = TLim3DMaster::GetOutBuffer();	//出力先.

	XYZ Pos[5];
	Pos[0].x = FI(du[0].Display.x);	Pos[0].y = FI(du[0].Display.y);	Pos[0].z = FI(du[0].Display.z);
	Pos[1].x = FI(du[1].Display.x);	Pos[1].y = FI(du[1].Display.y);	Pos[1].z = FI(du[1].Display.z);
	Pos[2].x = FI(du[2].Display.x);	Pos[2].y = FI(du[2].Display.y);	Pos[2].z = FI(du[2].Display.z);
	Pos[3].x = FI(du[3].Display.x);	Pos[3].y = FI(du[3].Display.y);	Pos[3].z = FI(du[3].Display.z);
	Pos[4].x = FI(du[4].Display.x);	Pos[4].y = FI(du[4].Display.y);	Pos[4].z = FI(du[4].Display.z);
    switch(Many)
    {
    case 5:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture , 
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[2].Texture ,du[3].Texture ,du[4].Texture ,
			OutBuffer, Pos[2], Pos[3], Pos[0],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[3].Texture ,du[4].Texture ,du[0].Texture ,
			OutBuffer, Pos[3], Pos[4], Pos[0],  ZBuffer		);
        break;
    case 4:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture , 
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[2].Texture ,du[3].Texture ,du[0].Texture ,
			OutBuffer, Pos[2], Pos[3], Pos[0],  ZBuffer		);
        break;
    case 3:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture ,
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        break;
    }
}





//ライティングあり,テクスチャなし.
void TLim3DPolygonDraw::LightNonTexture
				(TLim3DPolygon* Poly , DrawUse* du,int Many , TLim3DMaterial* inMaterial)
{
/*
	int FillColor ;
	//ライティングされた色を求めます.
	FillColor = ClasFillLightingColor
		( inMaterial->GetColor() ,Poly , Poly->GetAvg() , inMaterial);
	TZBuffer * ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.
	BMD		   OutBuffer = TLim3DMaster::GetOutBuffer();	//出力先.

	XYZ Pos[5];
	Pos[0].x = FI(du[0].Display.x);	Pos[0].y = FI(du[0].Display.y);	Pos[0].z = FI(du[0].Display.z);
	Pos[1].x = FI(du[1].Display.x);	Pos[1].y = FI(du[1].Display.y);	Pos[1].z = FI(du[1].Display.z);
	Pos[2].x = FI(du[2].Display.x);	Pos[2].y = FI(du[2].Display.y);	Pos[2].z = FI(du[2].Display.z);
	Pos[3].x = FI(du[3].Display.x);	Pos[3].y = FI(du[3].Display.y);	Pos[3].z = FI(du[3].Display.z);
	Pos[4].x = FI(du[4].Display.x);	Pos[4].y = FI(du[4].Display.y);	Pos[4].z = FI(du[4].Display.z);
    switch(Many)
    {
    case 5:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[2], Pos[3], Pos[0],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[3], Pos[4], Pos[0],
                     FillColor,  ZBuffer		);
        break;
    case 4:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer		);
        ReSizeBoxZ3(OutBuffer,
					 Pos[2], Pos[3], Pos[0],
                     FillColor,  ZBuffer		);
        break;
    case 3:
        ReSizeBoxZ3(OutBuffer,
					 Pos[0], Pos[1], Pos[2],
                     FillColor,  ZBuffer	);
        break;
    }
*/
	_BetaData BetaData;
	BetaData.OutBmd = TLim3DMaster::GetOutBuffer();	//出力先.
	BetaData.rgb = ClasFillLightingColor
			( inMaterial->GetColor() ,Poly , Poly->GetAvg() , inMaterial);
	BetaData.ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.

    switch(Many)
    {
    case 5:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[2].Display , (_XYZP*)&du[3].Display , (_XYZP*)&du[0].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[3].Display , (_XYZP*)&du[4].Display , (_XYZP*)&du[0].Display , &BetaData);
        break;
    case 4:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
		TriangleDraw((_XYZP*)&du[2].Display , (_XYZP*)&du[3].Display , (_XYZP*)&du[0].Display , &BetaData);
        break;
    case 3:
		TriangleDraw((_XYZP*)&du[0].Display , (_XYZP*)&du[1].Display , (_XYZP*)&du[2].Display , &BetaData);
        break;
    }
}

//ライティングあり,テクスチャあり.
void TLim3DPolygonDraw::LightTexture
				(TLim3DPolygon* Poly , DrawUse* du,int Many , TLim3DMaterial* inMaterial)
{
	BMD Texture = inMaterial->GetTexture() ;
	TZBuffer * ZBuffer = TLim3DMaster::GetZBuffer();	//ZBUffer.
	BMD		   OutBuffer = TLim3DMaster::GetOutBuffer();	//出力先.

	XYZ Pos[5];
	Pos[0].x = FI(du[0].Display.x);	Pos[0].y = FI(du[0].Display.y);	Pos[0].z = FI(du[0].Display.z);
	Pos[1].x = FI(du[1].Display.x);	Pos[1].y = FI(du[1].Display.y);	Pos[1].z = FI(du[1].Display.z);
	Pos[2].x = FI(du[2].Display.x);	Pos[2].y = FI(du[2].Display.y);	Pos[2].z = FI(du[2].Display.z);
	Pos[3].x = FI(du[3].Display.x);	Pos[3].y = FI(du[3].Display.y);	Pos[3].z = FI(du[3].Display.z);
	Pos[4].x = FI(du[4].Display.x);	Pos[4].y = FI(du[4].Display.y);	Pos[4].z = FI(du[4].Display.z);
    switch(Many)
    {
    case 5:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture , 
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[2].Texture ,du[3].Texture ,du[4].Texture ,
			OutBuffer, Pos[2], Pos[3], Pos[0],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[3].Texture ,du[4].Texture ,du[0].Texture ,
			OutBuffer, Pos[3], Pos[4], Pos[0],  ZBuffer		);
        break;
    case 4:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture , 
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        ReSizeDrawZ3(Texture , du[2].Texture ,du[3].Texture ,du[0].Texture ,
			OutBuffer, Pos[2], Pos[3], Pos[0],  ZBuffer		);
        break;
    case 3:
        ReSizeDrawZ3(Texture , du[0].Texture ,du[1].Texture ,du[2].Texture ,
			OutBuffer, Pos[0], Pos[1], Pos[2],  ZBuffer		);
        break;
    }
}

//ライティングされた色を求めます.
int TLim3DPolygonDraw::ClasFillLightingColor
	(int inRGB , TLim3DPolygon* inPoly , XYZFixed* const inPos , TLim3DMaterial* inMaterial)
{
	return (TLim3DMaster::GetLightCaretaker())
						->GetColor(inRGB , inPoly , inPos , inMaterial);
}

