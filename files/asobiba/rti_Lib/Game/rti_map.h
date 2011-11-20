#include "StartWithExeFile.h"
#ifdef BASE_MAP

#ifndef ___MAPH
#define ___MAPH
/**************************************************************************************/
//マップ (このマップは円筒スクロールはしません (^^;; ) 
//円筒スクロールしてほしいときには、 この TMap クラスの上位クラス TMapRotate を使ってください
/**************************************************************************************/
#include <.\Graphics\rti_graphics.h>

//マップチップのサイズは 16 * 16限定(爆)
#define MAP_CHIP_SIZE		16
#define MAP_CHIP_POW_SIZE	4
#define MAP_FILENAME	"No%d.def"
#define MAP_HEDER "This is MapFile"

//画面関係
#define SCREENXSIZE (ScreenX)
#define SCREENYSIZE (ScreenY)
#define SCREENXSIZEHARF	(SCREENXSIZE>>1)
#define SCREENYSIZEHARF	(SCREENYSIZE>>1)
#define MAP_SCREEN_XSIZE (SCREENXSIZE >> MAP_CHIP_POW_SIZE) //一画面文
#define MAP_SCREEN_YSIZE (SCREENYSIZE >> MAP_CHIP_POW_SIZE) //一画面文
//マップの総ピクセル数
#define MAP_FULLX	(MapW << MAP_CHIP_POW_SIZE)
#define MAP_FULLY	(MapH << MAP_CHIP_POW_SIZE)

//チップ関係
#define MAP_CHIPNO_TO_MAP_X(no) ((no) % 20) //マップチップナンバーをチップパレット座標に変換
#define MAP_CHIPNO_TO_MAP_Y(no) ((no) / 20) //マップチップナンバーをチップパレット座標に変換
#define MAP_NOW_POINT(x,y) ((x) + (y) * MapW) //現在読み書きできるところ
//スクロールを止める場所
#define MAP_STOP_XSCROLL ((MapW-MAP_SCREEN_XSIZE-1)<<MAP_CHIP_POW_SIZE)
#define MAP_STOP_YSCROLL ((MapH-MAP_SCREEN_YSIZE-1)<<MAP_CHIP_POW_SIZE)
//遊びの部分
#define NOMOVEX (80)
#define NOMOVEY (64)
//そのチップが通れるかどうか調べる.
#define MAP_THIS_CHIP_THUR(Chip) ( (Chip) >=MAP_SCREEN_YSIZE && (Chip) <= MAP_SCREEN_YSIZE*4)

class TMap
{
	int MapNo;	//所持しているマップナンバー.
	char *ChipFileName;	//チップの絵のデータをもっているやつ.
	BMD	 Chip;
protected:
	int ScreenX,ScreenY;
	int Nowx,Nowy;		//現在の左上の座標.
	short MapW,MapH;		//マップの幅と高さ
	short *Map;			//マップデータ

	void MapDrawing(BMD bmd,int Dx,int Dy,int Sx,int Sy,int W,int H);		//マップ 描く
public:
	TMap(){ScreenX = ScreenY = Nowx =  Nowy = 0;};
	~TMap(){Destroy();};
	int GetNowx(){return Nowx;};
	int GetNowy(){return Nowy;};
	void SetScreen(int x,int y){ScreenX = x; ScreenY = y;};	//スクリーンサイズ定義
	virtual int GetScreenX(int x){return x - Nowx;};
	virtual int GetScreenY(int y){return y - Nowy;};

	bool Read(int no);	//マップロード
	void Destroy();		//マップデータの破棄
	void DrawMain(BMD bmd,int w,int h);					//マップを絵画します
	virtual void Draw(BMD bmd){TMap::DrawMain(bmd,SCREENXSIZE,SCREENYSIZE);};	//マップを絵画します
	virtual void MapMasterMove(short nx,short ny,int speed);//主人公移動などによるマップスクロール
	virtual void SetCeneter(short nx,short ny);				//指定したポイントを画面の中心にする.
	virtual bool If(short nx,short ny);			//そのポイントを通過できるかチェック
	bool Clipping(int x,int y,int w,int h);			//マップの外に飛び出さないように... 飛び出していたら true
	bool IamNotDisplay(short x,short y,short faj);	//私は表示されていないのかな? されてない true
};


#endif

#endif  //LIFE END

