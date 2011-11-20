#include "StartWithExeFile.h"
#ifdef SAVE_MENU_LIST

#include <.\Game\rti_list_menu.h>
#include <.\Graphics\rti_backsurface.h>
/***************************************************************/
//リストメニュー
//DOOM や　FF などのセーブスロットみたいなヤツを作る
/***************************************************************/

/***************************************************************/
//コンストラクタ
/***************************************************************/
TListMenu::TListMenu()
{
    AllDel();
};

/***************************************************************/
//すべて削除
/***************************************************************/
void TListMenu::AllDel()
{
    Selected=0;
    Menu_Count=0;
    Fream  = NULL;
    Cursor = NULL;
}


/***************************************************************/
//セット
/***************************************************************/
void TListMenu::Set(BMD fream,BMD cursor)
{
    Fream   = fream;        //メニューの枠
    Cursor  = cursor;       //カーソル
}


/***************************************************************/
//追加
/***************************************************************/
int TListMenu::Add(BMD bmd)
{
	//追加不可能
	if (Menu_Count >= LIST_MENU_MAX) return -1;
	//保存
	Card[Menu_Count] = bmd;
	//メニューの数を増やす
	Menu_Count++;
	//追加したところを返す 0 ～
	return Menu_Count-1;
}

/***************************************************************/
//削除
/***************************************************************/
bool TListMenu::Del(short num)
{
	//そんなもんはない!!
	if (num >= Menu_Count) return false;
	//ずらす
	for(int i=0; i < (Menu_Count - num - 1) ; i ++)
		Card[i+num]=Card[i+num+1];
	//メニューの数を減らす
	Menu_Count--;
	//もし、現在選んでいるところが消滅した場合、ずらす
	if (Selected >= Menu_Count) Selected = 0;
	return true;
}

/***************************************************************/
//移動
/***************************************************************/
void TListMenu::MenuUd(bool ud)
{
	if (ud)
	{ // 上
		Selected--;
		//補正
		if (Selected < 0) Selected = Menu_Count-1;
	}
	else
	{ //下
		Selected++;
		//補正
		if (Selected >= Menu_Count) Selected = 0;
	}
}

/***************************************************************/
//絵画
/***************************************************************/
void TListMenu::Draw(GraphicsProc Proc,int x,int y)
{
    BMD TempBmd;
    //フレーム
    if (Fream)
        ::Draw(Fream,0,0,Fream->w,Fream->h,Sprite,x,y);
    int NowCardY = y;
    for (int i=0; i < Menu_Count ; i++)
    {
        TempBmd = Card[i];
        Proc(TempBmd,0,0,TempBmd->w,TempBmd->h,
            Sprite,x,NowCardY);
        if (i == Selected)
        {   //カーソル絵画
            if (Cursor)
               ::Draw(Cursor,0,0,Cursor->w,Cursor->h,Sprite,x,NowCardY);
        }
        NowCardY += TempBmd->h;
    }
}


#endif
