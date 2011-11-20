#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE

#ifndef ___GAME_ANIME_BANKH
#define ___GAME_ANIME_BANKH

#include <.\Graphics\rti_anime_ex.h>
#include <vector>

class TGameAnimeBank
{
    std::vector<TAnimeSrc*> AnimeBank;	//アニメデータのソース.
public:
	TGameAnimeBank(){};
	~TGameAnimeBank(){All_Breaker();};
	TAnimeSrc* New(int *MappingNo,Anime_Src_struct *a1,Anime_Src_struct *a2 ,
		Anime_Src_struct *a3 ,Anime_Src_struct *a4 ,Anime_Src_struct *a5);
	void Destroy(TAnimeSrc* puf);
	void Destroy(int no){Destroy(AnimeBank[no]); };
	void All_Breaker();
	//取得する.
	TAnimeSrc* Get(int no){return AnimeBank[no];};
};

#endif

#endif //LIFE END
