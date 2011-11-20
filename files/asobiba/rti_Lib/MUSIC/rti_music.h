#include "StartWithExeFile.h"
#ifdef MUSIC_LIFE

#ifndef ___MUSICH
#define ___MUSICH

#pragma comment(lib,"winmm.lib")

class TMusic
{
	bool PlayMusic; //現在再生中
	bool LoopMusic; //ループ再生中
	bool LoopMode;

    //初期化
	bool Init();
    //終了処理
    void End();
public:
    TMusic(){Init();};
    ~TMusic(){End();};
	void Play(char* FileName);
	void Replay();
	void Stop();
	void NoLoop();
    void MciSuccessfulEnd();    //再生が終了しましたっせ.

};


#endif

#endif //LIFE END


