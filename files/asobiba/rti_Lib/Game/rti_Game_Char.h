#include "StartWithExeFile.h"
#if defined(GAME_ANIME_LIFE) && defined(SCRIPTE_LIFE)

#ifndef ___GAME_CHARH
#define ___GAME_CHARH

#include <.\Game\rti_Game_AnimeBankForFile.h>
#include <.\Game\rti_Game_Scripte_Run.h>

class TGame_CharBase
{
public:
	int x,y,z;	//座標.
	int sx,sy,sz;	//移動量.
	TGame_CharBase(){x = y = z = sx = sy = sz = 0;};
    bool alive;
	int Tag;		//タグ
};

class TGame_CharAnime : public TGame_CharBase
{
	TAnimeEx AEx[8];	//アニメーションデータ
	int Animes;	//方向によるアニメパターン (1パターン、4方位、8方位 のいずれか.)
	int Muki;	//
public:
	TGame_CharAnime(){};
	//その方向のアニメデータをセット. no は、 GameAnimeBank から.
	void SetAnimeSrc(int muki,int no,int speed){ AEx[muki].Set( GameAnimeBankForFile.Get(no) , speed); };
    //現在のアニメデータを返す.
    TAnimeEx* GetAnime(){return &AEx[Muki];}
    //現在のアニメデータのアニメ構造体を返す.GetAnime() のパワーアップ版
    Anime_Src_struct* GetAnimeSrc(){ return AEx[Muki].Get(); };
};

class TScripeRunAreaRunPlus : public TScripeRunArea
{
public:
	void Run(TScripteRun* Sr){ while( ! Sr->Run(RunNumber,this) ); };
};


class TGameChar : public TGame_CharAnime , public TScripeRunAreaRunPlus
{
};

//タスクシステムを補助するクラス.
class TBasicTaskFriend
{
    bool alive;
};

//タスクシステム ～  なんか、ディ○クシステムみたいな名前で良いですね.
//       ----------
//       |        |
//       |        |     << こんなのが出てきて、マ×オとル×ージが
//       ----------        鬼ごっこするんだよね. くへっ
//           ||

//_T は、そのタスク処理をしたい敵や玉のクラス.
//_NUM は、その数(最大数です.)
//_T のクラスは、最低 2 つの事を満たしていなければなりません.
//        1 ,  bool alive;  と、いう public アクセスが可能なメンバを保持している.
//              または、 TBasicTaskFriend クラスを継承しているクラス.
//        2 ,  void Action(); と、いう　以下同文.
//          
template<class _T,int _NUM>

class TBasicTask
{
    friend class TBasicTaskFriend;  //こいつが持っている alive メンバに public アクセスできるように.
protected:
    _T  Task[_NUM];
public:
    TBasicTask(){};

    //初期化
    void clear()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            puf->alive = false;
        }
    };
    //タスクを実行させる.
    void dispach()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            if ( puf->alive )
            {
                puf->Action();
            }
        }
    };
    //破棄.
    void destroy(_T* puf)
    {
        puf->alive = false;
    };
    //登録
    _T* attach()
    {
        int i;
        _T *puf = &Task[0];
        for (i = 0 ; i < _NUM ; i++ , puf ++)
        {
            if ( puf->alive ) continue;
            //登録.
            puf->alive = true;
            return puf; //うーむ、アドレスは返したいないんだけどなぁ
        }
        return NULL;
    };
};


#endif

#endif      //LIFE END

