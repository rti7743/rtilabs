#include "StartWithExeFile.h"
#ifdef MUSIC_LIFE
/*******************************************************************/
/*
    Midi 再生ルーチン for MCI

    Thank you For EL (Eazy Link Library)    Botchy

    再構築 RTI

*/
/*******************************************************************/

#include <.\Music\rti_music.h>
#include <.\VCL\rti_vcl_apprication.h>

/*********************************************************************/
//初期化
/*********************************************************************/
bool TMusic::Init()
{
	PlayMusic = false;
	LoopMusic = false;
	LoopMode  = true;

	return true;
}

/*********************************************************************/
//終了処理
/*********************************************************************/
void TMusic::End()
{
	mciSendString("stop midi",NULL,0,NULL);
	mciSendString("close midi",NULL,0,NULL);
}

/*********************************************************************/
//再生
/*********************************************************************/
void TMusic::Play(char* FileName)
{
	char Buffer[256];

	sprintf(Buffer,"open \"%s\" type sequencer alias midi",FileName);

	if (PlayMusic) Stop();

	mciSendString(Buffer,NULL,0,NULL);
	mciSendString("seek midi to start",NULL,0,NULL);
	mciSendString("play midi notify",NULL,0,Application->GethWnd() );

	PlayMusic =true;
	LoopMusic =false;
	LoopMode  =true;
}

/*********************************************************************/
//ループ再生
/*********************************************************************/
void TMusic::Replay()
{
	if (LoopMode)
	{
		mciSendString("seek midi to start",NULL,0,NULL);
		mciSendString("play midi notify",NULL,0,Application->GethWnd() );
	}
	else
    { //ストップ
		Stop();
	}

	LoopMusic= true;
}

/*********************************************************************/
//ストップ
/*********************************************************************/
void TMusic::Stop()
{
	if (PlayMusic)
	{
		mciSendString("stop midi",NULL,0,NULL);
		mciSendString("close midi",NULL,0,NULL);

		PlayMusic= false;
	}
}

/*********************************************************************/
//ループの無効化
/*********************************************************************/
void TMusic::NoLoop(void)
{
	LoopMode= false;
}

/*********************************************************************/
//再生が終了しましたっせ.
/*********************************************************************/
void TMusic::MciSuccessfulEnd()
{
    // MCI文字列バッファ
    char MciBuffer[32];

    if (PlayMusic )
	{
	    mciSendString("status midi mode",MciBuffer,32,NULL);
    	if (MciBuffer[0]=='s' && MciBuffer[1]=='t')
		{
			Replay();
		}
	}
}


#endif  //LIFE END
