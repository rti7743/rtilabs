#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#ifndef __RING_MENU_LISTH
#define __RING_MENU_LISTH


#include <windows.h>
#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_anime_ex.h>

#define ROTATE_MENU_MAX 20

#define ROTATION_NONE  0x00
#define ROTATION_RIGHT 0x01
#define ROTATION_LEFT 0x02
#define ROTATION_OPEN 0x04
#define ROTATION_CLOSE 0x08

class TRingMenu
{
	struct Anime_Src_struct AS[ROTATE_MENU_MAX]; //データ
    Angle1024 Kankaku[ROTATE_MENU_MAX];
    Angle1024 Icon_Count;					//現在のアイコンの数
	char Selected; 								//現在選択しているところ
    short Icon_Kankaku;             //アイコンを配置する感覚
    short RotationSpeed;            //回転速度

	//オープンクローズの処理用
	short Distance;					//中心からの距離
	short Icon_HoseiDistance;		//中心からの距離補正
	short DistanceSpeed;			//速度だよん
    //回転アニメーション用
    Angle1024 Rotation;
    char OldSelected;               //回転を始める前に選んでいたヤツ
    short Icon_Hosei;               //アイコンを補正

	//アイコン座標の再計算.
	void CalsIconPoint();
    void NomalDraw(GraphicsProc Proc,int x,int y);      //通常絵画

	//特殊アイコン絵画ルーチンだぴょん
	void SpDraw(GraphicsProc Proc,int x,int y);
    void RotationDraw(GraphicsProc Proc,int x,int y);   //回転絵画
	void OpenCloseDraw(GraphicsProc Proc,int x,int y);	//オープンクローズ
public:
	TRingMenu();
	int GetSelect(){return Selected;};
	void SetSelect(char _Selected){Selected = _Selected;};
    void Set(short _Distance,short _RotationSpeed);
	int Add(struct Anime_Src_struct *AS);	//メニューに追加
	bool Del(short num);	//メニューから削除

	bool MenuRotate(bool LR);	//メニュー回転
	bool MenuOpenClose(bool Open);	//オープンクローズアニメーション
    void MenuClose(){Rotation = ROTATION_NONE;};        //メニュー終了 回転中消し去る.
    bool GetNowNone(){return Rotation == ROTATION_NONE;}; //何もない通常モードだったら true

	void Draw(GraphicsProc Proc,int x,int y)
    {
		switch(Rotation)
		{
		case ROTATION_NONE:
			NomalDraw(Proc,x,y);
			break;
		case ROTATION_RIGHT:
		case ROTATION_LEFT:
			RotationDraw(Proc,x,y);
			break;
		case ROTATION_OPEN:
		case ROTATION_CLOSE:
			OpenCloseDraw(Proc,x,y);	//オープンクローズ
			break;
		}
    }
};


#endif

#endif
