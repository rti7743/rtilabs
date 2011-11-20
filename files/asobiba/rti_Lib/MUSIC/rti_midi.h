#include "StartWithExeFile.h"
#ifdef MIDI_LIFE

#ifndef ___MIDIH
#define ___MIDIH
#include <windows.h>
#include <mmsystem.h>

//fmtチャンクの詳細を受け取るための構造体
typedef struct
{
    DWORD   dwTimeFormat;   //MIDI時間フォーマット(拍子分割　分解能)
    DWORD   dwMaxBuffer;    //ストリームバッファ1ブロックの最大サイズ
    DWORD   dwFlags;        //データフォーマット
}MDSFORMAT;

//分割された演奏データの詳細を抜き出すのに用いる
typedef struct
{
    DWORD   dwBlockStart;   //ブロックの再生時間
    DWORD   dwBlockLength;  //ブロックデータサイズ
}MDSBUFFERHEADER,*LPMDSBUFFERHEADER;

//一時的に分割された演奏データを保存しておくためのバッファ
//(分割されたブロック数必要)
typedef struct
{
    DWORD   dwSize;     //ブロックデータサイズ
    BYTE    *lpData;    //演奏データ
}MDSBUFFER,*LPMDSBUFFER;

typedef void (*MidiCallBackProcDef)(HMIDIIN,UINT,DWORD,DWORD,DWORD);

class TMidiUse
{
private:
    HMIDISTRM   hMidiStream;    //MIDIストリームデバイスハンドル
    UINT        uiDeviceID;     //MIDIストリームデバイスID
    BOOL        bStream;        //ストリーム形式フラグ
            //(ストリーム形式 TRUE /  ノーマル形式 FALSE)
    BOOL        bPlay;          //再生中
            //(再生中 TRUE /  停止 FALSE)
    BOOL        bLoop;          //ループ
            //(ループする TRUE /  ループしない FALSE)
    DWORD       dwNumBlocks;    //MDS演奏データブロック数
    DWORD       dwCurrentBlock; //現在までで内部バッファへ出力したブロック数
    MIDIHDR     *lpMidiHdr;     //演奏用 MIDIHDR 構造体配列用メモリ
    MDSBUFFER   *lpMDSBuffer;   //MDSブロックデータ配列用メモリ
    MDSFORMAT   MdsFormat;      //MDSデータフォーマット
    MidiCallBackProcDef MidiCallBackProc;

    BOOL LoadMDS(LPSTR lpszFileName,BYTE **lpMDSData);
    BOOL SetStreamNextBuffer(DWORD dwPlayedBlock);
    BOOL SetNormalNextBuffer(DWORD dwPlayedBlock);

public:
    TMidiUse(MidiCallBackProcDef Proc);
    ~TMidiUse();
    BOOL Open(LPSTR lpszfileName);
    BOOL Play(BOOL bLooping);
    BOOL Close();
    void DefMidiProc(HMIDIIN hMidiIn,UINT message,
                 DWORD dwInstance,DWORD wParam,DWORD lParam);
    BOOL Stop();

};
typedef TMidiUse* TMIDI;


#endif

#endif //LIFE END
