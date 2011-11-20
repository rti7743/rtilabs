#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#include <.\Game\rti_high_ring_menues.h>
#include <.\Base\rti_key.h>
/********************************************************/
//コンストラクタ
/********************************************************/
THighRingMenues::THighRingMenues()
{
    MenuKind = 0;
    MenuRotationEvent = NULL;
    MenuCloseEvent = NULL;
    MenuChangeEvent = NULL;

    JoyStick = NULL;

}

/********************************************************/
//デストラクタ
/********************************************************/
THighRingMenues::~THighRingMenues()
{
    std::vector<TRingMenu*>::iterator it;

    //すべてのリストにアクセスして解放する.
    for(it = RingMenuList.begin() ; it != RingMenuList.end() ; it ++)
    {
        delete (*it);
    }
    //リストのすべてのデータの破棄
    RingMenuList.clear();

}

/********************************************************/
//追加 絵、一つのアイコンの幅と高さ　その絵に何個のアイコンがあるのか.
/********************************************************/
TRingMenu* THighRingMenues::Add(BMD _bmd,int w,int h,int count)
{
	//リングメニュー作成
	struct Anime_Src_struct AS;
    TRingMenu *Trm = new TRingMenu;
    //リングメニュー追加
    RingMenuList.push_back(Trm);

	AS.bmd = _bmd;
	AS.y = 0;
	AS.w = w;
	AS.h = h;
	Trm->Set(80,6);
	for (int i = 0; i < count ; i++)
	{
		AS.x = (w+1)*i;
		Trm->Add(&AS);
	}
    return Trm;
}

/********************************************************/
//メニュー処理のときに最初に呼んでね(はーと)
/********************************************************/
void THighRingMenues::Open()
{
    NowMenuClosing = false;
    NowMenuEnd = false;

	//現在仕様しているメニューを取得
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	//オープン処理だよん
	NowMenu->MenuOpenClose(true);
}

/********************************************************/
//メニュー基本処理
/********************************************************/
void THighRingMenues::Draw(GraphicsProc Proc,int x,int y)
{
	//キー判定
    if (KeyPush(VK_LEFT) ) 		MenuRotation(false);
    if (KeyPush(VK_RIGHT) )		MenuRotation(true);
    if (KeyPushOne(VK_UP) ) 		MenuChange(true);
    if (KeyPushOne(VK_DOWN) )		MenuChange(false);
	if (KeyPushOne(0x5A) || KeyPushOne(0x58))   MenuClose();

	//ジョイスティックが使える
	if (JoyStick && JoyStick->PadAlive())
	{
		JoyStick->JoyStickRef();
		if (JoyStick->KeyGet() & JOY_POS_RIGHT)	MenuRotation(true);
		if (JoyStick->KeyGet() & JOY_POS_LEFT)		MenuRotation(false);
		if (JoyStick->KeyOnesGet() & JOY_POS_TOP)		MenuChange(true);
		if (JoyStick->KeyOnesGet() & JOY_POS_DOWN)		MenuChange(false);
		if (JoyStick->ButtonOnesGet() & (JOY_BTN_A | JOY_BTN_B) )  MenuClose();
	}

	//現在仕様しているメニューを取得
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	//絵画
	NowMenu->Draw(Proc,x,y);

    //メニューの終了処理をしているのならば...
	MenuClosingProf();
	
}


/********************************************************/
//回転など
/********************************************************/
void THighRingMenues::MenuRotation(bool LR)
{
	//現在仕様しているメニューを取得
	TRingMenu *NowMenu = RingMenuList[MenuKind];
    //回転命令発効
	NowMenu->MenuRotate(LR);
    //回転したというイベント
    if (MenuRotationEvent) MenuRotationEvent(this,NowMenu,MenuKind);
}

/********************************************************/
//メニューの切り替えを始める
/********************************************************/
void THighRingMenues::MenuChange(bool Ud)
{
	//現在使用しているメニューを取得
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	if (NowMenu->MenuOpenClose(false) )
	{	//現在のメニューのクローズ処理承認!!
		NowMenuClosing = true;
		//どっちに切り替えるのか保存
		NowMenuClosingBeforUd = Ud;
	}
}

/********************************************************/
//メニューの切り替え中だったら
/********************************************************/
void THighRingMenues::MenuClosingProf()
{
	if (NowMenuClosing)
	{	//現在のメニューのクローズ処理中です!
		//現在使用しているメニューを取得
		TRingMenu *NowMenu = RingMenuList[MenuKind];
		if (NowMenu->GetNowNone() )
		{	//終了処理が終了した.
			NowMenuClosing = false;
            if (NowMenuEnd)
            {   //メニューをもう閉じる場合
                MenuCloseDo(NowMenu);
            }
            else
            {   //実際に切り替える.
			    MenuChangeDo();
            }
		}
	}
}

/********************************************************/
//メニューの切り替えする.
/********************************************************/
void THighRingMenues::MenuChangeDo()
{
    int menu_count = RingMenuList.size();
    menu_count--;
    while(1)
    {
	    if (NowMenuClosingBeforUd) 
    	{
	    	MenuKind++;
		    if (MenuKind > menu_count) MenuKind = 0;
    	}
    	else
    	{
	    	MenuKind--;
		    if (MenuKind < 0) MenuKind = menu_count;
	    }
        if (!MenuChangeEvent) break;
        if (MenuChangeEvent(MenuKind) ) break;
    }
    //新規に切り替えたメニューに対してオープン処理をさせる.
    TRingMenu *NowMenu = RingMenuList[MenuKind];
    NowMenu->MenuOpenClose(true);
}

/********************************************************/
//メニューをしめる準備をする
/********************************************************/
void THighRingMenues::MenuClose()
{
	//現在使用しているメニューを取得
	TRingMenu *NowMenu = RingMenuList[MenuKind];
	if (NowMenu->MenuOpenClose(false) )
	{	//現在のメニューのクローズ処理承認!!
		NowMenuClosing = true;
        //メニューを閉じるという合図
		NowMenuEnd = true;
	}
}


/********************************************************/
//メニューをしめる
/********************************************************/
void THighRingMenues::MenuCloseDo(TRingMenu *NowMenu)
{
    //メニューさんに閉じるという合図を送る.
    NowMenu->MenuClose();
    //終了したというイベント
    if (MenuCloseEvent) MenuCloseEvent(this,NowMenu,MenuKind);
}


/********************************************************/
//指定されたメニューを取得
/********************************************************/
TRingMenu* THighRingMenues::GetMenu(int no)
{
    if (no < 0) return NULL;
    if (no > RingMenuList.size())   return NULL;
    return RingMenuList[no];
}


#endif		//LIFE END
