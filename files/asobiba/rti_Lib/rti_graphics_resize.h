#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_RESIZEH
#define GRAPHICS_RESIZEH
/*****************************************************************/
//自由変換
/*****************************************************************/

#include <.\Graphics\rti_graphics.h>

    struct _XYT
    {
        int x,y;        //座標
        int tx,ty;      //テクスチャ
    };

//自由変換 メイン部
    //直接呼び出しても結構です (^^;;
     void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY);
//自由四角形ハッチング.
     void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb);

    //指定したテクスチャ(四角形)を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4)
     {
        _XYT XY[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    XY[0].tx = rx1;    XY[0].ty = ry1;
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    XY[1].tx = rx1+rw1;XY[1].ty = ry1;
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    XY[2].tx = rx1+rw1;XY[2].ty = ry1+rh1;
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    XY[3].tx = rx1;    XY[3].ty = ry1+rh1;
        //メイン処理
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
     //指定された複雑な形のテクスチャを自由変換して絵画する.
     inline void ReSizeDrawEx(const BMD bmd1,
                      int rx1_1,int ry1_1,
                      int rx1_2,int ry1_2,int rx1_3,int ry1_3,
                      int rx1_4,int ry1_4,
                      BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4)
     {
        _XYT XY[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    XY[0].tx = rx1_1;   XY[0].ty = ry1_1;
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    XY[1].tx = rx1_2;   XY[1].ty = ry1_2;
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    XY[2].tx = rx1_3;   XY[2].ty = ry1_3;
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    XY[3].tx = rx1_4;   XY[3].ty = ry1_4;
        //メイン処理
        ReSizeDrawMain(bmd1,bmd2,XY);
     }
    //指定したテクスチャ(四角形)を自由変換して絵画する(内部でReSizeDrawMain呼び出し)
     inline void ReSizeBox(BMD bmd2,int rx2_1,int ry2_1,
                      int rx2_2,int ry2_2,int rx2_3,int ry2_3,
                      int rx2_4,int ry2_4,int Rgb)
     {
        _XYT XY[4];    //変換した座標を格納. (rx2 , ry2シリーズ)

        //変換した座標を格納する. (rx2 , ry2シリーズ)
        XY[0].x = rx2_1;    XY[0].y = ry2_1;    
        XY[1].x = rx2_2;    XY[1].y = ry2_2;    
        XY[2].x = rx2_3;    XY[2].y = ry2_3;    
        XY[3].x = rx2_4;    XY[3].y = ry2_4;    
        //メイン処理
        ReSizeBoxMain(bmd2,XY,Rgb);
     }

#endif

#endif //LIFE END
