#include "StartWithExeFile.h"
#ifdef GEAJI_LIFE
#include <.\Game\rti_geaji.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_backsurface.h>
/*****************************************************************/
//ゲージルーチン
/*****************************************************************/

/*****************************************************************/
//ゲージの初期設定
/*****************************************************************/
void TGeaji::Set(BMD pbmd,int px,int py ,int pw,int ph,int pmax,int pmin,int _GeajiSpeed)
{
    bmd = pbmd; //ゲージの絵
    bmdx = px ; //その絵をスタートさせる X
    bmdy = py ; //その絵をスタートさせる Y
    bmdw = pw ; //その絵の幅
    bmdh = ph ; //その絵の高さ

    max = pmax; //ゲージの最大値
    min = pmin; //ゲージの最小値
    distance = (pmax - pmin); //距離
    draw_width = 0; 
    true_width = 0;
    GeajiSpeed = _GeajiSpeed; //ゲージのスピード
    value = pmin;
}

/*****************************************************************/
//最大値を変更し直す
/*****************************************************************/
void TGeaji::ReSetMax(int pmax)
{
    max = pmax;
    distance = (max - min); //距離

    //念のためもとめ直す
    draw_width = bmdw * value / distance;
    true_width = draw_width;
}

/*****************************************************************/
//指定した値にゲージを変更する
/*****************************************************************/
void TGeaji::NowValue(int pvalue)
{
    //簡易クリッピング
    if (pvalue > max) pvalue = max;
    if (pvalue < min) pvalue = min;
    //ゲージの最大 : distance = X : value の比率を解く
    draw_width = bmdw * pvalue / distance;
    true_width = draw_width;
    //ゲージの値を変更
    value = pvalue;
}

/*****************************************************************/
//指定した値にゲージを少しずつ移動させて変更する
/*****************************************************************/
void TGeaji::AfterValue(int pvalue)
{
    //簡易クリッピング
    if (pvalue > max) pvalue = max;
    if (pvalue < min) pvalue = min;
    //ゲージの最大 : distance = X : value の比率を解く
    true_width = bmdw * pvalue / distance;
    //ゲージの値を変更
    value = pvalue;
}


/*****************************************************************/
//ゲージの絵画
/*****************************************************************/
void TGeaji::Draw(GraphicsProc Proc,int x,int y)
{
    Proc(bmd,bmdx,bmdy,draw_width,bmdh,Sprite,x,y);
    //増減していくゲージの場合は.......
    if (draw_width != true_width)
    {
        if(draw_width < true_width)
        {
            draw_width+=GeajiSpeed;
            if (draw_width > true_width) draw_width = true_width; //補正
        }
        else
        {
            draw_width-=GeajiSpeed;
            if (draw_width < true_width) draw_width = true_width; //補正
        }
    }

}

#endif
