/**********************************************************/
// アプリケーションの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef APPLICATION_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_debug.h>

TApplication *Application;

/**********************************************************/
//コンストラクタ
/**********************************************************/
TApplication::TApplication(HINSTANCE ihInstance , HINSTANCE ihPrevInstance,
				   LPSTR ilpszArgs , int inWinMode)
{
    //WinMain の引数を記録しておく.
    hInstance     = ihInstance;
    hPrevInstance = ihPrevInstance;
    lpszArgs      = ilpszArgs;
    nWinMode      = inWinMode;
    //ID コレクターの初期値
    IdCounter     = 100;
    //私の属性 = ボス
    Kind = OBJECT_KIND_APPLICATION;
    //ウィンドハンドルはとりあえず、 NULL にしておく.
    hWnd			= NULL;
    //その他の初期化
    Init();
}

/**********************************************************/
//デストラクタ
/**********************************************************/
TApplication::~TApplication()
{
    //終了処理
    End();
    //自分の持っていたすべてのコントロールを閉じる
    OwerKill        = true;
    ChildDestroy();
}

/**********************************************************/
//初期化
/**********************************************************/
void TApplication::Init()
{
#ifdef COMMCTRL_USING
    InitCommonControls(); //コモンコントロールの初期化
#endif
#ifdef MUSIC_LIFE
    IamMusic = NULL; //Midi コントロールの初期化
#endif
#ifdef CD_LIFE
    IamCD = NULL;   //CD コントロールの初期化
#endif
#ifdef WAVE_SOUND_LIFE
    IamWave = NULL; //Waveコントロール
#endif
}

/**********************************************************/
//終了処理
/**********************************************************/
void TApplication::End()
{
#ifdef CD_LIFE
    delete IamCD; //CD破棄
#endif
#ifdef MUSIC_LIFE
    delete IamMusic; //音楽破棄
#endif
#ifdef WAVE_SOUND_LIFE
    delete IamWave; //Wave破棄
#endif
}


/**********************************************************/
//ボスに聞く. DefProc
/**********************************************************/
LRESULT TApplication::DefProc(HWND hwnd , UINT message,
           						 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
	case MM_MCINOTIFY:
		if (wParam==MCI_NOTIFY_SUCCESSFUL)
		{
#ifdef MUSIC_LIFE
            if (IamMusic) IamMusic->MciSuccessfulEnd();
#endif
#ifdef CD_LIFE
            if (IamCD) IamCD->MciSuccessfulEnd();
#endif
        }
		break;
#ifdef WAVE_SOUND_LIFE
    case MM_WOM_DONE:
        if (IamWave) IamWave->WaveSuccessfulEnd((WAVEHDR*)lParam);
        break;
#endif
 	default:
        //ボスもお手上げなので Windows 君に聞く.
		return DefWindowProc(hwnd,message,wParam,lParam);
    }
    return 0;
}


#endif //LIFE END
