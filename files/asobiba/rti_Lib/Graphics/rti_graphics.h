#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICSH
#define GRAPHICSH

#include <.\Base\rti_glookup.h>
#include <.\rti_define.h>
#include <.\rti_namespace.h>
#include <windows.h>
#define BMD_W_POSSITION	0	//BMD の w メンバの位置 (機種依存)
#define BMD_H_POSSITION	4	//BMD の h メンバの位置 (機種依存)
#define BMD_BM_POSSITION 8	//BMD の bm メンバの位置 (機種依存)
#define BMD_POW2W_POSSITION 12//BMD の Pow2W メンバの位置 (機種依存)

#include "rti_graphics_inlines.h" 
#include "rti_graphics_resize.h" 

_GRAPHICS_BEGIN //namespace begin
    //グラフィックス分解
	 int GraphicsCutTile(const BMD bmd,int sizex,int sizey,short *cx,short *cy);
    //拡大縮小
     void BigDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int rw2,int rh2);
    //ぼかし
	 void ShadeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //半透明
     void Blend50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //指定透明
     void BlendDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed rblend,Fixed gblend,Fixed bblend);
    //じょじょに透明
	 void BlendGradetionDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
						BMD bmd2,int rx2,int ry2,
						Fixed fblend,Fixed fglend,Fixed frlend,
						unsigned char mode,int start,int size);
    //半明度調整
     void Bright50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
     //指定明度調整
     void BrightDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend);
    //加算アルファブレンド
     void AddDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //減算アルファブレンド
     void SubDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //Bit And 合成
     void AndDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //Bit Or 合成
     void OrDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //透明色無視の高速絵画
     void DirectDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);
    //反転
     void RevDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1);
    //透明色ありの絵画
     void Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2);    
    //クリア
     void Cls(BMD bmd,BYTE clear = 0x00);
    //ラインを描く
     void Line(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb);
    //四角を欠く
     void Box(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb);
    //円を描く
     void Circle(BMD bmd1,int rx1,int ry1,int rr1,long rgb);
    //上下左右反転
     void MillorDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,bool lr,bool ud);
    //モザイクをかける.
     void MosaicDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,int mw,int mh);
    //指定した成分を加算して表示する.
     void FilterDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      int R,int G,int B);
    //指定された度の数だけ上方向にずらす.
     void ShiftDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int S);
    //回転
	 void RotateDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,Angle seeta);
    //回転3D
	 void Rotate3DDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,int jikuz,
					  unsigned char seetax,unsigned char seetay,unsigned char seetaz);
	//グレイスケールで絵画
	void GrayScaleDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
		                  BMD bmd2,int rx2,int ry2);
     //簡易クリッピング
	bool EasyClipingEx(BMD bmd,int *rx1,int *ry1,int *rw1,int *rh1,
				 int rw2,int rh2,int rx2,int ry2,
				 int *sw,int *sh,int *ew,int *eh);
    //データの頭出し.
	 BYTE *SeekData(BMD bmd,int rx,int ry,
			  int sw,int sh,int *lpAddPitch);

_GRAPHICS_END   //namespace end

USEING_GRAPHICS //ネームスペースの利用.

#define RTIRGB(r,g,b) ((DWORD)(((BYTE)(b) | \
    ((WORD)(g) << 8)) | \
    (((DWORD)(BYTE)(r)) << 16)))

//汎用グラフィックス関数コール.
// DirectDraw   Draw    Blend50Draw     ShadeDraw   Bright50Draw
//  AddDraw SubDraw AndDraw OrDraw  
typedef void (*GraphicsProc)(const BMD,int,int,int,int,BMD,int,int);

#endif


#endif //LIFE END
