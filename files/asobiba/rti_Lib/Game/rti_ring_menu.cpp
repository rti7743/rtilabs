#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#include <.\Game\rti_ring_menu.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_glookup.h>
#include <.\Base\rti_debug.h>
/********************************************************/
//リングメニュー
/********************************************************/

/********************************************************/
//コンストラクタ
/********************************************************/
TRingMenu::TRingMenu()
{
	Icon_Count = 0;
	Selected   = 0;
    Distance   = 0;
    Rotation   = 0;
    RotationSpeed = 1;
}

/********************************************************/
//セット 中心からの距離をセット
/********************************************************/
void TRingMenu::Set(short _Distance,short _RotationSpeed)
{
	Distance = _Distance;
    RotationSpeed = _RotationSpeed;
}

/********************************************************/
//メニューに追加
/********************************************************/
int TRingMenu::Add(struct Anime_Src_struct *as)
{
	//追加不可能
	if (Icon_Count >= ROTATE_MENU_MAX) return -1;
	//保存
	memcpy(&AS[Icon_Count],as,sizeof(Anime_Src_struct) );
	//メニューの数を増やす
	Icon_Count++;
	//アイコン座標の再計算.
	CalsIconPoint();
	//追加したところを返す 0 ～
	return Icon_Count-1;
}

/********************************************************/
//削除
/********************************************************/
bool TRingMenu::Del(short num)
{
	//そんなもんはない!!
	if (num >= Icon_Count) return false;
	//ずらす
	for(int i=0; i < (Icon_Count - num - 1) ; i ++)
		memcpy(&AS[i+num],&AS[i+num+1],sizeof(Anime_Src_struct) );
	//メニューの数を減らす
	Icon_Count--;
	//もし、現在選んでいるところが消滅した場合、ずらす
	if (Selected >= Icon_Count) Selected = 0;
	//アイコン座標の再計算.
	CalsIconPoint();
	return true;
}

/********************************************************/
//アイコン座標の再計算.
/********************************************************/
void TRingMenu::CalsIconPoint()
{
	int i;          //カウンタ
	int seeta = 0;  //角度

    //アイコンの間隔.
	Icon_Kankaku = 256 / Icon_Count;

	for(i=0 ; i < Icon_Count ; i++)
	{
        Kankaku[i] = seeta;
		seeta += Icon_Kankaku;
	}
}

/********************************************************/
//回転
/********************************************************/
bool TRingMenu::MenuRotate(bool LR)
{
    if (Rotation != ROTATION_NONE) return false;
    //過去に選択されていたものを保存
    OldSelected = Selected;
	if (LR)
	{ // 右回転
        Rotation = ROTATION_RIGHT;
		Selected--;
		//補正
		if (Selected < 0) Selected = Icon_Count-1;
	}
	else
	{ //左回転
        Rotation = ROTATION_LEFT;
		Selected++;
		//補正
		if (Selected >= Icon_Count) Selected = 0;
	}
    Icon_Hosei = 0; //アイコン補正の初期化
	return true;
}

/********************************************************/
//オープンクローズ
/********************************************************/
bool TRingMenu::MenuOpenClose(bool Open)
{
	//何かほかにやっていたら無視
    if (Rotation != ROTATION_NONE) return false;
	//近づける速度だにょー
	DistanceSpeed = 40;
    //過去に選択されていたものを保存 (回転を加えるため)
	OldSelected = Selected;	//OpenClose ではほぼダミーです.
	if (Open)
	{ // オープン
        Rotation = ROTATION_OPEN;
		Icon_HoseiDistance = 300;
		//補正する位置は、回転終了したときに 0 になるようにします.
		Icon_Hosei = -(Icon_HoseiDistance / DistanceSpeed) * RotationSpeed;
	}
	else
	{ //クローズ
        Rotation = ROTATION_CLOSE;
		Icon_HoseiDistance = Distance;
	    Icon_Hosei = 0; //アイコン補正の初期化
	}
	return true;
}

/********************************************************/
//通常絵画
/********************************************************/
void TRingMenu::NomalDraw(GraphicsProc Proc,int x,int y)
{
    int i;
	Anime_Src_struct *TempAS = &AS[Selected];
 	Angle1024 *lpKankaku = &Kankaku[0];
    Angle1024 TempKankaku;

	//現在選択されているものを真ん中に表示
    Proc(TempAS->bmd,TempAS->x,TempAS->y,
		 TempAS->w,TempAS->h, Sprite, x,y);

	//絵画
	for (i = Selected ; i < Icon_Count ; i++)
	{
        TempKankaku = *lpKankaku;
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Distance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Distance,TempKankaku));
        //次のデータへ
		TempAS++;
		lpKankaku++;
	}
	TempAS = &AS[0];
	for (i = 0; i < Selected ; i ++)
	{
        TempKankaku = *lpKankaku;
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Distance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Distance,TempKankaku));
        //次のデータへ
		TempAS++;
		lpKankaku++;
	}
}

/********************************************************/
//回転絵画
/********************************************************/
void TRingMenu::RotationDraw(GraphicsProc Proc,int x,int y)
{
    //表示角度移動
    switch(Rotation)
    {
    case ROTATION_RIGHT:    //右回転
        //角度移動
        Icon_Hosei += RotationSpeed;
        //補正をやりすぎないようにする (^^;
        if (Icon_Hosei > Icon_Kankaku)
        {
            Icon_Hosei = Icon_Kankaku;
            Rotation = ROTATION_NONE; //回転の終了
        }
        break;
    case ROTATION_LEFT:     //左回転
        //角度移動
        Icon_Hosei -= RotationSpeed;
        //補正をやりすぎないようにする (^^;
        if (Icon_Hosei < -Icon_Kankaku) 
        {
            Icon_Hosei = -Icon_Kankaku;
            Rotation = ROTATION_NONE; //回転の終了
        }
        break;
    }
	//絵画ルーチンを呼ぶぴょん
	SpDraw(Proc,x,y);
}

/********************************************************/
//オープンクローズ
/********************************************************/
void TRingMenu::OpenCloseDraw(GraphicsProc Proc,int x,int y)
{
	//アイコンを回転させて格好をつける
    Icon_Hosei += RotationSpeed;
    //表示角度移動
    switch(Rotation)
    {
    case ROTATION_OPEN:    //モード
        //角度移動
        Icon_HoseiDistance -= DistanceSpeed;
        //補正をやりすぎないようにする (^^;
        if (Icon_HoseiDistance < Distance)
        {
            Icon_HoseiDistance = Distance;
			Icon_Hosei = 0;
            Rotation = ROTATION_NONE; //オープンの終了
        }
        break;
    case ROTATION_CLOSE:     //モード
        //角度移動
        Icon_HoseiDistance += DistanceSpeed;
        //補正をやりすぎないようにする (^^;
        if (Icon_HoseiDistance > 300) 
        {
            Rotation = ROTATION_NONE; //回転の終了
        }
        break;
    }
	//絵画ルーチンを呼ぶぴょん
	SpDraw(Proc,x,y);
}

/********************************************************/
//特殊アイコン絵画ルーチンだぴょん
/********************************************************/
void TRingMenu::SpDraw(GraphicsProc Proc,int x,int y)
{
    int i;
	Anime_Src_struct *TempAS = &AS[OldSelected];
 	Angle1024 *lpKankaku = &Kankaku[0];
    Angle1024 TempKankaku;

	//絵画
	for (i = OldSelected ; i < Icon_Count ; i++)
	{
        //バイトの限界で座標系ループを実現する
        TempKankaku = (Angle1024) (*lpKankaku + Icon_Hosei);
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Icon_HoseiDistance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Icon_HoseiDistance,TempKankaku));
        //次のデータへ
		TempAS++;
		lpKankaku++;
	}
	TempAS = &AS[0];
	for (i = 0; i < OldSelected ; i ++)
	{
        //バイトの限界で座標系ループを実現する
        TempKankaku = (unsigned char ) (*lpKankaku + Icon_Hosei);
	    Proc(TempAS->bmd,TempAS->x,TempAS->y,
			TempAS->w,TempAS->h,
			Sprite,
			x+SIMPLE_ROTATEX1024(Icon_HoseiDistance,TempKankaku),
            y+SIMPLE_ROTATEY1024(Icon_HoseiDistance,TempKankaku));
        //次のデータへ
		TempAS++;
		lpKankaku++;
	}
}

#endif

