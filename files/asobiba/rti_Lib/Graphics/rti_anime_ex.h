#include "StartWithExeFile.h"
#ifdef ANIME_LIFE

#ifndef ___ANIMEH
#define ___ANIMEH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>

/*****************************************************************/
/*****************************************************************/
/*アニメーション処理									     	 */
/*****************************************************************/
/*****************************************************************/
#include "rti_graphics.h"
#include "rti_bitmaplib.h"

struct Anime_Src_struct;
struct Anime_Src_struct
{
	BMD bmd;
	short x;
	short y;
	short w;
	short h;
};

//アニメの基本となるデータをつかさどるところ 
class TAnimeSrc
{
private:
	short AnimeMax;         //アニメのマックス
	Anime_Src_struct AS[6];
public:
    TAnimeSrc(){AnimeMax=0;};
	//アニメの最大値を取得
	short GetAnimeMax(){return AnimeMax;};
	//データセット
	void Set(Anime_Src_struct *a1,Anime_Src_struct *a2 ,
		Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,Anime_Src_struct *a5);
	//絵画命令
	void Draw(GraphicsProc Proc,int x,int y,short Pattern);
    //パターンのデータの取得
    Anime_Src_struct* Get(int Pattern){return &AS[Pattern];};
};

//アニメーションさせるためのタイマーをつかさどる.
class TAnimeEx
{
private:
	short AnimePatternCounter ;
	short AnimeCounter ;
	short AnimeSpeed ;

	TAnimeSrc* AnimeSrcData;
	// アニメEx カウンターが指定スピードを超えたらアニメパーン ++
	bool AnimeStepUp();
public:
	TAnimeEx(){};
	// アニメEx設定
	void Set(TAnimeSrc* _SrcData,short _speed);
	//カウンターリセット
	void ReSetCounter(){AnimePatternCounter = AnimeCounter = 0;};
	// アニメEx 指定した位置に絵画
	bool Draw(GraphicsProc Proc,int x,int y);
    //　アニメデータの取得.
    Anime_Src_struct* Get();
};

#endif

#endif //LIFE END
