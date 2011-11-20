#include "StartWithExeFile.h"
#ifdef RING_MENU_LIST

#ifndef __HIGH_RING_MENUES_LISTH
#define __HIGH_RING_MENUES_LISTH

#include <vector>
#include <.\Game\rti_ring_menu.h>
#include <.\Base\rti_joystick.h>

class THighRingMenues;

//メニューが回転、終了したときに..
typedef void (*RingMenuEvent)
        (THighRingMenues* Menues,TRingMenu* NowMenu,int MenuKind);
//メニュー切り替えの前にチェンジ
typedef bool (*RingMenuChange)(int MenuKind);

class THighRingMenues
{
    bool NowMenuEnd;        //メニューを抜けるためにクローズしています.
    bool NowMenuClosing ;	//現在メニューを閉じる処理してます.
    bool NowMenuClosingBeforUd;	//メニューを閉じたら次のメニューは 上下のどっち?
    int MenuKind ;              //現在使用しているメニュー
    TJoyStick *JoyStick;        //ジョイスティック
    std::vector<TRingMenu*> RingMenuList; //リングメニューリスト.

    //回転など
    void MenuRotation(bool LR);
    //メニューの切り替え中だったら
    void MenuClosingProf();

    //メニューの切り替えを始める
    void MenuChange(bool Ud);
    //メニューの切り替えする.
    void MenuChangeDo();

    //メニューをしめる準備をする
    void MenuClose();
    //メニューをしめる
    void MenuCloseDo(TRingMenu *NowMenu);
public:
    //メニューが回転した時に呼ばれる
    RingMenuEvent MenuRotationEvent;
    //メニューが閉じたときに呼ばれる
    RingMenuEvent MenuCloseEvent;
    //メニューをチェンジしたときに呼ばれる.
    RingMenuChange MenuChangeEvent;
    //コンストラクタ
    THighRingMenues();
    //デストラクタ
    ~THighRingMenues();
    //追加 絵、一つのアイコンの幅と高さ　その絵に何個のアイコンがあるのか.
    TRingMenu* Add(BMD _bmd,int w,int h,int count);
    //メニュー達を開くのです!
    void Open();
    //基本処理
	void Draw(GraphicsProc Proc,int x,int y);
    //アクセス関数
    TRingMenu* GetMenu(int no);
	//ジョイスティック登録
	void SetJoyStick(TJoyStick *j){JoyStick = j;};
};

#endif

#endif		//LIFE END
