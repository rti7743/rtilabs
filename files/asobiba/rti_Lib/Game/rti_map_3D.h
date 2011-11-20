#include "StartWithExeFile.h"
#ifdef THE3D_MAP

#ifndef ___MAP3DH
#define ___MAP3DH

#include <.\Game\rti_map_rotate.h>

class TMap3D : public TMapRotate
{
    int GRx1,GRy1,GRx2,GRy2,GRx3,GRy3,GRx4,GRy4;	//BmdDummyMap の中の 転送開始領域の各頂点
	BMD BmdSky;			//空のテクスチャ
	BMD BmdSkyGradation;//空の下の方のグラデーション
	BMD AddGradation;	//地面と空の境目をうめるグラデーション(加算アルファブレンドをする.)
	BMD BmdDummyMap;	//一時的にマップ(2D)を絵画をするところ.
	int SkyMoveAngle;	//1度動くたびの空のテクスチャの増分
    int X,Y,Z;
	unsigned char Angle;	//回転角度.
	int YMax;			//最高高度
public:
    TMap3D();
	~TMap3D();
	BMD GetDummyMap(){return BmdDummyMap;};		//ダミー領域の取得.
    int GetX(){return X;};
    int GetY(){return Y;};
    int GetZ(){return Z;};
	unsigned char GetAngle(){return Angle;};	//アングルの取得
	virtual void SetScreen(int x,int y);	//スクリーンサイズ定義
    //Map3D 透視変換
        // -128 : -Y = X : A      A を求める.  -Y * X / -128
        int GetGx1(int X,int Y){    return (-Y)*X/(-128) + X;   };
        //  128 : Y = X : A      A を求める.  Y * X / 128
        int GetGx2(int X,int Y){    return (Y)*X/(128)   + X;   };
        //  128 : (Y/3) = X : A      A を求める.  (Y/3) * X / 128
        int GetGx3(int X,int Y){    return (Y/3)*X/(128) + X;   };
        //  -128 : (-Y/3) = X : A      A を求める.  (-Y/3) * X / -128
        int GetGx4(int X,int Y){    return (-Y/3)*X/(-128) + X;   };
        // 120 : (Y*2) = Z : A      A を求める.  (Y*2) * Z / 120
        int GetGy1(int Z,int Y){    return (Y*2) * Z / 120 + Z;   };
        // 120 : (Y*2) = Z : A      A を求める.  (Y*2) * Z / 120 (Gy2 と同じ)
        int GetGy2(int Z,int Y){    return (Y*2) * Z / 120 + Z;   };
        // 0 : 0 = Z : A             答えは 0 だよん (^^;;;
        int GetGy3(int Z,int Y){    return 0 + Z;   };
        // 0 : 0 = Z : A             答えは 0 だよん (^^;;; (Gy4 と同じ)
        int GetGy4(int Z,int Y){    return 0 + Z;   };
	//各種移動
	virtual void UpProc(int p){		Y-=p;	if (Y < 0 ) Y = 0;	};
	virtual void DownProc(int p){	Y+=p;	if (Y > YMax ) Y = YMax;	};
	virtual void LeftProc(unsigned char p){	Angle-=p;	};
	virtual void RightProc(unsigned char p){Angle+=p;	};
	virtual void AfterProc(int p)
	{
		    unsigned char RR = 255- Angle;
	        X -= ANGLE_MOVEX(RR,p);
	        Z -= ANGLE_MOVEY(RR,p);
		    MapHosei(&X,&Z);      //補正
	};
	virtual void BeforProc(int p)
	{
		    unsigned char RR = 255- Angle;
	        X += ANGLE_MOVEX(RR,p);
	        Z += ANGLE_MOVEY(RR,p);
		    MapHosei(&X,&Z);      //補正
	};
    //2次元(マップ)に物体表示.
    void DrawObject2D(GraphicsProc Proc,BMD bmd,int x,int y,int w,int h,int ox,int oy)
    {
        Proc(bmd,x,y,w,h,BmdDummyMap,ox,oy);
    };
    //3次元物体表示	
    bool DrawObject(BMD bmd,int x,int y,int w,int h,int ox,int oy,int oz,BMD Sp);
	//各種テクスチャの準備.
	void UsingTexture(BMD _BmdSky,BMD _BmdSkyGradation,BMD _AddGradation);
	//マップを絵画します(円筒スクロール 3D)
	virtual void Draw(BMD bmd){	BeforDraw();	AfterDraw(bmd);	}
	//前処理	BmdDummyMap に2D としてマップを描きます.
	void BeforDraw();
	//後処理	BmdDummyMap に 2D として、描いたマップを 3Dにします.
	void AfterDraw(BMD bmd);
};

#endif

#endif //LIFE END
