#include "StartWithExeFile.h"
#ifdef ROTATE_MAP

#include <.\Game\rti_map_rotate.h>
#include <.\Graphics\rti_backsurface.h>

/**************************************************************/
//マップを絵画します(円筒スクロール)
/**************************************************************/
void TMapRotate::DrawMain(BMD bmd,int w,int h)
{
	int x,y;
    x = (MAP_FULLX - Nowx);
    y = (MAP_FULLY - Nowy);
    //上下、左右の貫通
  	if ( x < w && y < h )
    {
        //左上
		MapDrawing(bmd,0,0,   Nowx,Nowy,  x,y);
        //右上
		MapDrawing(bmd,x,0,   0,Nowy,  w - x,y);
        //左下
		MapDrawing(bmd,0,y,   Nowx,0,  x,h - y);
        //右下
		MapDrawing(bmd,x,y,   0,0,  w - x,h - y);
        return;
    }
    //左右の貫通
	if ( x < w)
	{
        //左
		MapDrawing(bmd,0,0,  Nowx,Nowy,  x,h);
        //右
		MapDrawing(bmd,x,0,  0,Nowy,  w - x,h);
        return;
	}
    //上下の貫通
    if ( y < h)
    {
        //上
		MapDrawing(bmd,0,0,   Nowx,Nowy,  w,y);
        //下
		MapDrawing(bmd,0,y,   Nowx,0,  w , h - y);
        return;
    }

    //通常絵画.
   	MapDrawing(bmd,0,0,Nowx,Nowy,w,h);
}

/**************************************************************/
//主人公移動などによるマップスクロール(円筒)
/**************************************************************/
void TMapRotate::MapMasterMove(short nx,short ny,int speed)
{
	short lx,ly;  //スクリーン座標に変換
	//スクリーン座標に変換
	lx = GetScreenX(nx);
	ly = GetScreenY(ny);
    //左右
	if (ly >=(SCREENYSIZE/2)+NOMOVEY)		Nowy +=speed;
	if (ly <=(SCREENYSIZE/2)-NOMOVEY)	    Nowy -=speed;
    //上下
    if (lx >=(SCREENXSIZE/2)+NOMOVEX)		Nowx +=speed;
    if (lx <=(SCREENXSIZE/2)-NOMOVEX)		Nowx -=speed;
	//補正
	MapHosei(&Nowx,&Nowy);
}
/**************************************************************/
//円筒スクロールの場合のキャラクタ位置の補正
/**************************************************************/
void TMapRotate::MapHosei(int *x,int *y)
{
	if (*x > MAP_FULLX) *x -= MAP_FULLX;
	if (*y > MAP_FULLY) *y -= MAP_FULLY;
    if (*x < 0) *x += MAP_FULLX;
    if (*y < 0) *y += MAP_FULLY;
}
/**************************************************************/
//円筒スクロールの場合のキャラクタ位置の補正(オーナーロードだぴょん)
/**************************************************************/
void TMapRotate::MapHosei(short *x,short *y)
{
	if (*x > MAP_FULLX) *x -= MAP_FULLX;
	if (*y > MAP_FULLY) *y -= MAP_FULLY;
    if (*x < 0) *x += MAP_FULLX;
    if (*y < 0) *y += MAP_FULLY;
}

/**************************************************************/
//指定したポイントを画面の中心にする.
/**************************************************************/
void TMapRotate::SetCeneter(short nx,short ny)
{
	Nowx = nx - SCREENXSIZEHARF;
	Nowy = ny - SCREENYSIZEHARF;
	//補正
	MapHosei(&Nowx , &Nowy);
}

/**************************************************************/
//そのポイントを通過できるかチェック
/**************************************************************/
bool TMapRotate::If(short nx,short ny)
{
	//ここで問題になるのが、
	//マップチップをまたぐ場合である.
	//とりあえず、そのすべてのまたぐチップと判定をとる.
	//もし、一つでも移動できないチップがまたがれると、没になる.
	short lx,ly;
	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//補正
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//補正
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//補正
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	MapHosei(&lx,&ly);//補正
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	return true;
}

#endif //LIFE END
