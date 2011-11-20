#include "StartWithExeFile.h"
#ifdef SPHERE_LIFE

#ifndef ___SPHERE_EFFECTH
#define ___SPHERE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス
#include <.\rti_define.h>

// トンネル中央の位置
#define TUNNEL_CENTER   -1  // 画面中央

// トンネルの大きさ
#define TUNNEL_DEFAULT_SIZE -1 // デフォルト

// トンネル中央
enum TUNNEL_CENTRAL
{
    TUNNEL_NORMAL,   // 普通 ( 拡大も歪みもなし )
    TUNNEL_WIDE_X,   // 左右に拡大
    TUNNEL_WIDE_Y,   // 上下に拡大
    TUNNEL_TO_RIGHT, // 右に歪む
    TUNNEL_TO_LEFT,  // 左に歪む
};

// トンネル形状
enum TUNNEL_SHAPE
{
    TUNNEL_TYPE_CIRCLE, // 円
    TUNNEL_TYPE_BOX1,   // 四角
    TUNNEL_TYPE_BOX2,   // 菱形
    TUNNEL_TYPE_CROSS,  // 十字
    TUNNEL_TYPE_WALL,   // 左右の壁
    TUNNEL_TYPE_GROUND, // 上下の地面
    TUNNEL_TYPE_FLASH,  // フラッシュ効果
};

// トンネル曲線
enum TUNNEL_CURVE
{
    TUNNEL_LINE_NORMAL, // 普通 ( 直線 )
    TUNNEL_LINE_X,      // Ｘ曲線
    TUNNEL_LINE_Y,      // Ｙ曲線
    TUNNEL_LINE_BOX1,   // 四角曲線
    TUNNEL_LINE_BOX2,   // 菱形曲線
    TUNNEL_LINE_CROSS,  // 十字曲線
};

class TSphereEffect
{
private:
    void Init();
    BMD m_OutBmd; //出力
    BMD m_bmd;
    int TunnelDisplayY,TunnelDisplayX;
	unsigned char TunnelSpritePow2W , TunnelSpritePow2H;
	int TunnelSpritePow2MaskW , TunnelSpritePow2MaskH;
    XY *m_PointChangeTable;
public:
    TSphereEffect();
    ~TSphereEffect();
    bool Draw(int ux,int uy , int Rotate , int Dist);
    bool Set(BMD bmd,BMD OutBmd);
    bool CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx=TUNNEL_CENTER,int Cy=TUNNEL_CENTER,
						  int Size=TUNNEL_DEFAULT_SIZE,TUNNEL_CENTRAL Center=TUNNEL_NORMAL,
						  TUNNEL_SHAPE Type=TUNNEL_TYPE_CIRCLE,TUNNEL_CURVE Line=TUNNEL_LINE_NORMAL);
};

#endif


#endif  //LIFE END
