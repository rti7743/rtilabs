#include "StartWithExeFile.h"
#ifdef CD_LIFE

#ifndef ___CDH
#define ___CDH

#pragma comment(lib,"winmm.lib")

#define CD_MAX 99+1

class TCDPlayer
{
	void Init();
    void End();
    //  曲の再生 ( 時間指定 )
	void Play(int,int,int,int,int,int);
    //  曲のループ再生
	void Replay(void);

    // 再生状態
    bool PlayCD;
	bool LoopCD;
    // ループ再生フラグ
	bool LoopMode;

    // 一時停止状態での時間
	int PauseM;
	int PauseS;
	int PauseF;

    // 開始曲番号と終了曲番号
	int StartSongNo;
	int EndSongNo;

	// 曲の数
    int SongMax;

    // CD情報
    struct _CDInfo
    {
    	// 曲の開始時間
	    struct start
	    {
    		int m,s,f; // 分・秒・フレーム
	    }
	    Start;
		// 曲の時間
    	struct time
    	{
	    	int m,s,f; // 分・秒・フレーム
	    }
	    Time;
    }CDInfo[CD_MAX];

public:
    TCDPlayer(){Init();};
    ~TCDPlayer(){End();};

    //情報取得
	void GetInfo(void);
    //すべて再生
	void AllPlay();
    //指定したトラック再生
	void Play(int);
    //指定したトラックの間を再生
	void Play(int,int);
    //ストップ
	void Stop(void);
    //ホーズ
	void Pause(bool);
    //曲スキップ (指定した曲スキップする)
	void Skip(int);
    //早送り(秒)
	void Forward(int);
    //巻き戻し(秒)
	void Backward(int);
    //CD ドライブに　ブツがはいっているか?
	bool Ready(void);
    //CDドライブオープン
	void Open(void);
    //CDドライブクローズ
	void Close(void);
    //ループしない
	void NoLoop(void);
    //最大の局数
	int  MaxSong(void);
    //現在再生している曲
	int  NowSong(void);
    //正常に再生が終了したときに呼ばれる
    void MciSuccessfulEnd();
};
#endif

#endif  //LIFE END
