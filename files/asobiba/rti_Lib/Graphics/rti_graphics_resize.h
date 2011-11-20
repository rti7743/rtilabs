#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_RESIZEH
#define GRAPHICS_RESIZEH
/*****************************************************************/
//自由変換
/*****************************************************************/

#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_zbuffer.h>


    struct _XYT
    {
        short x,y;        //座標
        short tx,ty;      //テクスチャ
    };
    struct _XYZT
    {
        short x,y,z;        //座標
        short tx,ty;        //テクスチャ
    };
    struct _XYZCT
    {
        short x,y,z;        //座標
        short cr,cg,cb;     //色
        short tx,ty;        //テクスチャ
    };

_GRAPHICS_BEGIN //namespace begin

    struct _WorkArea        //ふつー
    {
        Fixed StepX,StepTX,StepTY;
    };
    struct _WorkAreaZ       // ZBuffer
    {
        Fixed StepX,StepZ,StepTX,StepTY;
    };
    struct _WorkAreaG       //グローシェーディング
    {
        Fixed StepX,StepZ,StepCR,StepCG,StepCB,StepTX,StepTY;
    };

//通常絵画
    //自由変換 テクスチャー変形
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYT *XY);
    //自由四角形ハッチング.
     void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb);
     void ReSizeBoxMain3(BMD bmd2,_XYT *XY,int Rgb);
//ZBuffer
    //自由変換 テクスチャー変形
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer);
    //自由四角形ハッチング.
     void ReSizeBoxMain(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer);
     void ReSizeBoxMain3(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer);
//グローシェーディングs
    //自由変換 テクスチャー変形
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer);
     void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer);
    //自由四角形ハッチング. 
     void ReSizeBoxMain(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer);
     void ReSizeBoxMain3(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer);

     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDraw(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,XY xy1, XY xy2,XY xy3, XY xy4)
     {
        _XYT XY[4];    //変換した座標を格納. (rx2 , ry2シリーズ)
        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = xy1.x; XY[0].y = xy1.y; XY[0].tx = txy1.x;   XY[0].ty = txy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y; XY[1].tx = txy2.x;   XY[1].ty = txy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y; XY[2].tx = txy3.x;   XY[2].ty = txy3.y;
        XY[3].x = xy4.x; XY[3].y = xy4.y; XY[3].tx = txy4.x;   XY[3].ty = txy4.y;
        //メイン処理
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
    //指定した色で四角形を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBox(BMD bmd2,XY xy1, XY xy2,XY xy3, XYZ xy4,int Rgb)
     {
        _XYT XY[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = xy1.x; XY[0].y = xy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y;
        XY[3].x = xy4.x; XY[3].y = xy4.y;
        //メイン処理
        ReSizeBoxMain(bmd2,XY,Rgb);
     }
     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDrawZ(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, XYZ xyz4,LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[4];    //変換した座標を格納. (rx2 , ry2シリーズ)
        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z; XYZ[3].tx = txy4.x;   XYZ[3].ty = txy4.y;
        //メイン処理
        ReSizeDrawMain(bmd1,bmd2,XYZ,lpZBuffer);
     }
    //指定した色で四角形を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBoxZ(BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, XYZ xyz4,
			                      int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z;
        //メイン処理
        ReSizeBoxMain(bmd2,XYZ,Rgb,lpZBuffer);
     }
     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDrawG(const BMD bmd1,XY txy1, XY txy2,XY txy3, XY txy4,
                 BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 XYZ xyz4,_RGB rgb4,
                 LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[4];    //変換した座標を格納. (rx2 , ry2シリーズ)
        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z; XYZ[3].tx = txy4.x;   XYZ[3].ty = txy4.y;
        XYZ[3].cr = rgb4.r;XYZ[3].cg = rgb4.g;XYZ[3].cb = rgb4.b;  
        //メイン処理
        ReSizeDrawMain(bmd1,bmd2,XYZ,lpZBuffer);
     }
     
    //指定した色で四角形を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBoxG(BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 XYZ xyz4,_RGB rgb4,
                 int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        XYZ[3].x = xyz4.x; XYZ[3].y = xyz4.y; XYZ[3].z = xyz4.z;
        XYZ[3].cr = rgb4.r;XYZ[3].cg = rgb4.g;XYZ[3].cb = rgb4.b;  
        //メイン処理
        ReSizeBoxMain(bmd2,XYZ,Rgb,lpZBuffer);
     }




    //こっからは、三角形バージョンです,

     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDraw3(const BMD bmd1, XY txy1, XY txy2,XY txy3,
                                            BMD bmd2,XY xy1, XY xy2,XY xy3)
     {
        _XYT XY[3];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = xy1.x; XY[0].y = xy1.y; XY[0].tx = txy1.x;   XY[0].ty = txy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y; XY[1].tx = txy2.x;   XY[1].ty = txy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y; XY[2].tx = txy3.x;   XY[2].ty = txy3.y;
        //メイン処理
        ReSizeDrawMain3(bmd1,bmd2,XY);
     }
    //指定したテクスチャ(3角形)を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBox3(BMD bmd2,XY xy1, XY xy2,XY xy3,int Rgb)
     {
        _XYT XY[3];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = xy1.x; XY[0].y = xy1.y;
        XY[1].x = xy2.x; XY[1].y = xy2.y;
        XY[2].x = xy3.x; XY[2].y = xy3.y;
        //メイン処理
        ReSizeBoxMain3(bmd2,XY,Rgb);
     }
     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDrawZ3(const BMD bmd1, XY txy1, XY txy2,XY txy3,
                  BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3, LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[3];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        //メイン処理
        ReSizeDrawMain3(bmd1,bmd2,XYZ,lpZBuffer);
     }
    //指定したテクスチャ(3角形)を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBoxZ3(
           BMD bmd2,XYZ xyz1, XYZ xyz2,XYZ xyz3,int Rgb,LPZBUFFER lpZBuffer)
     {
        _XYZT XYZ[3];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        //メイン処理
        ReSizeBoxMain3(bmd2,XYZ,Rgb,lpZBuffer);
     }
     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDrawG3(const BMD bmd1,XY txy1, XY txy2,XY txy3,
                 BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[3];    //変換した座標を格納. (rx2 , ry2シリーズ)
        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z; XYZ[0].tx = txy1.x;   XYZ[0].ty = txy1.y;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z; XYZ[1].tx = txy2.x;   XYZ[1].ty = txy2.y;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z; XYZ[2].tx = txy3.x;   XYZ[2].ty = txy3.y;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        //メイン処理
        ReSizeDrawMain3(bmd1,bmd2,XYZ,lpZBuffer);
     }
     
    //指定した色で四角形を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBoxG3(BMD bmd2,
                 XYZ xyz1,_RGB rgb1,
                 XYZ xyz2,_RGB rgb2,
                 XYZ xyz3,_RGB rgb3,
                 int Rgb,  LPZBUFFER lpZBuffer)
     {
        _XYZCT XYZ[3];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XYZ[0].x = xyz1.x; XYZ[0].y = xyz1.y; XYZ[0].z = xyz1.z;
        XYZ[0].cr = rgb1.r;XYZ[0].cg = rgb1.g;XYZ[0].cb = rgb1.b;  
        XYZ[1].x = xyz2.x; XYZ[1].y = xyz2.y; XYZ[1].z = xyz2.z;
        XYZ[1].cr = rgb2.r;XYZ[1].cg = rgb2.g;XYZ[1].cb = rgb2.b;  
        XYZ[2].x = xyz3.x; XYZ[2].y = xyz3.y; XYZ[2].z = xyz3.z;
        XYZ[2].cr = rgb3.r;XYZ[2].cg = rgb3.g;XYZ[2].cb = rgb3.b;  
        //メイン処理
        ReSizeBoxMain3(bmd2,XYZ,Rgb,lpZBuffer);
     }

_GRAPHICS_END   //namespace end

#endif

#endif //LIFE END
