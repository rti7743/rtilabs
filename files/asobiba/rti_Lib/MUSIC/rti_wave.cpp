#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE
/***********************************************************/
//ウェーブファイル読み込み
/***********************************************************/

#include <.\Music\rti_wave.h>
#include <.\Base\rti_error_message.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

TWaveLib *WaveChar;

/***********************************************************/
//コンストラクタ
/***********************************************************/
TWaveLib::TWaveLib()
{
}

/***********************************************************/
//デストラクタ
/***********************************************************/
TWaveLib::~TWaveLib()
{
    All_Breaker();
}

#define WAVE_FILE_READ_ERROR_NO_FILE 0
#define WAVE_FILE_READ_ERROR_NO_RIFF 1
#define WAVE_FILE_READ_ERROR_NO_WAVE 2
#define WAVE_FILE_READ_ERROR_NO_DATA 3
#define WAVE_FILE_READ_ERROR_NO_READ 4

/***********************************************************/
//そのファイルから Wave を読み込む
//注意: フォーマットは 
//22.506kHZ の 8 ビットステレオの曲以外扱わない!!!
/***********************************************************/
WVD TWaveLib::Make(char *filename)
{
    //  チャンクの構造体
    struct TChunk
    {
        char ID[4];     //チャンクの種類
        long Size;     //チャンクのサイズ(Windowsらしくリトルエンディアン)
    }chunk;
    char FormType[4];   
    WVD Wvd;
    int FChannels = 2;      //チャンネル
    int FSampleRate = 22050;    //サンプリングレーシ
    int FSampleBits = 8;
    char  *wv = NULL;           //データを入れるところ
    DWORD FSize = 0;         //データサイズ

    //エラーメッセージテーブル
    char *ErrorString[]=
    {
        {"ファイル %s は 存在しません."},
        {"ファイル %s は RIFF フォーマットではない."},
        {"ファイル %s は WAVE フォーマットではない."},
        {"ファイル %s は メモリ確保時、またはデータ読み込み時にエラーが発生しました."},
        {"ファイル %s は 指定されたサイズ %d バイト 読み込めませんでした."},
    };

    FILE *fp;
    fp = fopen(filename,"rb");
    if (fp==NULL) 
    {
        //エラーメッセージの表示
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_FILE],filename);
        goto FileError_Rtn;   //エラー処理
    }

    //データを読みます
    fread(&chunk,8,1,fp);
    //RIFFの文字を確認
    if (strncmp(chunk.ID,"RIFF",4)!=0)
    {
        //エラーメッセージの表示
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_RIFF],filename);
        goto Error_Rtn; //エラー処理
    }

    //データを読みます
    fread(FormType,4,1,fp);
    //WAVE の文字を確認する.
    if (strncmp(FormType,"WAVE",4)!=0)
    {
        //エラーメッセージの表示
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_WAVE],filename);
        goto Error_Rtn; //エラー処理
    }

    while( fread(&chunk,sizeof(TChunk),1,fp) == 1)
    {
        if(strncmp(chunk.ID,"fmt ",4)==0)
        {
            WAVEFORMATEX wf;
            fread(&wf,min(long(18),chunk.Size),1,fp);
            FChannels   = wf.nChannels;
            FSampleRate = wf.nSamplesPerSec;
            FSampleBits = wf.wBitsPerSample;
        }
        else
        {
            if(strncmp(chunk.ID,"data",4)==0)
            {
                //メモリ確保と読み込みでシュー
                wv    = (char*)malloc(chunk.Size);
                FSize = chunk.Size;
                if (fread(wv,1,FSize,fp) != FSize)
                {
                    //エラーメッセージの表示
                    MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_READ],filename,FSize);
                }

                break;
            }
            else fseek(fp,chunk.Size,1);
        }
    }
    if (wv == NULL || FSize == 0)
    {
        //エラーメッセージの表示
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_DATA],filename);
        goto FileError_Rtn;   //エラー処理
    }

    //実体があるようなので、それを受け止める側の作成
    Wvd = new WAVE_DATA_TABLE;  //メモリ確保
    Wvd->wv = wv;   //データですぅ.
    Wvd->FSize = FSize;   //データサイズなのだー
    WvdList.push_back(Wvd); //リストに追加.

    fclose(fp);
    return Wvd;
    //エラー処理ルーチン
Error_Rtn:
    if (wv!=NULL) free(wv);
    fclose(fp);
FileError_Rtn:
    return NULL;
}

/***********************************************************/
//すべて破棄
/***********************************************************/
void TWaveLib::All_Breaker()
{
    std::list<WVD>::iterator it;
    //すべてのリストにアクセスして解放する.
    for(it = WvdList.begin() ; it != WvdList.end() ; it ++)
    {
        free( (*it)->wv );
        delete (*it);
    }
    //リストのすべてのデータの破棄
    WvdList.clear();
}

/***********************************************************/
//破棄
/***********************************************************/
void TWaveLib::Breaker(WVD puf)
{
    std::list<WVD>::iterator it =
        std::find(WvdList.begin(),WvdList.end(),puf);
    if(it != WvdList.end())
    {
            free ( puf->wv );
        	delete puf;
            WvdList.erase(it);
    }
}


#endif //LIFE END