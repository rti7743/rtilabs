#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

/*****************************************************************/
//自由変換( N角形の絵画 )  最高に汚いのであんまり見ないでー
/*****************************************************************/

#include <.\Graphics\rti_graphics_resize.h>
#include <.\Base\rti_debug.h>
#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_glookup.h>

_GRAPHICS_BEGIN //namespace begin

Fixed X1 , X2;                  //実転送開始場所X
Fixed TX1, TX2;                 //実転送開始場所テクスチャーX
Fixed TY1, TY2;                 //実転送開始場所テクスチャーY
Fixed Z1 , Z2;                  //実転送開始場所Z
Fixed CR1, CR2;                 //実転送開始場所グラデーションR
Fixed CG1, CG2;                 //実転送開始場所グラデーションG
Fixed CB1, CB2;                 //実転送開始場所グラデーションB
int DeltaXYTStart;   		    //スタートする _WorkArea の場所.
int DeltaXYTMax;				//終了する _WorkArea の場所.


//テクスチャを指定した形に変換.
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkArea *WA,int *StopY,int StartY);
//自由絵画で　指定した図形を指定した色で絵画
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkArea *WA,int *StopY,int StartY);
//テクスチャを指定した形に変換. ZBuffer
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//自由絵画で　指定した図形を指定した色で絵画 ZBuffer 
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//テクスチャを指定した形に変換. グロー
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//自由絵画で　指定した図形を指定した色で絵画 グロー
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);


 


/*****************************************************************/
//自由変換( N角形の絵画 ) の下請けルーチン
/*****************************************************************/

//規約違反の あえていえば、弓形の四角形なのれす.
enum enumOFFENDER
{
	OFFENDER_NONE ,
	OFFENDER_LEFT ,
	OFFENDER_RIGHT ,
};
/* *******************************************************
*       重要な説明!!!
*  *******************************************************
*
* このルーチンは、四角、三角形を絵画するために作られましたが、
* それをゼットバッファ付きで絵画、フラットシェーディング、
* グローシェーディングで絵画のルーチンをプラスします.
* そのためテンプレートを導入して、コーディング量を減らします.
*
*  template<class _COMMXYT>
*  template<class _COMMWORKAREA>
*
*  通常         XYT     _WorkArea
*  ZBuffer      XYZT    _WorkAreaZ
*  フラット     XYZT    _WorkAreaZ
*  グロー       XYZCT   _WorkAreaG
*/

#define WORK_AREA_GET(p,lr) ( (p<<1)+lr)
//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
template<class _COMMXYT , class _COMMWORKAREA>
static int XYBrokeringAndSettingWorkArea(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd);
//T と B の直線の式に 点 P の X を入れて y を求める.
template<class _COMMXYT>
static int Ypos(const _COMMXYT* Top ,const _COMMXYT* Buttom ,const _COMMXYT* P );

//各頂点　が Top Buttom Left Right のどれなのか決める
template<class _COMMXYT>
static void SettingVector(_COMMXYT *XY,_COMMXYT **_Top,_COMMXYT **_Buttom,
                          _COMMXYT **_Left,_COMMXYT **_Right,enumOFFENDER *_Offender);
//イカさない四角形かどうか調べる
template<class _COMMXYT>
static enumOFFENDER IsOffenderSquare(int Lpos,int Rpos,
			 const _COMMXYT *Left,const _COMMXYT *Right,const _COMMXYT *Top,const _COMMXYT *Buttom);

//へちまで 左と右が逆かどうか調べる. 逆だったら true
template<class _COMMXYT>
static bool IsHechimaAndLeftRightRev(int Lpos , const _COMMXYT *Left,
                              const _COMMXYT *Right,const _COMMXYT *Top ,
                              const _COMMXYT *Buttom);

//各頂点間の増分を調べる(イカさない四角形バージョン)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorOffenderSquareRightSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);

//各頂点間の増分を調べる(イカさない四角形バージョン)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorOffenderSquareLeftSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);
//各頂点間の増分を調べる(ふつーの四角形)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorNormalSquare(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);
//二つの頂点間の Y にたいする傾きを求める.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector(_COMMWORKAREA *WA,int *StopY,
       const _COMMXYT *Left,const _COMMXYT *Right,
       const _COMMXYT *Top ,const _COMMXYT *Buttom,enumOFFENDER OffenderSquare);

//そもそも絵画自体をしていいのかどうか調べる.
template<class _COMMXYT>
static bool IsDrawOk(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
                     const _COMMXYT *Buttom);

/*****************************************************************/
/*****************************************************************/
//三角形バージョン
/*****************************************************************/
/*****************************************************************/
//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
template<class _COMMXYT , class _COMMWORKAREA>
static int XYBrokeringAndSettingWorkArea3(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd);
//3方の座標を決定.
template<class _COMMXYT>
static void SettingVector3(_COMMXYT * XY,_COMMXYT **Top,
                           _COMMXYT **Buttom,_COMMXYT **Median);

//そもそも絵画自体をしていいのかどうか調べる.
template<class _COMMXYT>
static bool IsDrawOk3(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
                      const _COMMXYT *Buttom);

//頂点同士の傾きをもつめる.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3(_COMMWORKAREA *WA,int *StopY,const _COMMXYT *Top,
                        const _COMMXYT *Buttom,const _COMMXYT *Median);

//右向き三角形の各頂点別の傾きを求めます.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3Left(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median);

//右向き三角形の各頂点別の傾きを求めます.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3Right(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median);


/*****************************************************************/
/*****************************************************************/
//統合.
/*****************************************************************/
/*****************************************************************/
//そもそも　かいがしたい図形なのか?
//その図形の本当の頂点数を返す.
template<class _COMMXYT>
static int IsTarget(_COMMXYT *XY,int NumVec);
//Y座標のクリッピング.
template<class _COMMWORKAREA>
static bool YClipping(int &Y,int *StopY,_COMMWORKAREA * &lpWA,_COMMWORKAREA * &WAL,_COMMWORKAREA * &WAR,BMD const bmd2);
//二つの頂点間の Y にたいする傾きを求める.  ふつー
static void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E);
//二つの頂点間の Y にたいする傾きを求める.  Z Buffer
static void HokanLine(_WorkAreaZ *WA,const _XYZT *B,const _XYZT *E);
//二つの頂点間の Y にたいする傾きを求める.  グロー
static void HokanLine(_WorkAreaG *WA,const _XYZCT *B,const _XYZCT *E);
//各スタート地点の設定 ふつー
static void StartSet(const _XYT *SL,const _XYT *SR);
//各スタート地点の設定 ZBuffer
static void StartSet(const _XYZT *SL,const _XYZT *SR);
//各スタート地点の設定 ぐろー
static void StartSet(const _XYZCT *SL,const _XYZCT *SR);
//指定された Step だけ、各値を増分させる.  ふつー
static void StepUp(const _WorkArea *WAL,const _WorkArea *WAR,int Step);
//指定された Step だけ、各値を増分させる.  ZBuffer
static void StepUp(const _WorkAreaZ *WAL,const _WorkAreaZ *WAR,int Step);
//指定された Step だけ、各値を増分させる.  グロー
static void StepUp(const _WorkAreaG *WAL,const _WorkAreaG *WAR,int Step);


/*****************************************************************/
//自由変換 メイン部
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y);
}
/*****************************************************************/
//自由変換 メイン部(三角形バージョン)
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    //そもそもそれが三角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y);
}
/*****************************************************************/
//自由四角形ハッチング.
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y);
}
/*****************************************************************/
//自由四角形ハッチング.三角形バージョン
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYT *XY,int Rgb)
{
    //そもそもそれが三角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y);
}
/*****************************************************************/
//自由変換 メイン部 ZBuffer
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//自由変換 メイン部(三角形バージョン) ZBuffer
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//自由四角形ハッチング. ZBuffer
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//自由四角形ハッチング.三角形バージョン ZBuffer
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}


/*****************************************************************/
//自由変換 メイン部 グロー
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//自由変換 メイン部(三角形バージョン) グロー
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.
    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //テクスチャを指定した形に変換.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//自由四角形ハッチング. グロー
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}

/*****************************************************************/
//自由四角形ハッチング.三角形バージョン グロー
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //そもそもそれが四角形なのかどうかチェック.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}


/*****************************************************************/
//テクスチャを指定した形に変換.
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkArea *WA,int *StopY,int StartY)
{
    _WorkArea *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkArea *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2;
    BYTE *data1,*data2;
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
		    register int Temp;		//作業領域.
            register BYTE *d2;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalTX += LocalStepTX * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalTY += LocalStepTY * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                d2 = data2 + 0 + 0 + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                DirectPSet(d2, data1+Temp+Temp+Temp );
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;
            }
        }
		//X,テクスチャの増分を追加だよん.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//自由絵画で　指定した図形を指定した色で絵画
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkArea *WA,int *StopY,int StartY)
{
    _WorkArea *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkArea *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	CommDraw->GraphicsLock(bmd2);

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    int *lpStopY;
	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
            register BYTE *d2;
            register int Temp;		//作業領域.
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                d2 = data2 + 0 + 0 + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, Rgb );
                d2 += 3;
            }
		}
		//X の増分を加える.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        data2 += AddPitch2;
    }
	CommDraw->GraphicsUnLock(bmd2);
}

/*****************************************************************/
//テクスチャを指定した形に変換. ZBuffer
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaZ *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkAreaZ *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2,AddZBufferPitch;
    BYTE *data1,*data2;
    Fixed *Zb;      //ゼットバッファのバッファそのもののアドレス
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
		    register int Temp;		//作業領域.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalZ  += LocalStepZ  * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalTX += LocalStepTX * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalTY += LocalStepTY * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
                tZb = Zb + StartX;
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer より、小さいデータだったら絵画.
                    *tZb = LocalZ;    //Z Buffer の更新.
                    Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                    DirectPSet(d2, data1+Temp+Temp+Temp );
                }
                LocalZ  += LocalStepZ;
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;    //次のデータへ.
                tZb ++; //次の ZBuffer データへ.
            }
        }
		//X,テクスチャの増分を追加だよん.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//自由絵画で　指定した図形を指定した色で絵画 ZBuffer 
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaZ *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkAreaZ *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	CommDraw->GraphicsLock(bmd2);

    int AddPitch2,AddZBufferPitch;
    BYTE *data2;
    Fixed *Zb;      //ゼットバッファのバッファそのもののアドレス
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;

    int *lpStopY;
	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
            register BYTE *d2;
            register int Temp;		//作業領域.
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalZ  += LocalStepZ  * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
                tZb = Zb + StartX;
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer より、小さいデータだったら絵画.
					*tZb = LocalZ;
                    DirectPSet(d2, Rgb );
                }
                LocalZ  += LocalStepZ;
                d2 += 3;    //次のデータへ.
                tZb ++; //次の ZBuffer データへ.
            }
		}
		//X の増分を加える.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
}


/*****************************************************************/
//テクスチャを指定した形に変換. グロー
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaG *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkAreaG *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2,AddZBufferPitch;
    BYTE *data1,*data2;
    Fixed *Zb;      //ゼットバッファのバッファそのもののアドレス
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
		    register int Temp;		//作業領域.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalCR   ,   LocalStepCR;
            Fixed LocalCG   ,   LocalStepCG;
            Fixed LocalCB   ,   LocalStepCB;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalCR = CR1;      LocalStepCR = (CR2-CR1) / DrawLen;
            LocalCG = CG1;      LocalStepCG = (CG2-CG1) / DrawLen;
            LocalCB = CB1;      LocalStepCB = (CB2-CB1) / DrawLen;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalZ  += LocalStepZ  * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalCR += LocalStepCR * StartX; // 加算画像量の色 R
                LocalCG += LocalStepCG * StartX; // 加算画像量の色 G
                LocalCB += LocalStepCB * StartX; // 加算画像量の色 B
                LocalTX += LocalStepTX * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalTY += LocalStepTY * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
                tZb = Zb + StartX;
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer より、小さいデータだったら絵画.
					*tZb = LocalZ;
                    Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                    DirectPSet(d2, data1+Temp+Temp+Temp ,
                       (FI(LocalCR) << 16) + (FI(LocalCG) << 8) + FI(LocalCB) );
                }
                LocalZ  += LocalStepZ;
                LocalCR += LocalStepCR;
                LocalCG += LocalStepCG;
                LocalCB += LocalStepCB;
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;    //次のデータへ.
                tZb ++; //次の ZBuffer データへ.
            }
        }
		//X,テクスチャの増分を追加だよん.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        CR1 += WAL->StepCR;      CR2 += WAR->StepCR;
        CG1 += WAL->StepCG;      CG2 += WAR->StepCG;
        CB1 += WAL->StepCB;      CB2 += WAR->StepCB;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//自由絵画で　指定した図形を指定した色で絵画 グロー
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaG *lpWA ;			    //現在の _WorkArea .(yが1進んだときの各種の増分値)
    _WorkAreaG *WAL,*WAR;			//現在の左右を担当している_WorkArea.
    int y = StartY;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch2,AddZBufferPitch;
    BYTE *data2;
    Fixed *Zb;      //ゼットバッファのバッファそのもののアドレス
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int *lpStopY;

	//実転送ループ
    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
		    register int Temp;		//作業領域.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalCR   ,   LocalStepCR;
            Fixed LocalCG   ,   LocalStepCG;
            Fixed LocalCB   ,   LocalStepCB;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalCR = CR1;      LocalStepCR = (CR2-CR1) / DrawLen;
            LocalCG = CG1;      LocalStepCG = (CG2-CG1) / DrawLen;
            LocalCB = CB1;      LocalStepCB = (CB2-CB1) / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalZ  += LocalStepZ  * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalCR += LocalStepCR * StartX; // 加算画像量の色 R
                LocalCG += LocalStepCG * StartX; // 加算画像量の色 G
                LocalCB += LocalStepCB * StartX; // 加算画像量の色 B
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
                tZb = Zb + StartX;
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer より、小さいデータだったら絵画.
					*tZb = LocalZ;
                    DirectPSet(d2, Rgb ,
                        (FI(LocalCR) << 16) + (FI(LocalCG) << 8) + FI(LocalCB) );
                }
                LocalZ  += LocalStepZ;
                LocalCR += LocalStepCR;
                LocalCG += LocalStepCG;
                LocalCB += LocalStepCB;
                d2 += 3;    //次のデータへ.
                tZb ++; //次の ZBuffer データへ.
            }
        }
		//X,テクスチャの増分を追加だよん.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        CR1 += WAL->StepCR;      CR2 += WAR->StepCR;
        CG1 += WAL->StepCG;      CG2 += WAR->StepCG;
        CB1 += WAL->StepCB;      CB2 += WAR->StepCB;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
}



















/*****************************************************************/
//自由変換( N角形の絵画 ) の下請けルーチン
/*****************************************************************/

/*****************************************************************/
/*****************************************************************/
//四角形バージョン
/*****************************************************************/
/*****************************************************************/

/*****************************************************************/
//各頂点　が Top Buttom Left Right のどれなのか決める
/*****************************************************************/
template<class _COMMXYT>
void SettingVector(_COMMXYT *XY,_COMMXYT **_Top,_COMMXYT **_Buttom,
                   _COMMXYT **_Left,_COMMXYT **_Right,enumOFFENDER *_Offender)
{
    /*
    *     T          T                  R
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         L--------------------B  
    *   \/
    *   B
    *   上のようになります.
    * T とは、 y が最小　　y == y があったら、x が小さい方.
    */
    _COMMXYT *Left,*Right,*Top,*Buttom;
    _COMMXYT *Temp;
	enumOFFENDER Offender;
    register int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
    //Top を求める.
    BackupI1 = BackupI2 = 0;
    Buttom = Top = &XY[0];
    for(i = 0; i < 4; i++)
    {
        Temp = &XY[i];
        if ( Temp->y < Top->y  ||
                    ( Temp->y == Top->y && Temp->x < Top->x))
        {
            Top = Temp;   BackupI1 = i;
        }
        if (Temp->y > Buttom->y  || 
                    ( Temp->y == Buttom->y && Temp->x > Buttom->x))
        {
            Buttom = Temp;    BackupI2 = i;
        }
    }
    //Top の場所を記録
    SETBIT(OkFlg,BackupI1);
    //Buttom の場所を記録
    SETBIT(OkFlg,BackupI2);
    Left = NULL;
    //Top Buttom 以外の 2つから、 Left Right を求める.
    for(i = 0; i < 4; i++)
    {
        if (! GETBIT(OkFlg,i) )
        {
            Temp = &XY[i];
            //残りはどちらかしかないので、片方ではなかったら　もう片方です.
            if (Left==NULL)
            {
                Left = Temp;
            }
            else
            {
                Right = Temp;
                break;  //Left , Right が求まったので　これ以上のループ続行は無意味.
            }
        }
    }

    if (Left->x > Right->x)
    {   //交換
        Temp = Left ;   Left = Right;   Right = Temp;
    }

    int Rpos , Lpos ;
	Rpos = Ypos(Top,Buttom,Right);	//Top と Buttom の直線式に Right->x を代入して Y を求める.
	Lpos = Ypos(Top,Buttom,Left);	//Top と Buttom の直線式に Left ->x を代入して Y を求める.

	Offender = IsOffenderSquare(Lpos,Rpos,Left,Right,Top,Buttom);
    if ( Offender != OFFENDER_NONE )
    {   
        /*      
        *    T
        *    |\
        *    | \        こんな、菱形になっていない図形は、
        *    |  \ L     イレギュラー(イングリッシュでいうと格好良いですね)
        *    |   |      な、図形として別途の処理をします.
        *    |   |      また、このときの図形の L R は、    
        *    |  / R     かならず、 Left->y > Right->y でなくては、
        *    | /        なりまへん.
        *    |/
        *    B
        */
        if (Left->y > Right->y)
        {
            Temp = Left ;   Left = Right;   Right = Temp;
        }
    }
    else
    {
        /*
        *         T
        *        / | 
        *       L  |  
        *      /   \    現在だと、このような図形　へちま型と命名しなくてもいい.
        *     /   \     の、 L と R が入れ替わって絵画されないときがあります.
        *    /   \      
        *    |  \       
        *    B-R
        */
		if (IsHechimaAndLeftRightRev(Lpos,Left,Right,Top,Buttom) )
		{
	        Temp = Left ;   Left = Right;   Right = Temp;
		}
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Left = Left;
    *_Right = Right;
    //イカさない四角形かどうかを返すのです.
	*_Offender = Offender;
}
/*****************************************************************/
//T と B の直線の式に 点 P の X を入れて y を求める.
/*****************************************************************/
template<class _COMMXYT>
int Ypos(const _COMMXYT* Top ,const _COMMXYT* Buttom ,const _COMMXYT* P )
{
	if (Top->x == Buttom->x)
	{
		return Top->x;
	}
	return (Top->y - Buttom->y) * 
		    (P->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
}

 
/*****************************************************************/
//イカさない四角形かどうか調べる
/*****************************************************************/
template<class _COMMXYT>
enumOFFENDER IsOffenderSquare(int Lpos , int Rpos,
					  const _COMMXYT *Left,const _COMMXYT *Right,
					  const _COMMXYT *Top ,const _COMMXYT *Buttom)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          か、どうか調べる.
        *    |  / R
        *    | /
        *    |/
        *    B
        */
    /*
        判別アルゴリズム.
            上のような図形の場合、 画面を 線TB に二つに分割したとすると、
            点 L , R は 線TB の　右側　か　左側のどちらかに両方ともに
            あらわれます.
            これが、菱形などになると、 点 L , R は、 線TB の
            右と左に分離します.
            その特性を使うことによって、判別しています.
    */
    if (Top->x != Buttom->x)
    {
        if (Lpos < Left->y && Rpos < Right->y)
        {
            if (Top->x < Buttom->x)  return OFFENDER_LEFT;
            else                     return OFFENDER_RIGHT;
        }
        if (Lpos > Left->y && Rpos > Right->y)
        {
            if (Top->x < Buttom->x)  return OFFENDER_RIGHT;
            else                     return OFFENDER_LEFT;
        }
    }
    else
    {
        if (Top->x < Left->x && Top->x < Right->x) return OFFENDER_RIGHT;
        if (Top->x > Left->x && Top->x > Right->x) return OFFENDER_LEFT;
    }
    //ふつーの四角形.
    return OFFENDER_NONE;
}
/*****************************************************************/
//へちまで 左と右が逆かどうか調べる. 逆だったら true
/*****************************************************************/
template<class _COMMXYT>
bool IsHechimaAndLeftRightRev(int Lpos ,
					  const _COMMXYT *Left,const _COMMXYT *Right,
					  const _COMMXYT *Top ,const _COMMXYT *Buttom)
{
	if (Top->x < Buttom->x)
	{
		if (Lpos > Left->y)
		{
			return true;
		}
	}
	else if (Top->x > Buttom->x)
	{
		if (Lpos < Left->y)
		{
			return true;
		}
	}
	return false;
}

/*****************************************************************/
//各頂点間の増分を調べる
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
{
    //規約違反な図形かな?
    //規約違反の図形は、下の図みたいなやつ.
    //菱形や長方形ぢゃないタイプです.
	switch(OffenderSquare)
	{
	case OFFENDER_LEFT:
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  こんな感じの図形
        *    |  |          は、特別なルーチンが必要です(涙)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            return HokanVectorOffenderSquareLeftSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
			break;
	case OFFENDER_RIGHT:
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          は、特別なルーチンが必要です(涙)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            return HokanVectorOffenderSquareRightSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
			break;
	case OFFENDER_NONE:
        //通常の　長方形、正方形、　菱形、台形など.
        return HokanVectorNormalSquare
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
		break;
	}
	//エラー呼吸(^^;;)
	return -1;
}

/*****************************************************************/
//各頂点間の増分を調べる(イカさない四角形バージョン)
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorOffenderSquareRightSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          は、特別なルーチンが必要です(涙)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            //T -> L について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Left);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Buttom);

            //第一ストップポイント
            StopY[0] = Left->y;    //L の Y で一休み
            if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
				if (Left->y == Right->y)
				{	//これは、 T L R の y が等しい つまり、三角形ですね(涙)
	                HokanLine(&WA[WORK_AREA_GET(0,1)],Right,Buttom);  //WaR = R => B まで
	                StopY[0] = Buttom->y;     //R の Y で一休み
					return 0;
				}
                HokanLine(&WA[WORK_AREA_GET(0,1)],Left,Right);  //WaR = L => R まで
                //WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[0] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
                return 1;
            }
            else
            {
                if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
				if (Left->y == Right->y)
				{	//Left と Right の Y が等しいってことは、
                    //三角形になります.
                    HokanLine(&WA[WORK_AREA_GET(1,1)],Left,Buttom);  //WaR = L => B まで
					WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];	//T->B
					return 1;
				}
                HokanLine(&WA[WORK_AREA_GET(1,1)],Left,Right);  //WaR = L => R まで
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 1;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[2] = Buttom->y;     //B の Y で一休み
                return 2;
            }
}

/*****************************************************************/
//各頂点間の増分を調べる(イカさない四角形バージョン)
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorOffenderSquareLeftSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
{
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  こんな感じの図形
        *    |  |          は、特別なルーチンが必要です(涙)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            //T -> L について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Buttom);
            //第一ストップポイント
            StopY[0] = Left->y;    //L の Y で一休み
            if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Right);  //WaR = L => R まで
                //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[0] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(1,0)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
                return 1;
            }
            else
            {
				if (Left->y == Right->y)
				{	//Left と Right の Y が等しいってことは、
                    //三角形になります.
                    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaR = L => B まで
					WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];	//T->B
					return 1;
				}
                HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Right);  //WaR = L => R まで
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 1;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(2,0)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[2] = Buttom->y;     //B の Y で一休み
                return 2;
            }
}
/*****************************************************************/
//各頂点間の増分を調べる(ふつーの四角形)
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorNormalSquare(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
{
    //T -> L について.
    HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
    //T -> R について.
    HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Right);
    //第一ストップポイント
    if (Right->y < Left->y)
    {
        StopY[0] = Right->y;    //R の Y で一休み
        if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
		if (Top->y == StopY[0])
		{	//上の辺が同じ高さにある四角形だった場合
			HokanLine(&WA[WORK_AREA_GET(0,1)],Right,Buttom);  //WaR = R => B まで
			//WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

			StopY[0] = Left->y;     //L の Y で一休み
	        if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B まで
			WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaR は、前回の続きを実行する.
			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
			HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom);  //WaR = R => B まで
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

			StopY[1] = Left->y;     //L の Y で一休み
	        if (Left->y == Buttom->y)   return 1;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(2,0)],Left,Buttom);  //WaL = L => B まで
			WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaR は、前回の続きを実行する.
			StopY[2] = Buttom->y;
			return 2;
		}
    }
    else if (Right->y > Left->y)
    {
        StopY[0] = Left->y;    //L の Y で一休み
        if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		if (Top->y == StopY[0])
		{	//上の辺が同じ高さにある四角形だった場合
	        HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Buttom);  //WaL = L => B まで
		    //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaR は、前回の続きを実行する.

			StopY[0] = Right->y;     //R の Y で一休み
	        if (Right->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B まで
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL は、前回の続きを実行する.

			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
	        HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B まで
		    WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaR は、前回の続きを実行する.

			StopY[1] = Right->y;     //R の Y で一休み
	        if (Right->y == Buttom->y)   return 1;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B まで
			WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL は、前回の続きを実行する.

			StopY[2] = Buttom->y;
			return 2;
		}
    }
    //ここは、 Left->y == Right->y の場合.
    //つまり、綺麗な菱形です.
    StopY[0] = Left->y;    //L,R の Y で一休み
    if (Left->y == Buttom->y)   return 0;   //L,R と B が同じ高さだったらおしまい.
    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B まで
    HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B まで
	StopY[1] = Buttom->y;
    return 1;
}



//そもそも絵画自体をしていいのかどうか調べる.
template<class _COMMXYT>
bool IsDrawOk(const BMD bmd,const _COMMXYT *XY,
              const _COMMXYT *Top,const _COMMXYT *Buttom)
{
	int w,h;
	w = bmd->w;	h = bmd->h;
	//すべて右側の星の果て.
	if (XY[0].x < 0 && XY[1].x < 0 && XY[2].x < 0 && XY[3].x < 0)	return false;
	//すべて左側の銀河の向こう.
	if (XY[0].x >= w && XY[1].x >= w && XY[2].x >= w && XY[3].x >= w)	return false;
	//上と下のあっちの世界に逝っちゃった.
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= h && Buttom->y >= h))	return false;
	
	//帰ってきてくれたんだね. 絵画しても ok よん.
	return true;
}


//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
template<class _COMMXYT , class _COMMWORKAREA>
int XYBrokeringAndSettingWorkArea(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd)
{
    _COMMXYT *Left,*Right,*Top,*Buttom;
    enumOFFENDER OffenderSquare;
    //四方の座標を決定.
    SettingVector(XY,&Top,&Buttom,&Left,&Right,&OffenderSquare);
	//そもそも絵画自体をしていいのかどうか調べる.
	if ( !IsDrawOk(bmd,XY,Top,Buttom))		return bmd->h;
    /*
    *     T          L                  T
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         B--------------------R  
    *   \/
    *   B
    * 基本的な図形の菱形は、
    * T.y ～　B.y までの移動になるが、
    * T.y ～　R.y    0回目転送
    * R.y ～　L.y    1回目転送
    * L.y ～　B.y    2回目転送
    * の最大　3 回転送にわけて処理する.
    */
    DeltaXYTStart = 0;				//だから、最初に使うワークエリアは最初です.
    DeltaXYTMax = HokanVector(WA,StopY,
                    Left,Right,Top,Buttom,OffenderSquare);
    //スタート位置の決定.
    //X1 TX1 TY1 X2 TX2 TY2 はスタティックなグローバル変数ですので注意.
    _COMMXYT *SL,*SR;                   //スタート地点
    //スタート地点設定 スタート地点は、 ひし形の場合は、 右左は、 T->y から始まりますが、
	//正方形などは、 右 T 左 L などから始まります.
    SL = SR = Top;
    if (OffenderSquare != OFFENDER_NONE)
    {	//ふつーぢゃない四角形の場合.
        if (Top->y == Left->y)
        {	//T L の地点が等しい場合、 スタート地点は常に T からとはかぎらない.
			if (Left->y != Right->y)
			{	// T L が一番小さい y をもっている (スタート地点)
				if (Top->x > Left->x)   SL = Left;
				else                    SR = Left;
			}
			else
			{	//念のため、 T L Rの y が等しいときのために調べておきます
				//これが成立するのは、 三角形がきたときだけです.
				//そもそも、 四角形書くルーチンに三角形をおくるな.
				if (Top->x > Right->x)  SL = Right;
				else                    SR = Right;
			}
        }
    }
    else
    {
        if (Top->y == Left->y)      SL = Left;
        if (Top->y == Right->y)     SR = Right;
    }
    //各スタート地点セット
    StartSet(SL,SR);
    //転送開始 Y 座標.
    return Top->y;
}

///////////////////////////////////////////////////////////////////////////////////////////
//三角形バージョン
///////////////////////////////////////////////////////////////////////////////////////////

//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
template<class _COMMXYT , class _COMMWORKAREA>
int XYBrokeringAndSettingWorkArea3(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd)
{
    _COMMXYT *Median,*Top,*Buttom;

    //3方の座標を決定.
    SettingVector3(XY,&Top,&Buttom,&Median);
	//そもそも絵画自体をしていいのかどうか調べる.
	if ( !IsDrawOk3(bmd,XY,Top,Buttom))		return bmd->h;
    /*
    *     T
    *    / |	三角形の秘密!!.
    *   /  |
    *  /  |
    *M/   | 
    *  \ |
    *   \
    *   B
    */
    DeltaXYTStart = 0;				//だから、最初に使うワークエリアは最初です.
    DeltaXYTMax = HokanVector3(WA,StopY,Top,Buttom,Median);
    //スタート位置の決定.
    //X1 TX1 TY1 X2 TX2 TY2 はスタティックなグローバル変数ですので注意.
    _COMMXYT *SL,*SR;                   //スタート地点
    //スタート地点設定
    SL = SR = Top;
    if (Top->y == Median->y)   
	{
		if (Top->x > Median->x)	SL = Median;
		else                    SR = Median;
	}
    //各スタート地点セット
    StartSet(SL,SR);
    //転送開始 Y 座標.
    return Top->y;
}

//3方の座標を決定.
template<class _COMMXYT>
void SettingVector3(_COMMXYT * XY,_COMMXYT **_Top,
                    _COMMXYT **_Buttom,_COMMXYT **_Median)
{
    /*
        *       T
        *      /|
        *     / |
        *    /  | 
        *  M/   |  
        *   \   |
        *    \  |
        *     \ |
        *      \|
        *       B
    *   上のようになります.
    * T とは、 y が最小　　y == y があったら、x が小さい方.
    * B とは、 y が最大.
    * M は、それ以外の点.
    */
    _COMMXYT *Top,*Buttom,*Median;
    _COMMXYT *Temp;
    int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
    //Top を求める.
    BackupI1 = BackupI2 = 0;
    Buttom = Top = &XY[0];
    for(i = 0; i < 3 ; i++)
    {
        Temp = &XY[i];
        if ( Temp->y < Top->y  ||
                    ( Temp->y == Top->y && Temp->x < Top->x))
        {
            Top = Temp;   BackupI1 = i;
        }
        if (Temp->y > Buttom->y  || 
                    ( Temp->y == Buttom->y && Temp->x > Buttom->x))
        {
            Buttom = Temp;    BackupI2 = i;
        }
    }
    //Top の場所を記録
    SETBIT(OkFlg,BackupI1);
    //Buttom の場所を記録
    SETBIT(OkFlg,BackupI2);
    //Top Buttom 以外の 1から、 Median を求める.
    for(i = 0; i < 3 ; i++)
    {
        if (! GETBIT(OkFlg,i) )
        {
            Median = &XY[i];
            break;
        }
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Median = Median;
}

//そもそも絵画自体をしていいのかどうか調べる.
template<class _COMMXYT>
bool IsDrawOk3(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
               const _COMMXYT *Buttom)
{
	int w,h;
	w = bmd->w;	h = bmd->h;
	//すべて右側の星の果て.
	if (XY[0].x < 0 && XY[1].x < 0 && XY[2].x < 0)	return false;
	//すべて左側の銀河の向こう.
	if (XY[0].x >= w && XY[1].x >= w && XY[2].x >= w)	return false;
	//上と下のあっちの世界に逝っちゃった.
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= h && Buttom->y >= h))	return false;
	
	//帰ってきてくれたんだね. 絵画しても ok よん.
	return true;
}

//頂点同士の傾きをもつめる.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,
                 const _COMMXYT *Median)
{
    int Mpos ;
	Mpos = Ypos(Top,Buttom,Median);	//Top と Buttom の直線式に Median->x を代入して Y を求める.
	if (Top->x > Buttom->x)
	{
		if (Mpos > Median->y)	return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
		else					return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
	} 
	else if (Top->x < Buttom->x)
	{
		if (Mpos >= Median->y)	return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
		else					return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
	}
	else
	{
		if (Median->x < Top->x)	return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
		else					return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
	}
}

//右向き三角形の各頂点別の傾きを求めます.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3Left(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median)
{
        /*      
        *       T
        *      /|
        *     / |
        *    /  | 
        *  M/   |  こんな感じの図形
        *   \   |			は、特別なルーチンが必要です(涙)
        *    \  |
        *     \ |
        *      \|
        *       B
        */
            //T -> M について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Median);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Buttom);
            //第一ストップポイント
            StopY[0] = Median->y;    //M の Y で一休み
            if (Median->y == Buttom->y)   return 0;   //M と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,0)],Median,Buttom);  //WaR = Median => Buttom まで
                //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第1ストップポイント
                StopY[0] = Buttom->y;     //B の Y で一休み
				return 0;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,0)],Median,Buttom);  //WaR = Median => Buttom まで
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
				return 1;
            }
}

//右向き三角形の各頂点別の傾きを求めます.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3Right(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median)
{
        /*      
        *   T
        *   |\
        *   | \
        *   |  \
        *   |   \M   こんな感じの図形
        *   |	/           は、特別なルーチンが必要です(涙)
        *   |  /
        *   | /
        *   |/
        *   B
        */
            //T -> M について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Median);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Buttom);
            //第一ストップポイント
            StopY[0] = Median->y;    //M の Y で一休み
            if (Median->y == Buttom->y)   return 0;   //M と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,1)],Median,Buttom);  //WaR = Median => Buttom まで
                //WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第1ストップポイント
                StopY[0] = Buttom->y;     //B の Y で一休み
				return 0;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,1)],Median,Buttom);  //WaR = Median => Buttom まで
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
				return 1;
            }
}

/*****************************************************************/
/*****************************************************************/
//統合
/*****************************************************************/
/*****************************************************************/

//Y 座標に対するクリッピングを実行します.
template<class _COMMWORKAREA>
bool YClipping(int &Y,int *StopY,_COMMWORKAREA * &lpWA,_COMMWORKAREA * &WAL,_COMMWORKAREA * &WAR,BMD const bmd2)
{
	register int Temp;
    int *lpStopY;
	//Yの転送開始アドレスが 0 未満だった処理 (超ややこしい(;;))
	for(lpStopY = &StopY[0] , DeltaXYTStart = 0 ; Y < 0; DeltaXYTStart ++)
	{
		if (*lpStopY >= 0)
		{	//0 以上になった.
			Temp = (- Y)  ;	// y の数だけ そのプラスの *lpStopY から食いちぎる.
			//各要素をプラスする.
            StepUp(WAL,WAR,Temp);
			Y = 0;
			break;
		}
		else
		{	//まだ 0 に到達しない場合....
			if (DeltaXYTStart == DeltaXYTMax) return false; //すべてマイナスに突っ込んでいる.
			Temp = (-Y) + *lpStopY ;	// 符号反転した ストップ Y の値分、各要素をプラスします.
			//各要素をプラスする.
            StepUp(WAL,WAR,Temp);
			Y = *lpStopY;
		}
        lpStopY++;  //次の停止場所へ
        lpWA+=2;
        WAL = lpWA;
	    WAR = (lpWA+1);
	}

    if (Y >= bmd2->h) return false;
    
    //Y座標のオーバー転送の防止
    for(lpStopY = &StopY[DeltaXYTStart] , Temp = DeltaXYTStart;Temp <= DeltaXYTMax;lpStopY++ , Temp++)
    {
        if(*lpStopY >= bmd2->h)
        {
            DeltaXYTMax = Temp;
            *lpStopY = (bmd2->h - 1);  //はい ここで.
            break; 
        }
    }
	return true;
}




/*****************************************************************/
//そもそも　かいがしたい図形なのか?
//その図形の本当の頂点数を返す.
/*****************************************************************/
template<class _COMMXYT>
int IsTarget(_COMMXYT *XY,int NumVec)
{
    int i,l,UseCount;
    int UseVec = NumVec;     //結局の頂点数
    _COMMXYT NewXY[4];

    UseCount = 0;
    for(i = 0; i < NumVec ;i ++)
    {
        for(l = i+1 ; l < NumVec ; l ++)
        {
            //自分以外の点とリーグ戦をする.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //自分と同じ.
                UseVec--;
                break;
            }
        }
        //その頂点は必要
        if(l>=NumVec)   NewXY[UseCount++] = XY[i];
    }
    if (UseVec == NumVec) return UseVec;

	_COMMXYT * lpNewXY = NewXY;

    //その図形を XY に書き戻す.
	switch(UseVec)
	{
	case 3:
		*XY++ = *lpNewXY++ ;
	case 2:
		*XY++ = *lpNewXY++ ;
	case 1:
		*XY++ = *lpNewXY++ ;
	}
 	//memcpy( XY , NewXY , sizeof(_COMMXYT) * UseVec );
    return UseVec;
	/*
    int i,l,UseCount;
    int UseVec = NumVec;     //結局の頂点数
    _COMMXYT NewXY[4];

    UseCount = 0;
    for(i = 0; i < NumVec ;i ++)
    {
        for(l = i+1 ; l < NumVec ; l ++)
        {
            //自分以外の点とリーグ戦をする.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //自分と同じ.
                UseVec--;
                break;
            }
        }
        //その頂点は必要
        if(l>=NumVec)   NewXY[UseCount++] = XY[i];
    }
    if (UseVec == NumVec) return UseVec;
    //その図形を XY に書き戻す.
    for(i = 0; i < UseVec ; i++)
    {
        XY[i] = NewXY[i];
    }
    return UseVec;
	*/
	/*
    int UseVec = NumVec;     //結局の頂点数
    for(int i = 0 ; i < UseVec ; i ++ )
    {
        for(int l = i+1 ; l < UseVec ; l ++ )
        {
            //自分以外の点とリーグ戦をする.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //自分と同じやつがあった. 自分はそいつ くりそつのそいつをみて
				//タイムパラドックスがおきたわけではないけど、消滅します.

				//メモリ移動 つまり自分のところがつぶれて、 その上のやつが落ちてくる.
				register int DecUseVec = UseVec-1;
				for( int n = i ; n < DecUseVec ; n++ )
				{
					XY[n] = XY[n+1];
				}
				//メモリを移動したことによる 諸トラブルの待避.
				i--;
                UseVec--;
                break;
            }
        }
    }
    return UseVec;
	*/
}


/*****************************************************************/
//二つの頂点間の Y にたいする傾きを求める.  ふつー
/*****************************************************************/
void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E)
{
    int DY ;
    Fixed X,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //とりあえず、　0　Div 回避
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepTX = TX / DY;
	    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//二つの頂点間の Y にたいする傾きを求める.  Z Buffer
/*****************************************************************/
void HokanLine(_WorkAreaZ *WA,const _XYZT *B,const _XYZT *E)
{
    int DY ;
    Fixed X,Z,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    Z  = IF(E->z - B->z);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //とりあえず、　0　Div 回避
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepZ  = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepZ  = Z  / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//二つの頂点間の Y にたいする傾きを求める.  グロー
/*****************************************************************/
void HokanLine(_WorkAreaG *WA,const _XYZCT *B,const _XYZCT *E)
{
    int DY ;
    Fixed X,Z,CR,CG,CB,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    Z  = IF(E->z - B->z);
    CR  = IF(E->cr - B->cr);
    CG  = IF(E->cg - B->cg);
    CB  = IF(E->cb - B->cb);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //とりあえず、　0　Div 回避
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepZ  = 0;
        WA->StepCR = 0;
        WA->StepCG = 0;
        WA->StepCB = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepZ  = Z  / DY;
    WA->StepCR = CR / DY;
    WA->StepCG = CG / DY;
    WA->StepCB = CB / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//各スタート地点の設定 ふつー
/*****************************************************************/
void StartSet(const _XYT *SL,const _XYT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}
/*****************************************************************/
//各スタート地点の設定 ZBuffer
/*****************************************************************/
void StartSet(const _XYZT *SL,const _XYZT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    Z1 = IF(SL->z);            Z2 = IF(SR->z);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}

/*****************************************************************/
//各スタート地点の設定 ぐろー
/*****************************************************************/
void StartSet(const _XYZCT *SL,const _XYZCT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    Z1 = IF(SL->z);            Z2 = IF(SR->z);
    CR1 = IF(SL->cr);          CR2 = IF(SR->cr);
    CG1 = IF(SL->cg);          CG2 = IF(SR->cg);
    CB1 = IF(SL->cb);          CB2 = IF(SR->cb);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}


/*****************************************************************/
//指定された Step だけ、各値を増分させる.  ふつー
/*****************************************************************/
void StepUp(const _WorkArea *WAL,const _WorkArea *WAR,int Step)
{
    X1 += WAL->StepX * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
	X2 += WAR->StepX * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
}

/*****************************************************************/
//指定された Step だけ、各値を増分させる.  ZBuffer
/*****************************************************************/
void StepUp(const _WorkAreaZ *WAL,const _WorkAreaZ *WAR,int Step)
{
    X1 += WAL->StepX * Step; Z1 += WAL->StepZ * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
	X2 += WAR->StepX * Step; Z2 += WAR->StepZ * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
}

/*****************************************************************/
//指定された Step だけ、各値を増分させる.  グロー
/*****************************************************************/
void StepUp(const _WorkAreaG *WAL,const _WorkAreaG *WAR,int Step)
{
    X1 += WAL->StepX * Step; Z1 += WAL->StepZ * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
    CR1+= WAL->StepCR * Step;CG1+= WAL->StepCG* Step; CB1 += WAL->StepCB * Step;
	X2 += WAR->StepX * Step; Z2 += WAR->StepZ * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
    CR2+= WAR->StepCR * Step;CG2+= WAR->StepCG* Step; CB2 += WAR->StepCB * Step;
}


_GRAPHICS_END   //namespace end

#endif
