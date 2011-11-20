#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE


#include <.\Game\rti_Game_AnimeBank.h>
#include <stdio.h>
#include <algorithm>

TGameAnimeBank GameAnimeBank;

/*******************************************************************/
//新規作成
/*******************************************************************/
TAnimeSrc* TGameAnimeBank::New(int *MappingNo,Anime_Src_struct *a1,Anime_Src_struct *a2 ,
		Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,Anime_Src_struct *a5)
{
	//アニメソース作成
	TAnimeSrc* AS = new TAnimeSrc();
	//ソースデータセット
	AS->Set(a1,a2,a3,a4,a5);
	//登録
	AnimeBank.push_back(AS);
    if (MappingNo != NULL) *MappingNo = AnimeBank.size()-1;

	return AS;
}

/*******************************************************************/
//削除
/*******************************************************************/
void TGameAnimeBank::Destroy(TAnimeSrc* puf)
{
    std::vector<TAnimeSrc*>::iterator it =
        std::find(AnimeBank.begin(),AnimeBank.end(),puf);
    if(it != AnimeBank.end())
    {
        	delete puf;
            AnimeBank.erase(it);
    }
}

/*******************************************************************/
//全て削除
/*******************************************************************/
void TGameAnimeBank::All_Breaker()
{
    std::vector<TAnimeSrc*>::iterator it;

    //すべてのリストにアクセスして解放する.
    for(it = AnimeBank.begin() ; it != AnimeBank.end() ; it ++)
    {
        delete (*it);
    }
    //リストのすべてのデータの破棄
    AnimeBank.clear();
}


#endif //LIFE END
