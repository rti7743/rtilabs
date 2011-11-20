#ifndef ___COMMH
#define ___COMMH
/**************************************************************/
//統合絵画ルーチン
// GDI と DirectDraw のインターフェースの違いを吸収する.
//　これにより、 GDI でも、 DirectDraw でも、動作するプログラムができます.
//  GDI の速度はとろい ? Non Non ご冗談を. あれは、へっぽこプログラマーが
//  作れば、とろいルーチンですが、ちゃんと作れば DirectDraw と比べても
//  遜色のないルーチンができます. つまり、科学の勝利です.
/**************************************************************/

#include <windows.h>
#include <.\rti_define.h>

//現在の絵画モード
#define DRAW_MODE_MAX	2
enum UseDrawMode{	USE_GDI_MODE	,USE_DIRECTDRAW_MODE};				//絵画モード
enum ScreenSize {	SCREEN_SIZE_NONE,SCREEN_FULL_OK ,SCREEN_WINDOW_OK};	//画面サイズ

struct _NowScreenMode
{
	bool fFullScreen;		//フルスクーリンだったら true
	unsigned int  iWidth,iHeight;	//スクリーンの幅と高さ.
	unsigned int  iBpp;				//色数
};

struct _CommDraw
{
	//初期化
	bool (*Init)(HWND hwnd,int w,int h,int bps);
    //終了処理
	bool (*End)(void);
    //指定されたファイルから BMP を読み込み
	bool (*GraphicsMake)(BMD bmd,const char* filename);
    //bmd->w bmd->h のサイズ分  メモリ確保
	bool (*GraphicsAlloc)(BMD bmd);
    //確保したメモリ解放
	bool (*GraphicsFree)(BMD bmd);
    //グラフィックスロック. これをすると、 bmd->bm から、画像データを読み出せる.
	bool (*GraphicsLock)(BMD bmd);
    //アンロック
	bool (*GraphicsUnLock)(BMD bmd);
    //HDC を得る.
	HDC (*GetDC)(BMD bmd);
    // HDC 削除
	bool (*DeleteDC)(BMD bmd,HDC hdc);
    // HDC 更新
	bool (*UpdataDC)(BMD bmd,HDC hdc);
    //現在の絵画モード取得
	UseDrawMode  (*GetMode)(void);
    //裏画面より、絵画
	void (*Ref)(HWND hwnd);
    //フルスクーリンにする
	bool (*DoFullScreen)(bool B = false);
    //ウィンドウモードにする.
	bool (*DoWindow)(bool B = false);
    //WM_MOVE により、ウィンドウされたウィンドの位置
	void (*SetDrawPoint)(int x,int y);
};

extern struct _NowScreenMode NowScreenMode;
extern const struct _CommDraw* CommDraw;

#endif
