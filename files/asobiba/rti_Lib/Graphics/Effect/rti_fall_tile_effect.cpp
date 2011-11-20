/*****************************************************************/
/*****************************************************************/
//　ぐすように現在の表示されているやつをこわしていく
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef FALLTILE_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_fall_tile_effect.h>

TFallTileEffect::TFallTileEffect()
{
    FallBlockBuf = NULL;
}

TFallTileEffect::~TFallTileEffect()
{
    Init();
}

void TFallTileEffect::Init()
{
    if (FallBlockBuf)  free(FallBlockBuf);
}

void TFallTileEffect::Set(BMD bmd,BMD _OutBmd,int sizex,int sizey,int speed,unsigned char kind,short xkankaku,short ykankaku)
{
    int t,wx,wy, falltime , falltimebackup;
    OutBmd = _OutBmd; //出力
    SpBuf = bmd;

	//タイルに切り裂いたときのサイズを求める
	TableSize = GraphicsCutTile
		(SpBuf,sizex,sizey,&MaxXSize,&MaxYSize);

    Init();
    FallBlockBuf = (struct _FallBlock*)
        malloc(TableSize * sizeof(struct _FallBlock) +1);
    wx = 0,wy = 0;
    //tはループカウンターに使い回す
    for(t = 0; t < TableSize ; t ++)
    {
        wx += sizex;
        wy += sizey;
        if (wx >= SpBuf->w) wx = 0;
        if (wy >= SpBuf->h) wy = 0;
        FallBlockBuf[t].x = wx;
        FallBlockBuf[t].y = wy;
    }

    int wspeedx,wspeedy;
	int halfx,halfy;

    if (kind & FALLTILE_X_HALF_SPEED) halfx = 2;
	else                              halfx = 1;
    if (kind & FALLTILE_Y_HALF_SPEED) halfy = 2;
	else                              halfy = 1;
    //移動方向
    if (kind & FALLTILE_X_UP)
        wspeedx = -speed /halfx;
    else if (kind & FALLTILE_X_DOWN)
        wspeedx = speed /halfx;
    else
        wspeedx = 0;

    if (kind & FALLTILE_Y_UP)
        wspeedy = -speed /halfy;
    else if (kind & FALLTILE_Y_DOWN)
        wspeedy = speed  /halfy;
    else
        wspeedy = 0;

    falltime = 0; //崩れ落ち出す時間
    struct _FallBlock* lpFallBlockBuf = FallBlockBuf;
    for(wy=0 ; wy < MaxYSize ; wy ++ )
    {
        falltimebackup = falltime ;
        for(wx=0 ; wx < MaxXSize ; wx ++ )
        {
            lpFallBlockBuf->dx = lpFallBlockBuf->x = wx * sizex;
            lpFallBlockBuf->dy = lpFallBlockBuf->y = wy * sizey;
            lpFallBlockBuf->alive = true;
            t = (lpFallBlockBuf->x + sizex) - SpBuf->w;
            if (t>0) lpFallBlockBuf->sizex = t;
            else     lpFallBlockBuf->sizex = sizex;
            t = (lpFallBlockBuf->y + sizey) - SpBuf->h;
            if (t>0) lpFallBlockBuf->sizey = t;
            else     lpFallBlockBuf->sizey = sizey;

            lpFallBlockBuf->speedx = wspeedx;
            lpFallBlockBuf->speedy = wspeedy;
            lpFallBlockBuf->start  = falltime;
            lpFallBlockBuf++;
            falltime+=xkankaku;
        }
        falltime = falltimebackup + ykankaku;
    }
    timecounter = 0;
    deedcount   = 0;
}


int TFallTileEffect::Draw(GraphicsProc Proc)
{
    short wx,wy;
    struct _FallBlock* lpFallBlockBuf = FallBlockBuf;
    for(wy=0 ; wy < MaxYSize ; wy ++ )
    {
        for(wx=0 ; wx < MaxXSize ; wx ++ )
        {
            if (timecounter >= lpFallBlockBuf->start && lpFallBlockBuf->alive)
            { //くずれるべくブロック
                lpFallBlockBuf->dx += (lpFallBlockBuf->speedx)  ; 
                lpFallBlockBuf->dy += (lpFallBlockBuf->speedy)  ; 
                lpFallBlockBuf->speedx *= 2; //速度を増やす
                lpFallBlockBuf->speedy *= 2;

                if (lpFallBlockBuf->dx < 0 || lpFallBlockBuf->dx >= SpBuf->w ||
                    lpFallBlockBuf->dy < 0 || lpFallBlockBuf->dy >= SpBuf->h )
                { //画面外にでたので殺す
                    lpFallBlockBuf->alive = false;
                    deedcount ++;  //死体カウント
                }

            }
//            Debug->DEBUG3("%d %d   %d",lpFallBlockBuf->x,lpFallBlockBuf->y,lpFallBlockBuf->start);
            lpFallBlockBuf++;
        }
    }

    for(wy=0 ; wy < MaxYSize ; wy ++ )
    {
        for(wx=0 ; wx < MaxXSize ; wx ++ )
        {
            lpFallBlockBuf--;
            if (lpFallBlockBuf->alive)
            {
                Proc(SpBuf,lpFallBlockBuf->x,lpFallBlockBuf->y,
                    lpFallBlockBuf->sizex,lpFallBlockBuf->sizey,
                    OutBmd,lpFallBlockBuf->dx,lpFallBlockBuf->dy);
            }
        }
    }

    timecounter ++;
    return deedcount - TableSize;  //0が終了
}

#endif  //LIFE END
