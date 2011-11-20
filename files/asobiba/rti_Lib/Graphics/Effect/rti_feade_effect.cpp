#include "StartWithExeFile.h"
#ifdef FEADE_LIFE

/*****************************************************************/
/*****************************************************************/
//指定した色に近づける
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_feade_effect.h>

/*****************************************************************/
//RGB は、 それぞれ 255 ～ -255 .  すべて255 にすると白.
//                                 すぺて-255 にすると黒.
/*****************************************************************/
TFeadeEffect::TFeadeEffect()
{
}

TFeadeEffect::~TFeadeEffect()
{
}

/*****************************************************************/
//RGB は、 それぞれ 255 ～ -255 .  すべて255 にすると白.
//                                 すぺて-255 にすると黒.
/*****************************************************************/
void TFeadeEffect::Set(BMD _OutBmd,int nr,int ng,int nb,int gr,int gg ,int gb,int fream)
{
    OutBmd = _OutBmd; //出力
    //移動したい色
    GoalR = gr;
    GoalG = gg;
    GoalB = gb;
    //初期色
    NowR  = nr;
    NowG  = ng;
    NowB  = nb;

    //速度をもとるめる
    SpeedR = (GoalR - NowR) / fream;
    SpeedG = (GoalG - NowG) / fream;
    SpeedB = (GoalB - NowB) / fream;

    //現在のフレーム数 0 になったら終了.
    NowFream = fream + 1;
}


/****************************************************************/
//目的の色に近づけるメイン 終わるんだったら false を返す
/****************************************************************/
bool TFeadeEffect::Draw()
{
    bool ret;
    //フィルタをかける
    FilterDraw(OutBmd,0,0,OutBmd->w,OutBmd->h,NowR,NowG,NowB);
    //フィルタを変形していきます.
    if (NowFream != 0)
    {//色のチェンジしていきます.
        NowR  += SpeedR;
        NowG  += SpeedG;
        NowB  += SpeedB;
        NowFream--;
        ret = true;
    }
    else
    { //終了　念のためゴールの色を代入しておく.
        NowR  = GoalR;
        NowG  = GoalG;
        NowB  = GoalB;
        ret = false;
    }
    return ret;
}

#endif //LIFE END
