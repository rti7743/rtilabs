/*****************************************************************/
/*****************************************************************/
//MIDI
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef MIDI_LIFE

#include <windows.h>
#include <.\Music\rti_midi.h>
#include <.\Base\rti_debug.h>

TMidiUse::TMidiUse(MidiCallBackProcDef Proc)
{
    hMidiStream    = NULL;            //MIDIストリームデバイスハンドル
    uiDeviceID     = MIDI_MAPPER;     //MIDIストリームデバイスID
    bStream        = FALSE;           //ストリーム形式フラグ
            //(ストリーム形式 TRUE /  ノーマル形式 FALSE)
    bPlay          = FALSE;           //再生中
            //(再生中 TRUE /  停止 FALSE)
    bLoop          = FALSE;           //ループ
            //(ループする TRUE /  ループしない FALSE)
    dwNumBlocks    = 0;               //MDS演奏データブロック数
    dwCurrentBlock = 0;               //現在までで内部バッファへ出力したブロック数
    lpMidiHdr   = NULL;              //演奏用 MIDIHDR 構造体配列用メモリ
    lpMDSBuffer = NULL;              //MDSブロックデータ配列用メモリ
    MidiCallBackProc        = Proc;
}

BOOL TMidiUse::LoadMDS(LPSTR lpszFileName,BYTE **lpMDSData)
{
    HMMIO       hFile;
    MMRESULT    mmResult;
    MMCKINFO    mmParent,mmSubchunk;
    DWORD       dwFileSize,dwDataSize;

    hFile = mmioOpen(lpszFileName,NULL,MMIO_READ);
    if(hFile==NULL)
    {
        DEBUG("file not open as mmi");
        return FALSE;
    }
    mmParent.fccType = mmioFOURCC('M','I','D','S');
    mmResult = mmioDescend(hFile,(LPMMCKINFO)&mmParent,
        NULL,MMIO_FINDRIFF);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("ここだよーん");
        mmioClose(hFile,0);
        return FALSE;
    }
    dwFileSize = mmParent.cksize;
    mmSubchunk.ckid = mmioFOURCC('f','m','t',' ');
    mmResult = mmioDescend(hFile,&mmSubchunk,&mmParent,MMIO_FINDCHUNK);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("fmt fot find");
        mmioClose(hFile,0);
        return FALSE;
    }
    if(mmioRead(hFile,(char*)&MdsFormat,sizeof(MDSFORMAT))
                                            != sizeof(MDSFORMAT))
    {
        DEBUG("read faild");
        mmioClose(hFile,0);
        return FALSE;
    }
    mmioAscend(hFile,&mmSubchunk,0);
    mmSubchunk.ckid = mmioFOURCC('d','a','t','a');
    mmResult = mmioDescend(hFile,&mmSubchunk,&mmParent,MMIO_FINDCHUNK);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("data not find");
        mmioClose(hFile,0);
        return FALSE;
    }
    dwDataSize = mmSubchunk.cksize;
    if(mmioRead(hFile,(char*)&dwNumBlocks,sizeof(DWORD)) != sizeof(DWORD) )
    {
        DEBUG("block not read");
        mmioClose(hFile,0);
        return FALSE;
    }
    (*lpMDSData) = (BYTE*) GlobalAlloc(GPTR,dwDataSize);
    if(lpMDSData == NULL)
    {
        DEBUG("not alloc");
        mmioClose(hFile,0);
        return FALSE;
    }
    if(mmioRead(hFile,(char*)*lpMDSData,dwDataSize ) == -1)
    {
        DEBUG("not read");
        mmioClose(hFile,0);
        GlobalFree(lpMDSData);
        return FALSE;
    }

    mmioClose(hFile,0);
    return TRUE;
}

BOOL TMidiUse::Open(LPSTR lpszfileName)
{
//      MMRESULT    mmResult;
    BYTE        *lpMDSData;

    Close();        //念のため

    if(!LoadMDS(lpszfileName,&lpMDSData))
    {
        DEBUG("file not load");
        return FALSE;
    }

    //データバッファをストリーム形式にするかどうか設定する
    //ブロック数が16より多い場合ストリーム形式
    bStream = (dwNumBlocks >= 16) ? TRUE : FALSE;
    //ストリーム用データの用意(バッファサイズ ×　ブロック数)
    lpMDSBuffer = (LPMDSBUFFER) GlobalAlloc(GPTR , 
                        dwNumBlocks * sizeof(MDSBUFFER));
    lpMidiHdr  = (MIDIHDR*) GlobalAlloc(GPTR , 
                        dwNumBlocks * sizeof(MIDIHDR));
    for(DWORD i = 0; i < dwNumBlocks ; i++)
    {
        //ストリームバッファ情報設定(ブロックデータの演奏データのサイズを格納)
        lpMDSBuffer[i].dwSize = ((LPMDSBUFFERHEADER)lpMDSData)->dwBlockLength;
        //MDSBUFFER の lpDataにメモリを割り当てる
        lpMDSBuffer[i].lpData = (BYTE*)GlobalAlloc(GPTR,(MdsFormat.dwMaxBuffer));;
        //MDSブロックデータへのポインタ
        lpMidiHdr[i].lpData   = (char*) lpMDSBuffer[i].lpData;
        //MDSブロックデータの実サイズ
        lpMidiHdr[i].dwBufferLength = MdsFormat.dwMaxBuffer;
        //MDSブロックデータ内で実際に使用しているバイト数
        lpMidiHdr[i].dwBytesRecorded = lpMDSBuffer[i].dwSize;
        //ブロック番号を識別子として入れておく
        lpMidiHdr[i].dwUser    = i;
        lpMidiHdr[i].reserved  = 0;
        lpMidiHdr[i].dwFlags   = 0;
        lpMidiHdr[i].lpNext    = NULL;
        //ポインタを進行させ演奏データをブロックごとコピーする
        lpMDSData  += sizeof(MDSBUFFERHEADER);
        CopyMemory(lpMDSBuffer[i].lpData,lpMDSData,lpMDSBuffer[i].dwSize);
        //ポインタを次のブロックデータに進める
        lpMDSData  += lpMDSBuffer[i].dwSize;
    }
    //ファイルから読み込んだデータを解放する
    GlobalFree(lpMDSData);
    //MIDIストリームデバイスオープン
    if(!hMidiStream)
        midiStreamOpen(&hMidiStream,&uiDeviceID,1,
                (DWORD)MidiCallBackProc,(DWORD)0,CALLBACK_FUNCTION);
    //タイムプロパティ
    MIDIPROPTIMEDIV midiProp;
    midiProp.cbStruct  = sizeof(MIDIPROPTIMEDIV);
    midiProp.dwTimeDiv = MdsFormat.dwTimeFormat;
    midiStreamProperty( hMidiStream,(BYTE*)&midiProp,
        MIDIPROP_SET | MIDIPROP_TIMEDIV);
    return TRUE;
}

BOOL TMidiUse::Play(BOOL bLooping)
{
    //2重再生を防ぐ
    if(bPlay)
        return FALSE;
    //各再生用パラメータを初期化
    bPlay = TRUE;
    bLoop = bLooping;
    //再生開始時の内部バッファ(演奏キュー)への出力ブロックはゼロ
    dwCurrentBlock = 0;
    //ストリームバッファへデータをセットする.
    //ストリームバッファは 64K までなので、
    // 65535Bytes / 4096Bytes で、一度にストリームバッファに転送できる
    //ブロックは 最大16ブロック
    for(DWORD i = 0 ; i < 16 ; i++)
    {
        //ストリーム形式でデータを転送しない場合は
        //最大ブロック数を越えた時点で終了
        if( (!bStream) && (i>=dwNumBlocks) )
            break;
        //ストリームバッファへデータを転送するための準備
        midiOutPrepareHeader((HMIDIOUT)hMidiStream,
            &lpMidiHdr[i],sizeof(MIDIHDR));
        //ストリームデータを再生するためのキューを作成する
        midiStreamOut(hMidiStream,&lpMidiHdr[i],sizeof(MIDIHDR));
        //内部バッファ(演奏キュー)へ出力したブロック数をカウントする
        dwCurrentBlock++;
    }
    //再生開始
    midiStreamRestart(hMidiStream);
    return TRUE;
}


void TMidiUse::DefMidiProc(HMIDIIN hMidiIn,UINT message,
                 DWORD dwInstance,DWORD wParam,DWORD lParam)
{
    MIDIHDR *midiHdr;
    //wParam は再生が終了した MIDIHDR 構造体(ブロックデータ)への
    //ポインタが格納されている構造体
    midiHdr = (MIDIHDR*)wParam;

    switch(message)
    {
    case MOM_DONE:
        //バッファ追加またはループ再生
        //midiHdr->User は、再生が終了したブロック番号
        //が入っているので、バッファ追加関数に渡す
        if(bStream==TRUE)
            SetStreamNextBuffer(midiHdr->dwUser+1);
        else
            SetNormalNextBuffer(midiHdr->dwUser+1);
        break;
    }
}

BOOL TMidiUse::SetNormalNextBuffer(DWORD dwPlayedBlock)
{
    if(!bPlay)
        return FALSE;
    if(dwPlayedBlock>=dwNumBlocks)
    {
        if(bLoop)
        {
            //ループ再生
            for(DWORD i = 0 ; i < dwNumBlocks ; i++)
            {
                //ストリームデータ再設定
                midiOutPrepareHeader( (HMIDIOUT)hMidiStream,
                    &lpMidiHdr[i],sizeof(MIDIHDR) );
                //ストリームデータを再生のためのキューに設定する
                midiStreamOut( hMidiStream,&lpMidiHdr[i],sizeof(MIDIHDR));
            }
        }
        else
            bPlay = FALSE;
    }
    return TRUE;
}

BOOL TMidiUse::SetStreamNextBuffer(DWORD dwPlayedBlock)
{
    if(!bPlay)
        return FALSE;
    //ストリームバッファが半分(8バッファ)
    //再生終了したら新しくバッファ追加する
    if(!(dwPlayedBlock&0x07))
    {
        //次の8バッファを出力
        for(DWORD i=0; i<8 ; i ++)
        {
            //バッファを設定したブロック数が
            //最大ブロック数を越えたら以下を省略する
            if(dwCurrentBlock>=dwNumBlocks)
                break;
            //ストリームデータ準備
            midiOutPrepareHeader( (HMIDIOUT)hMidiStream,
                 &lpMidiHdr[dwCurrentBlock],sizeof(MIDIHDR) );
            //ストリームデータを再生のためのキューに設定する
            midiStreamOut( hMidiStream,
                &lpMidiHdr[dwCurrentBlock],sizeof(MIDIHDR));
            //再生済みブロックをカウント
            dwCurrentBlock++;
        }
    }
    //ループ再生
    if(dwPlayedBlock >= dwNumBlocks)
    {
        bPlay = FALSE;
        if(bLoop)
            Play(TRUE);
    }
    return TRUE;
}

BOOL TMidiUse::Close()
{
    if(!hMidiStream)
        return FALSE;
    //デバイスをクローズする前に念のため演奏を停止させる.
    Stop();
    //MIDIデバイスのすべてのチャンネルの再生の中止
    midiOutReset((HMIDIOUT)hMidiStream);
    for(DWORD i = 0; i < dwNumBlocks ; i++)
    {
        //MIDI ストリームデータ解除
        //メモリが指し示すブロックを解放する前に必ず行わなくてはならない
        midiOutUnprepareHeader( (HMIDIOUT)hMidiStream,
            &lpMidiHdr[i] , sizeof(MIDIHDR) );
    }
    //MIDIストリームクローズ
    midiStreamClose(hMidiStream);
    hMidiStream = NULL;

    //ブロックに割り当てていたメモリを解放
    if(lpMDSBuffer)
    {
        for(DWORD i = 0 ; i < dwNumBlocks ; i++)
        {
            GlobalFree(lpMDSBuffer[i].lpData);
        }
        GlobalFree(lpMDSBuffer);
        lpMDSBuffer = NULL;
    }
    GlobalFree(lpMidiHdr);
    return TRUE;
}

BOOL TMidiUse::Stop()
{
    bPlay = FALSE;
    bLoop = FALSE;
    if(!hMidiStream)
        return FALSE;
    //MIDIストリーム演奏停止
    midiStreamStop(hMidiStream);
    return TRUE;
}

TMidiUse::~TMidiUse()
{
    Close();
}


#endif //LIFE END
