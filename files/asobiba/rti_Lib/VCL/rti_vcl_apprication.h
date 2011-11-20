#include "StartWithExeFile.h"
#ifdef APPLICATION_LIFE

#ifndef ___APPLICATIONH
#define ___APPLICATIONH

#include <windows.h>
#include <.\VCL\rti_vcl_windowobject.h>
#include <.\Music\rti_music.h>
#include <.\Music\rti_cd.h>
#include <.\Music\rti_wave_player.h>

#if defined(PROGRESS_LIFE) || defined(STATUS_LIFE) || defined(TABCOONTROL_LIFE) || defined(TOOLBAR_LIFE)
#define COMMCTRL_USING
#pragma comment(lib,"Comctl32.lib")
#endif

#ifdef COMMCTRL_USING
	#include <commctrl.h>
#endif

class TApplication : public TWinObject
{
private:
    int IdCounter;
    HINSTANCE hInstance;        //インストラクタ
    HINSTANCE hPrevInstance;
    LPSTR lpszArgs;
    int nWinMode;
    void Init();                //
    void End();
#ifdef MUSIC_LIFE
    TMusic *IamMusic;
#endif
#ifdef CD_LIFE
    TCDPlayer *IamCD;
#endif
#ifdef WAVE_SOUND_LIFE
    TWavePlayer *IamWave;
#endif
public:

#ifdef MUSIC_LIFE
    //音楽を使うよぉー
    TMusic* MusicUse(){return (IamMusic ? NULL :IamMusic = new TMusic );};
    //音楽のハンドルの取得
    TMusic* GetMusic(){return IamMusic;}; 
#endif
#ifdef CD_LIFE
    //CDを使うよぉー
    TCDPlayer* CDUse(){return (IamCD ? NULL :IamCD = new TCDPlayer );};
    //CDのハンドルの取得
    TCDPlayer* GetCD(){return IamCD;}; 
#endif
#ifdef WAVE_SOUND_LIFE
    //WAVEを使うよぉー
    TWavePlayer* WaveUse(){return (IamWave ? NULL :IamWave = new TWavePlayer );};
    //WAVEのハンドルの取得
    TWavePlayer* GetWave(){return IamWave;}; 
#endif

    TApplication(HINSTANCE ihInstance , HINSTANCE ihPrevInstance,
				   LPSTR ilpszArgs , int inWinMode);
    ~TApplication();

    //あいている ID を取得.
    int GetId(){return IdCounter++;};

    //インストラクタ取得
    HINSTANCE GethInstance(){return hInstance;};
    //ウィンドモードの取得
    int       GetnWinMode(){return nWinMode;};

    //メインウィンドに依存するメッセージの処理
    LRESULT DefProc(HWND hwnd , UINT message,
           						 WPARAM wParam,LPARAM lParam);

    //終了　レベル1
    void AppExit(){SendMessage((HWND)hWnd,WM_CLOSE,0L,0L);};
    //終了　レベル2
    void AppDestroy(){SendMessage((HWND)hWnd,WM_DESTROY,0L,0L);};
    //終了　レベル3
    void AppQuit(){SendMessage((HWND)hWnd,WM_QUIT,1L,0L);};
};

extern TApplication *Application;

#endif

#endif //LIFE END
