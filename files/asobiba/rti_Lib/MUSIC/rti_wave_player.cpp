#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

/***********************************************************/
//実際に Wave 再生を司るルーチン
//          Thank you C MAGAZIN ... 眠い.. ぐぷっ(ぉ
/***********************************************************/
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Music\rti_wave_player.h>
#include <.\Base\rti_debug.h>

//22.506kHZ の 8 ビットステレオの曲以外扱わない!!!
#define CHANNELS 2
#define SAMPLERATE 22050
#define SAMPLEBITS 8
//一つのデータのブロック長
#define ONE_BLOCK_BTYE	(CHANNELS * (SAMPLEBITS/8) ) //2
#define ONE_LINE_SIZE (char)	//一つのチャネルが必要とする型
#define BUF_LINE_SIZE (short)

#define POOL_WAVES 32
#define SOUND_OVER_MAX 100
#define WAVE_BUFFER_SIZE 30000
void TryProc(char *,MMRESULT rc);
/***********************************************************/
//コンストラクタ
/***********************************************************/
TWavePlayer::TWavePlayer()
{
    //フォーマット決定!!
    WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormat.nChannels = CHANNELS;
    WaveFormat.nSamplesPerSec = SAMPLERATE;
    WaveFormat.wBitsPerSample = SAMPLEBITS;
    WaveFormat.nBlockAlign = (WORD)((SAMPLEBITS/8) * CHANNELS);
    WaveFormat.nAvgBytesPerSec = (DWORD)SAMPLERATE * WaveFormat.nBlockAlign;
    WaveFormat.cbSize = sizeof(WAVEFORMATEX);
    //デバイスオープン    
    MMRESULT ret;
    ret = waveOutOpen(&hWaveOutHandle,WAVE_MAPPER,&WaveFormat,
                    (DWORD)Application->GethWnd(),0,CALLBACK_WINDOW);
    //デバドラをオープンできたかどうか記録しておきます.
    if (ret!=MMSYSERR_NOERROR) WavePlayOk = false;  //不可
    else                       WavePlayOk = true;   //成功!
    //ダブルバッファの作成
    for(int i=0;i<2;i++){
        FWaveHdr[i] = (WAVEHDR*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(WAVEHDR));
        FWaveHdr[i]->dwBufferLength = WAVE_BUFFER_SIZE;
        FWaveHdr[i]->lpData = (char*)HeapAlloc(GetProcessHeap(),0,FWaveHdr[i]->dwBufferLength+1);
    }
    Playing = false;
	BufUsing = 0;	//バッファにデータがない
    Pool = 0;
}

/***********************************************************/
//デストラクタ
/***********************************************************/
TWavePlayer::~TWavePlayer()
{
	//予約すべて取り消し
    WavePlayOk = false;
	All_Breaker();
    //ダブルバッファの解放
    for(int i=0;i<2;i++){
        TryProc( "~TWavePlayer",waveOutUnprepareHeader(hWaveOutHandle,FWaveHdr[i],sizeof(WAVEHDR)) );
        HeapFree(GetProcessHeap(),0,FWaveHdr[i]->lpData);
        HeapFree(GetProcessHeap(),0,FWaveHdr[i]);
    }
    //デバイスクローズ.
    waveOutClose(hWaveOutHandle);
}

/***********************************************************/
//そのデータを再生する.
/***********************************************************/
void TWavePlayer::Play(WVD wvd)
{
    //デバイスオープンに失敗していれば没
    if (!WavePlayOk) return ;

	if (PlayingList.size() < SOUND_OVER_MAX)
	{
		//曲リストに追加する.
		PlayingWaveList *lpPWL = new PlayingWaveList;
		lpPWL->wvd = wvd;
		lpPWL->Offset = 0;
		PlayingList.push_back(lpPWL);
//        DEBUG3("再生指令 %x 受理",(UINT)lpPWL );
        Pool ++; //プールの量を増やす
	}
}

/***********************************************************/
//演奏停止から演奏してね　にかわったときにはここだ.
/***********************************************************/
void TWavePlayer::FirstPlay()
{
//    DEBUG("バッファ ファーストを行います.");
    //プール解放
    Pool = 0;
    //再生中にする
    Playing = true;
    //バッファにロードします.

    TryProc("FirstPlay",waveOutPause(hWaveOutHandle));
    for(int i=0;i<2;i++)
    {
        WriteNext(FWaveHdr[i]);
    }
    TryProc("FirstPlay",waveOutRestart(hWaveOutHandle));
}

/***********************************************************/
//再生が完了したってこと. 
/***********************************************************/
void TWavePlayer::WaveSuccessfulEnd(WAVEHDR *hdr)
{
    //バッファにデータがないんですね.
    BufUsing--;
//    DEBUG3("バッファ %x 解放 %d",(UINT)hdr,BufUsing);
    //あいたバッファにデータを追加する.
    WriteNext(hdr);
}


/***********************************************************/
//次のデータ プリーズ
/***********************************************************/
void TWavePlayer::WriteNext(WAVEHDR *hdr)
{
    //後始末
    TryProc("WriteNext1",waveOutUnprepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
    //データロード
    if (WVDDataLoadBuf(hdr) > 0)
    {
        //バッファにデータ書き
//        TryProc("WriteNext3",waveOutWrite(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
        if (waveOutWrite(hWaveOutHandle,hdr,sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
        {
            DEBUG3("バッファ %x は異常なデータを受け取りました.",hdr);
            TryProc("WriteNext4",waveOutUnprepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
            return ; //無効なデータが送られてきたので無視.
        }
        //バッファにデータをロードしましたよ
        BufUsing++;
//        DEBUG3("バッファ %x 書き込み %d",(UINT)hdr,BufUsing);
    }
    else
    {
//        DEBUG3("バッファ %x 書き込みデータなし",(UINT)hdr);
        hdr->dwFlags = 0;
        hdr->reserved = 0;
        hdr->dwBufferLength = 0;
    }
}

/***********************************************************/
//予約されている データロード
/***********************************************************/
int TWavePlayer::WVDDataLoadBuf(WAVEHDR *hdr)
{
    int write_buffer_len;

    if (PlayingList.empty())
    {   //すべての予約を解消した.
//        DEBUG("再生するデータはありません.");
        return 0;
    }
    //準備
    TryProc("WVDDataLoadBuf",waveOutPrepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
    //データロード実行
    write_buffer_len = WVDDataLoadBuf_WaveAmp(hdr);
    //最後まで再生したデータの破棄
    WVDDataLoadBuf_EndPlayDel();
    return write_buffer_len;
}

/***********************************************************/
//予約されている データロードを実行
/***********************************************************/
int TWavePlayer::WVDDataLoadBuf_WaveAmp(WAVEHDR *hdr)
{
	int i;
	short WaveMixserBuf[WAVE_BUFFER_SIZE] ;
	char  WaveMixserDiv[WAVE_BUFFER_SIZE] ; 
    std::list<PlayingWaveList*>::iterator it;
	PlayingWaveList *lpPWL;
    DWORD len;
	DWORD MostLongest = 0;	//使用するバッファの長さ
    DWORD FSize ;//再生するデータのサイズ(テンポラリです.)
    //バッファクリア
	ZeroMemory(WaveMixserBuf,WAVE_BUFFER_SIZE * sizeof(short) );
	ZeroMemory(WaveMixserDiv,WAVE_BUFFER_SIZE * sizeof(char) );
    for(it = PlayingList.begin() ; it != PlayingList.end() ; it ++)
    {
		lpPWL = (*it);
        FSize = lpPWL->wvd->FSize; 
        if( lpPWL->Offset + WAVE_BUFFER_SIZE > FSize)
            len = FSize - lpPWL->Offset;
        else
            len = WAVE_BUFFER_SIZE;
		//バッファの最長変更
		if (MostLongest < len) MostLongest = len;
        //データを加えながら転送していきます.
		for(DWORD i=0; i < len ; i++)
		{
			WaveMixserBuf[i] += lpPWL->wvd->wv[lpPWL->Offset+i];
			WaveMixserDiv[i]++;
		}
        lpPWL->Offset += len;
//        DEBUG3("バッファ %x 再生指令 %x 実行",hdr,(UINT)(*it) );
    }
	
	//累計したデータの平均値をもとめます.
	for(i=0; i < MostLongest ; i++)
	{
		hdr->lpData[i] = (char) (WaveMixserBuf[i] / WaveMixserDiv[i]);
	}
	//出力するデータは最高の長さのやつです.
    hdr->dwBufferLength = MostLongest;
    return MostLongest;
}

/***********************************************************/
//予約されていたデータの中で最後まで再生したヤツを予約から削除
/***********************************************************/
void TWavePlayer::WVDDataLoadBuf_EndPlayDel()
{
    std::list<PlayingWaveList*>::iterator it_temp;
    std::list<PlayingWaveList*>::iterator it;
    //最後まで再生したデータの破棄
    for(it = PlayingList.begin() ; it != PlayingList.end() ;)
    {
        it_temp = it;
        it ++;
        if ( (*it_temp)->Offset == (*it_temp)->wvd->FSize) 
        {   //そのデータを再生する曲リストから抹消.
//            DEBUG3("再生指令 %x おしまい",(UINT)(*it_temp) );
            delete (*it_temp) ;
            PlayingList.erase(it_temp);
        }
    }
}


/***********************************************************/
//すべて破棄
/***********************************************************/
void TWavePlayer::All_Breaker()
{
    std::list<PlayingWaveList*>::iterator it;
    //ストップ
    TryProc("All",waveOutReset(hWaveOutHandle) );
    //すべてのリストにアクセスして解放する.
    for(it = PlayingList.begin() ; it != PlayingList.end() ; it ++)
    {
        delete (*it);
    }
    //リストのすべてのデータの破棄
    PlayingList.clear();
    //プール解放
    Pool = 0;
}

/***********************************************************/
//プールを強制排除して再生させる.
/***********************************************************/
void TWavePlayer::Ref()
{
    //デバイスオープンに失敗していれば没
    if (!WavePlayOk) return ;
    //バッファがゼロで、
    //プールされている値があるならば....
    if (Pool > 0 && BufUsing == 0)   FirstPlay();
}

void TryProc(char *p,MMRESULT rc)
{
    if(rc!= MMSYSERR_NOERROR){
        char msg[255];
        waveOutGetErrorText(rc,msg,sizeof(msg));
        DEBUG3("%s 内で　%s ",p,msg);
    }
}

#endif //LIFE END
