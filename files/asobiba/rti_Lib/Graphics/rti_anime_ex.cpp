#include "StartWithExeFile.h"
#ifdef ANIME_LIFE

/*****************************************************************/
/*****************************************************************/
/*アニメーション処理									     	 */
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Graphics\rti_anime_ex.h>
/**********************************************************/
// アニメソース設定
/**********************************************************/
void TAnimeSrc::Set(Anime_Src_struct *a1,Anime_Src_struct *a2 ,
					Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,
					Anime_Src_struct *a5)
{
	int i;
	i = 0;
	while(1)
	{
		if (a1== NULL || a1->bmd == NULL) break;
		memcpy(&AS[i++],a1,sizeof(Anime_Src_struct) );
		if (a2== NULL || a2->bmd == NULL) break;
		memcpy(&AS[i++],a2,sizeof(Anime_Src_struct) );
		if (a3== NULL || a3->bmd == NULL) break;
		memcpy(&AS[i++],a3,sizeof(Anime_Src_struct) );
		if (a4== NULL || a4->bmd == NULL) break;
		memcpy(&AS[i++],a4,sizeof(Anime_Src_struct) );
		if (a5== NULL || a5->bmd == NULL) break;
		memcpy(&AS[i++],a5,sizeof(Anime_Src_struct) );
		break;
	}
	AnimeMax = i;
}

/**********************************************************/
// アニメソース 指定されたパターンをやる.
/**********************************************************/
void TAnimeSrc::Draw(GraphicsProc Proc,int x,int y,short Pattern)
{
	if (AnimeMax < Pattern) return;
	Anime_Src_struct *puf = &(AS[Pattern]) ;
    Proc(puf->bmd,puf->x,puf->y,puf->w,puf->h,Sprite,x,y);
}


/**********************************************************/
// アニメEx設定
/**********************************************************/
void TAnimeEx::Set(TAnimeSrc* _SrcData,short _speed)
{
	AnimePatternCounter = 0; //アニメパターンカウンターを初期化
	AnimeCounter = 0;        //アニメカウンター初期化
	AnimeSpeed = _speed;     //アニメスピード
	AnimeSrcData = _SrcData; //ソースデータ
}

/**********************************************************/
// アニメEx カウンターが指定スピードを超えたらアニメパーン ++
/**********************************************************/
bool TAnimeEx::AnimeStepUp()
{
	if (AnimeCounter >= AnimeSpeed)
	{
		AnimeCounter = 0;

		//アニメパターンを一つ動かす
		AnimePatternCounter++;
		//最大のパターンだったら最初に戻す
		if (AnimePatternCounter >= AnimeSrcData->GetAnimeMax() )
		{
			AnimePatternCounter = 0;
			return true;
		}
	}
	AnimeCounter ++;
	return false;
}

/**********************************************************/
// アニメEx 指定した位置に絵画
/**********************************************************/
bool TAnimeEx::Draw(GraphicsProc Proc,int x,int y)
{
	//現在のアニメパターンを絵画してもらう.
	AnimeSrcData->Draw(Proc,x,y,AnimePatternCounter);
	return AnimeStepUp();
}

/**********************************************************/
//　アニメデータの取得.
/**********************************************************/
Anime_Src_struct* TAnimeEx::Get()
{
    Anime_Src_struct* Anime_Src;
	//現在のアニメパターンを絵画してもらう.
	Anime_Src = AnimeSrcData->Get(AnimePatternCounter);
	AnimeStepUp();
    return Anime_Src;
}
#endif //LIFE END
