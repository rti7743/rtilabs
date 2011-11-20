#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

#ifndef ___RTI_WAVE_PLAYER
#define ___RTI_WAVE_PLAYER


#include <.\Music\rti_wave.h>
#include <list>
#pragma warning(disable: 4786)
#pragma comment(lib,"winmm.lib")

struct PlayingWaveList
{
    WVD wvd;                //再生する曲データ
    unsigned long Offset;   //現在の再生したバイト数
};
class TWavePlayer
{
    HWAVEOUT    hWaveOutHandle;	// 出力デバイスのハンドルを取得する変数
    WAVEHDR     *FWaveHdr[2];    //演奏バッファのアドレス.(ダブルバッファ)
    WAVEFORMATEX    WaveFormat;  //

    std::list<PlayingWaveList*> PlayingList;
    bool        WavePlayOk;
    bool        Playing;    //現在再生中.
	unsigned char BufUsing;	//バッファを使っている(0だったらバッファは空)
    unsigned char Pool;     //プールしているウェーブの量

    //演奏停止から演奏してね　にかわったときにはここだ.
    void FirstPlay();
    //次のデータ プリーズ
    void WriteNext(WAVEHDR *hdr);
    //予約されている データロード
    int  WVDDataLoadBuf(WAVEHDR *hdr);
    //予約されている データロードを実行
    int WVDDataLoadBuf_WaveAmp(WAVEHDR *hdr);
    //予約されていたデータの中で最後まで再生したヤツを予約から削除
    void WVDDataLoadBuf_EndPlayDel();
public:
    TWavePlayer();
    ~TWavePlayer();
    //プールを強制排除して再生させる.
    void Ref();
    //そのデータを再生する.
    void Play(WVD wvd);
    //再生が完了したってこと. 
    void WaveSuccessfulEnd(WAVEHDR *hdr);
	//すべて破棄
	void All_Breaker();
};

#endif

#endif //LIFE END
