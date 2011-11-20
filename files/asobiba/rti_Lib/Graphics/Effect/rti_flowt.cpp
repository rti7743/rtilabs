#include "StartWithExeFile.h"
#ifdef FLOWT_LIFE

/*****************************************************************/
/*****************************************************************/
//ぶつを降らせる
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_flowt.h>


TFlowtEffect::TFlowtEffect()
{
    FlowtChar = NULL;
}

TFlowtEffect::~TFlowtEffect()
{
    Init();
}

void TFlowtEffect::Init()
{
    if (FlowtChar) free(FlowtChar);
}

void TFlowtEffect::Set(BMD _bmd,BMD _OutBmd,int _onesizew,int _total,int _yspeed,int _xspeedmax,int _flow,int _areatparsent,int _zureparsent)
{
    OutBmd = _OutBmd; //出力
    bmd = _bmd;               //テクスチャ
    onesizew = _onesizew;      //一つのピースの横幅
    total = _total;           //ピースの数
    xspeedmax = _xspeedmax;   //縦の移動量の正最大値(実際にはこれの2倍移動する)
    yspeed = _yspeed;         //おちるスピード
    flow = _flow;             //一度に表示する数
    areatparsent = _areatparsent; //出現する確率
    zureparsent = _zureparsent;   //ずれを起こす確率

    Init();
    int size =flow * sizeof(struct _FlowtChar) +1;
    FlowtChar = (struct _FlowtChar*) malloc(size);
    ZeroMemory(FlowtChar,size);

}

void TFlowtEffect::Draw(GraphicsProc Proc,int x,int y,int w,int h)
{
    struct _FlowtChar* lpwFlowtChar = FlowtChar;
    int i;
    bool allredy_make = false;
    for(i = 0 ;i < flow ; i ++)
    {
        if (lpwFlowtChar->alive)
        {
            lpwFlowtChar->x += lpwFlowtChar->xspeed;
            if ( rand100(zureparsent) )
            {
                lpwFlowtChar->xspeed+=1;
                if (lpwFlowtChar->xspeed>=xspeedmax)
                    lpwFlowtChar->xspeed = -lpwFlowtChar->xspeed;
            }
            lpwFlowtChar->y += lpwFlowtChar->yspeed;
            if (lpwFlowtChar->y>=h)
            { //抹消
                lpwFlowtChar->alive = false;
            }
            else
            {
                Proc(bmd,lpwFlowtChar->wsize,
                    0,onesizew ,bmd->h,
                    OutBmd,x+lpwFlowtChar->x,y+lpwFlowtChar->y);
            }
        }
        else //空き発見
        {
            if ( rand100(areatparsent) && !allredy_make)
            { //登録
                lpwFlowtChar->alive = true;
                lpwFlowtChar->y = 0;
                lpwFlowtChar->x = randX(0,w);
                lpwFlowtChar->xspeed = randX(-xspeedmax,xspeedmax);
                lpwFlowtChar->yspeed = yspeed;
                lpwFlowtChar->wsize = onesizew * randX(0,total);
                allredy_make = true;
            }
        }
        lpwFlowtChar++;
    }
}


#endif //LIFE END
