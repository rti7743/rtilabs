#include "StartWithExeFile.h"
#ifdef CD_LIFE
/*******************************************************************/
/*
    Audio CD 再生ルーチン for MCI

    Thank you For EL (Eazy Link Library)    Botchy

    再構築 RTI

*/
/*******************************************************************/

#include <.\Music\rti_cd.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <windows.h>

/*******************************************************************/
//初期化
/*******************************************************************/
void TCDPlayer::Init()
{
    //曲別情報構造体初期化
    ZeroMemory(CDInfo,sizeof(struct _CDInfo)*CD_MAX);

    //停止時間の初期化
	PauseM=-1;
	PauseS=-1;
	PauseF=-1;
    //曲数
	SongMax=0;

    //最初の曲と最後の曲
	StartSongNo=0;
	EndSongNo=0;

    //状態の初期化
	PlayCD  =false;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//終了処理
/*******************************************************************/
void TCDPlayer::End()
{
    Stop();
}


/*******************************************************************/
//CD情報の取得
/*******************************************************************/
void TCDPlayer::GetInfo()
{
	int i;
	char Buffer[64];
    // MCI文字列バッファ
    char MciBuffer[32];

	// CDの使用開始
	mciSendString("open cdaudio",NULL,0,NULL);

	// 曲数の取得
	mciSendString("status cdaudio number of tracks",MciBuffer,32,NULL);

	SongMax=atoi(MciBuffer);

	// 全体の時間の取得
	mciSendString("status cdaudio length",MciBuffer,32,NULL);

	Buffer[0]=MciBuffer[0];
	Buffer[1]=MciBuffer[1];
	Buffer[2]=NULL;

	CDInfo[0].Time.m=atoi(Buffer);

	Buffer[0]=MciBuffer[3];
	Buffer[1]=MciBuffer[4];
	Buffer[2]=NULL;

	CDInfo[0].Time.s=atoi(Buffer);

	Buffer[0]=MciBuffer[6];
	Buffer[1]=MciBuffer[7];
	Buffer[2]=NULL;

	CDInfo[0].Time.f=atoi(Buffer);

	for (i=1;i<=SongMax;i++)
	{
		// 曲の開始時間の取得
		wsprintf(Buffer,"status cdaudio position track %d",i);

		mciSendString(Buffer,MciBuffer,32,NULL);

		Buffer[0]=MciBuffer[0];
		Buffer[1]=MciBuffer[1];
		Buffer[2]=NULL;

		CDInfo[i].Start.m=atoi(Buffer);

		Buffer[0]=MciBuffer[3];
		Buffer[1]=MciBuffer[4];
		Buffer[2]=NULL;

		CDInfo[i].Start.s=atoi(Buffer);

		Buffer[0]=MciBuffer[6];
		Buffer[1]=MciBuffer[7];
		Buffer[2]=NULL;

		CDInfo[i].Start.f=atoi(Buffer);

		// 曲の時間の取得
		sprintf(Buffer,"status cdaudio length track %d",i);

		mciSendString(Buffer,MciBuffer,32,NULL);

		Buffer[0]=MciBuffer[0];
		Buffer[1]=MciBuffer[1];
		Buffer[2]=NULL;

		CDInfo[i].Time.m=atoi(Buffer);

		Buffer[0]=MciBuffer[3];
		Buffer[1]=MciBuffer[4];
		Buffer[2]=NULL;

		CDInfo[i].Time.s=atoi(Buffer);

		Buffer[0]=MciBuffer[6];
		Buffer[1]=MciBuffer[7];
		Buffer[2]=NULL;

		CDInfo[i].Time.f=atoi(Buffer);
	}

	// CDの使用終了
	mciSendString("close cdaudio",NULL,0,NULL);
}

/*******************************************************************/
//曲の再生 ( 全曲 )
/*******************************************************************/
void TCDPlayer::AllPlay()
{
    //再生中だったら黙らせる.
	if (PlayCD) Stop();

	// CDの使用開始
	mciSendString("open cdaudio",NULL,0,NULL);

	// 曲の再生
	mciSendString("play cdaudio notify",NULL,0,Application->GethWnd() );

	StartSongNo=1;
	EndSongNo=SongMax;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//  曲の再生 ( １曲指定 )
/*******************************************************************/
void TCDPlayer::Play(int SongNo)
{
	char Buffer[64];

    //再生中だったら黙らせる.
	if (PlayCD) Stop();

	// CDの使用開始
	mciSendString("open cdaudio",NULL,0,NULL);

	if (SongNo<SongMax)
	{
		wsprintf(Buffer,
				"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
				CDInfo[SongNo].Start.m,CDInfo[SongNo].Start.s,
				CDInfo[SongNo].Start.f,CDInfo[SongNo+1].Start.m,
				CDInfo[SongNo+1].Start.s,CDInfo[SongNo+1].Start.f);
	}
	else
	{
		wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d notify",
				CDInfo[SongNo].Start.m,CDInfo[SongNo].Start.s,
				CDInfo[SongNo].Start.f);
	}

	// 曲の再生
	mciSendString(Buffer,NULL,0,Application->GethWnd() );

	StartSongNo=SongNo;
	EndSongNo=SongNo;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//曲の再生 ( 範囲指定 )
/*******************************************************************/
void TCDPlayer::Play(int SongNo1,int SongNo2)
{
	char Buffer[64];

    //再生中だったら黙らせる.
	if (PlayCD) Stop();

	// CDの使用開始
	mciSendString("open cdaudio",NULL,0,NULL);

	if (SongNo2<SongMax)
	{
		wsprintf(Buffer,
				"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
				CDInfo[SongNo1].Start.m,CDInfo[SongNo1].Start.s,
				CDInfo[SongNo1].Start.f,CDInfo[SongNo2+1].Start.m,
				CDInfo[SongNo2+1].Start.s,CDInfo[SongNo2+1].Start.f);
	}
	else
	{
		wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d notify",
				CDInfo[SongNo1].Start.m,CDInfo[SongNo1].Start.s,
				CDInfo[SongNo1].Start.f);
	}

	// 曲の再生
	mciSendString(Buffer,NULL,0,Application->GethWnd() );

	StartSongNo=SongNo1;
	EndSongNo=SongNo2;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
/*-  曲の再生 ( 時間指定 )                                      ※ 内部で使用  -*/
/*-                                                                            -*/
/*-    int StartM : 開始時間 ( 分 )                                            -*/
/*-    int StartS : 開始時間 ( 秒 )                                            -*/
/*-    int StartF : 開始時間 ( フレーム )                                      -*/
/*-    int EndM   : 終了時間 ( 分 )                                            -*/
/*-    int EndS   : 終了時間 ( 秒 )                                            -*/
/*-    int EndF   : 終了時間 ( フレーム )                                      -*/
/*******************************************************************/
void TCDPlayer::Play(int StartM,int StartS,int StartF,int EndM,int EndS,int EndF)
{
	char Buffer[64];

    //再生中だったら黙らせる.
	if (PlayCD) Stop();

	// CDの使用開始
	mciSendString("open cdaudio",NULL,0,NULL);

	wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
			StartM,StartS,StartF,EndM,EndS,EndF);

	// 曲の再生
	mciSendString(Buffer,NULL,0,Application->GethWnd() );
	PlayCD  =true;
	LoopCD  =false;
}

/*******************************************************************/
//  曲のループ再生
/*******************************************************************/
void TCDPlayer::Replay()
{
	if (LoopMode)
	{
		Play(StartSongNo,EndSongNo);
	}
	else
	{
		Stop();
	}

	LoopCD=TRUE;
}

/*******************************************************************/
// 曲の停止
/*******************************************************************/
void TCDPlayer::Stop()
{
	if (PlayCD)
	{
		mciSendString("stop cdaudio",NULL,0,NULL);
		mciSendString("close cdaudio",NULL,0,NULL);

		PlayCD  =false;
	}
}

/*******************************************************************/
/*-  曲の一時停止                                                              -*/
/*-                                                                            -*/
/*-    BOOL Flag : true  = 一時停止                                            -*/
/*-                false = 一時停止解除                                        -*/
/*******************************************************************/

void TCDPlayer::Pause(bool Flag)
{
	char Buffer[64];
    // MCI文字列バッファ
    char MciBuffer[32];

	if (Flag)
	{
        //一時停止
		if (PlayCD)
		{   
			// 現在の時間を取得
			mciSendString("status cdaudio position",MciBuffer,32,NULL);

			Buffer[0]=MciBuffer[0];
			Buffer[1]=MciBuffer[1];
			Buffer[2]=NULL;

			PauseM=atoi(Buffer);

			Buffer[0]=MciBuffer[3];
			Buffer[1]=MciBuffer[4];
			Buffer[2]=NULL;

			PauseS=atoi(Buffer);

			Buffer[0]=MciBuffer[6];
			Buffer[1]=MciBuffer[7];
			Buffer[2]=NULL;

			PauseF=atoi(Buffer);

			// 停止
			Stop();
		}
	}
	else
	{   
        //復帰
		if (PauseM!=-1 && PauseS!=-1 && PauseF!=-1)
		{
			if (PauseM>CDInfo[0].Time.m ||
				PauseM==CDInfo[0].Time.m && PauseS>CDInfo[0].Time.s ||
				PauseM==CDInfo[0].Time.m && PauseS==CDInfo[0].Time.s &&
				PauseF>CDInfo[0].Time.m)
			{
				Play(StartSongNo,EndSongNo);
			}
			else
			{
				// 時間指定で再生
				if (EndSongNo==SongMax)
				{
					Play(PauseM,PauseS,PauseF,
							   CDInfo[0].Time.m,CDInfo[0].Time.s,
							   CDInfo[0].Time.f);
				}
				else
				{
					Play(PauseM,PauseS,PauseF,
							   CDInfo[EndSongNo+1].Start.m,
							   CDInfo[EndSongNo+1].Start.s,
							   CDInfo[EndSongNo+1].Start.f);
				}
			}

			PauseM=-1;
			PauseS=-1;
			PauseF=-1;
		}
	}
}

/*******************************************************************/
//  曲のスキップ
//    int Count : スキップ数 ( 1:次の曲 / -1:前の曲 )
/*******************************************************************/
void TCDPlayer::Skip(int Count)
{
	int Now;

	Now= NowSong()+Count;

	if (Now<StartSongNo)
	{
		Now=EndSongNo;
	}

	Pause(true);

	PauseM=CDInfo[Now].Start.m;
	PauseS=CDInfo[Now].Start.s;
	PauseF=CDInfo[Now].Start.f;

	Pause(false);
}

/*******************************************************************/
//曲の早送り
//    int Time : 早送り時間 ( 秒 )
/*******************************************************************/
void TCDPlayer::Forward(int Time)
{
    //一時停止状態を作る
	Pause(true);

	PauseS+=Time;

	if (PauseS>=60)
	{
		PauseS-=60;

		if (++PauseM>CDInfo[0].Time.m ||
			PauseM==CDInfo[0].Time.m && PauseS>CDInfo[0].Time.s ||
			PauseM==CDInfo[0].Time.m && PauseS==CDInfo[0].Time.s &&
			PauseF>CDInfo[0].Time.f)
		{
			PauseM=CDInfo[0].Time.m;
			PauseS=CDInfo[0].Time.s;
			PauseF=CDInfo[0].Time.f;
		}
	}
    //停止解除  早送りしたところからリスタート
	Pause(false);
}

/*******************************************************************/
/*-  曲の巻戻し                                                                -*/
/*-                                                                            -*/
/*-    int Time : 巻戻し時間 ( 秒 )                                            -*/
/*******************************************************************/
void TCDPlayer::Backward(int Time)
{
    //一時停止状態を作る
	Pause(true);

	PauseS-=Time;

	if (PauseS<0)
	{
		PauseS+=60;

		if (--PauseM<0)
		{
			PauseM=0;
			PauseS=0;
			PauseF=0;
		}
	}
    //停止解除  早送りしたところからリスタート
	Pause(false);
}

/*******************************************************************/
//CD挿入状態の取得
//    戻り値 : TRUE  = CDあり
//             FALSE = CDなし
/*******************************************************************/
bool TCDPlayer::Ready()
{
    // MCI文字列バッファ
    char MciBuffer[32];

	mciSendString("status cdaudio media present",MciBuffer,32,NULL);

	if (MciBuffer[0]=='t')
	{
		mciSendString("status cdaudio ready",MciBuffer,32,NULL);

		if (MciBuffer[0]=='t')
		{   
            //CD がありまっせ
			return true;
		}
	}
    //CD がないでっせ
	return false;
}

/*******************************************************************/
//  CDドライブのオープン
/*******************************************************************/
void TCDPlayer::Open()
{
	mciSendString("set cdaudio door open",NULL,0,NULL);
}

/*******************************************************************/
//  CDドライブのクローズ
/*******************************************************************/
void TCDPlayer::Close()
{
	mciSendString("set cdaudio door closed",NULL,0,NULL);
}

/*******************************************************************/
//ループ再生の無効化  ( Play関数の後に使用 )
/*******************************************************************/
void TCDPlayer::NoLoop()
{
	LoopMode=FALSE;
}

/*******************************************************************/
// CDの曲数の取得
/*******************************************************************/
int TCDPlayer::MaxSong()
{
	return SongMax;
}

/*******************************************************************/
//  再生中の曲番号の取得
/*******************************************************************/
int TCDPlayer::NowSong()
{
    // MCI文字列バッファ
    char MciBuffer[32];

	mciSendString("status cdaudio current track",MciBuffer,32,NULL);

	if (PlayCD)
	{
		return atoi(MciBuffer);
	}
	else
	{
		return 0;
	}
}

/*********************************************************************/
//再生が終了しましたっせ.
/*********************************************************************/
void TCDPlayer::MciSuccessfulEnd()
{
    // MCI文字列バッファ
    char MciBuffer[32];

	if (PlayCD)
	{
		mciSendString("status cdaudio mode",MciBuffer,32,NULL);

        if (MciBuffer[0]=='s' && MciBuffer[1]=='t')
        {
			Replay();   //リピート
		}
	}
}


#endif
