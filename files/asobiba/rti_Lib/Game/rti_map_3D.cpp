#include "StartWithExeFile.h"
#ifdef THE3D_MAP

#include <.\Graphics\rti_bitmaplib.h>
#include <.\Game\rti_map_3d.h>
#include <.\Base\rti_key.h>
#include <.\Base\rti_debug.h>

/***************************************************************/
//コンストラクタ
/***************************************************************/
TMap3D::TMap3D()
{
    X = 1000;
    Z = 750;
    Y = 50;
    Angle = 0;
	YMax = 0;
	//一時的にマップ(2D)を絵画をするところ.
	BmdDummyMap = NULL;
}

/***************************************************************/
//デストラクタ
/***************************************************************/
TMap3D::~TMap3D()
{
	//一時的にマップ(2D)を絵画をするところの削除
	if (BmdDummyMap != NULL)	BmpChar->Breaker(BmdDummyMap);
}

/***************************************************************/
//スクリーンサイズ定義
/***************************************************************/
void TMap3D::SetScreen(int x,int y)
{
	//先客削除
	if (BmdDummyMap != NULL)	BmpChar->Breaker(BmdDummyMap);
	//一時的にマップ(2D)を絵画をするところ.
    BmdDummyMap = BmpChar->New(x,y);
	//スクリーンサイズ定義
	ScreenX = x; ScreenY = y;
	//最大高度設定.	x = 512 で Ymax = 100    	512 : 100 = x: A を解く
	YMax = 100 * x / 512 ;
}

/***************************************************************/
//各種テクスチャの準備.
/***************************************************************/
void TMap3D::UsingTexture(BMD _BmdSky,BMD _BmdSkyGradation,BMD _AddGradation)
{
	BmdSky			= _BmdSky;			//空のテクスチャ
	BmdSkyGradation = _BmdSkyGradation;	//空の下の方のグラデーション
	AddGradation	= _AddGradation;	//地面と空の境目をうめるグラデーション(加算アルファブレンドをする.)

    //空のテクスチャは 90度　で一回りするのです.
    SkyMoveAngle    = _BmdSky->w / (256/4);
}

/***************************************************************/
//前処理	BmdDummyMap に2D としてマップを描きます.
/***************************************************************/
void TMap3D::BeforDraw()
{
    int Gx1,Gy1,Gx2,Gy2,Gx3,Gy3,Gx4,Gy4;
/*
        Gx1 = -(256/2)      - Y;
        Gy1 =  (120)        + Y*2;
        Gx2 =  (256/2)      + Y;
        Gy2 =  (120)        + Y*2;
        Gx3 =  (256/2)      + Y /3;
        Gy3 = 0             ;
        Gx4 = -(256/2)      - Y /3;
        Gy4 = 0             ;   
*/ 
        Gx1 = GetGx1(-128, Y );
        Gy1 = GetGy1( 120, Y );
        Gx2 = GetGx2( 128, Y );
        Gy2 = GetGy2( 120, Y );
        Gx3 = GetGx3( 128, Y );
        Gy3 = GetGy3(   0, Y );   
        Gx4 = GetGx4(-128, Y );
        Gy4 = GetGy4(   0, Y );   

        GRx1 = (int)ROTATEX256(Gx1,Gy1,Angle) + X;
        GRy1 = (int)ROTATEY256(Gx1,Gy1,Angle) + Z;
        GRx2 = (int)ROTATEX256(Gx2,Gy2,Angle) + X;
        GRy2 = (int)ROTATEY256(Gx2,Gy2,Angle) + Z;
        GRx3 = (int)ROTATEX256(Gx3,Gy3,Angle) + X;
        GRy3 = (int)ROTATEY256(Gx3,Gy3,Angle) + Z;
        GRx4 = (int)ROTATEX256(Gx4,Gy4,Angle) + X;
        GRy4 = (int)ROTATEY256(Gx4,Gy4,Angle) + Z;

        int minx,miny;
        minx = GRx1; miny = GRy1;
        if (GRx2 < minx) minx = GRx2;        if (GRy2 < miny) miny = GRy2;
        if (GRx3 < minx) minx = GRx3;        if (GRy3 < miny) miny = GRy3;
        if (GRx4 < minx) minx = GRx4;        if (GRy4 < miny) miny = GRy4;

        Nowx = minx;            Nowy = miny;
        MapHosei(&Nowx,&Nowy);      //補正

        //絵画.
        TMapRotate::Draw(BmdDummyMap);

        //すべての　Rx Ryを minx,miny 座標に変換.
        GRx1 -= minx;        GRy1 -= miny;
        GRx2 -= minx;        GRy2 -= miny;
        GRx3 -= minx;        GRy3 -= miny;
        GRx4 -= minx;        GRy4 -= miny;

}

/***************************************************************/
//後処理	BmdDummyMap に 2D として、描いたマップを 3Dにします.
/***************************************************************/
void TMap3D::AfterDraw(BMD bmd)
{
        //実転送
        ReSizeDraw(BmdDummyMap,GRx1,GRy1,
                      GRx2,GRy2,
                      GRx3,GRy3,
                      GRx4,GRy4,
                      bmd,
                      0,80,
                      320,80,
                      320,240,
                      0 ,240);

	    int SkyStart;
//        SkyStart = ( (255 - Map->GetAngle()) % (256 / 4) ) * SkyMoveAngle;
        SkyStart = ( (255 - Angle) &63 ) * SkyMoveAngle;

        //空だよん
		DirectDraw(BmdSky,0,0,BmdSky->w,BmdSky->h,bmd,SkyStart,0);
		DirectDraw(BmdSky,0,0,BmdSky->w,BmdSky->h,bmd,SkyStart - BmdSky->w,0);
        //空の下のグラデーション
		DirectDraw(BmdSkyGradation,0,0,
            BmdSkyGradation->w,BmdSkyGradation->h,bmd,0,BmdSky->h);
        //マップにたいするグラデーション
		AddDraw(AddGradation,0,0,AddGradation->w,AddGradation->h,bmd,0,80);
}

bool TMap3D::DrawObject(BMD bmd,int x,int y,int w,int h,int ox,int oy,int oz,BMD Sp)
{
    int Rx1,Ry1,Rx2,Ry2,Rx3,Ry3,Rx4,Ry4;
    int Gx1,Gy1,Gx2,Gy2,Gx3,Gy3,Gx4,Gy4;
    int NEX,NEZ;

    //視点座標に変換
    ox -= X;
    oy -= Y;
    oz -= Z;
    //奥にいけばいくほど、 Z をでかくなるようにする.
    oz = -oz;
    //視点による回転.
    NEX = (int)ROTATEX256(ox,oz,Angle);
    NEZ = (int)ROTATEY256(ox,oz,Angle);
    ox  =   NEX;
    oz  =   NEZ;
    oy = - oy;

    if (oz>=0)  return false;   // 視点より後ろ
    if (oz<=-700)  return false;   // 遠すぎるので没
    
    Gx1 = ox;
    Gy1 = oy;
    Gx2 = ox + bmd->w;
    Gy2 = oy;
    Gx3 = ox + bmd->w;
    Gy3 = oy + bmd->h;
    Gx4 = ox;
    Gy4 = oy + bmd->h;

#define APPENDER    32

    //透視投影
    Gx1 = Gx1*APPENDER / oz;
    Gy1 = Gy1*APPENDER / oz;
    Gx2 = Gx2*APPENDER / oz;
    Gy2 = Gy2*APPENDER / oz;
    Gx3 = Gx3*APPENDER / oz;
    Gy3 = Gy3*APPENDER / oz;
    Gx4 = Gx4*APPENDER / oz;
    Gy4 = Gy4*APPENDER / oz;

    //ディスプレイ座標に変換
#define DISPLAY_W   (320/2)
#define DISPLAY_H   (200/2)

    Rx1 = DISPLAY_W + Gx1;    Ry1 = DISPLAY_H - Gy1;
    Rx2 = DISPLAY_W + Gx2;    Ry2 = DISPLAY_H - Gy2;
    Rx3 = DISPLAY_W + Gx3;    Ry3 = DISPLAY_H - Gy3;
    Rx4 = DISPLAY_W + Gx4;    Ry4 = DISPLAY_H - Gy4;

    //実転送
    ReSizeDraw(bmd,x,y,w,h,
                      Sp,
                      Rx1 ,Ry1,
                      Rx2 ,Ry2,
                      Rx3 ,Ry3,
                      Rx4 ,Ry4  );

    return true;
}


#endif //LIFE END
