#define TIME_CHECK		//時間計測を行う
#define INTERVAL 110	//フレームレート(ms)

#include <rti_lib.h>
#pragma comment(lib,"Winmm.lib")

//コールバック.
static LRESULT CALLBACK GameProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam);
//ダミーコールバック.
static LRESULT CALLBACK DummyProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam);
//アクティブフラグ
static BOOL fActive = true;
//メインフォーム
TForm *GameForm;

//ゲームを作る場合の WinMain です.
//面倒なので、一つの関数にしてしまいます.
//なお、 Application は、最低作ってから呼んでください.
int GameMain(HINSTANCE hInstance , HINSTANCE hPrevInstance,LPSTR lpszArgs , int nWinMode,
			 int ScreenX,int ScreenY)
{
	MSG       msg;      //メッセージ取得用の構造体
    HWND      hwnd;     //メインウィンド
	DEBUG("================================実験開始==========================================");
    lookup_making(); //ルックアップテーブル作成

    try
    {
	    if (lstrcmp(lpszArgs,"DDX")==0)
	    {   //絵画エンジンのベースは DDX
			DDXAdoption();
	    }
		else
	    {
		    //絵画エンジンのベースは GDI
			GDIAdoption();
	    }
	    //メインフォーム作成
	    GameForm = new TForm(NULL,"ぽりごん りべんじ",WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN	,
    		CW_USEDEFAULT,CW_USEDEFAULT,ScreenX,ScreenY,
	    	NULL,DummyProc);
        if (! GameForm)
                throw MemoryDeficient("GameMain","メインフォーム 作成",sizeof(TForm) );
        if (! GameForm->GetKind() )
        {   
            delete GameForm; 
            throw FriendlyError("GameMain","メインフォーム 作成","?","?" );
        }
	    hwnd = GameForm->GethWnd();
	    //↓こいつは、いれないと、おかしな結果になる. うーむ. おそろしや.
        ShowWindow(hwnd ,SW_SHOWDEFAULT);  //ウィンド作成開始
        //画面解像度変更
        GameForm->ReClasSize(ScreenX,ScreenY);
	    if (!CommDraw->Init(hwnd,ScreenX,ScreenY,24) )	
            throw FriendlyError("GameMain","グラフィックスエンジン初期化","?","このモードでの起動は無理" );
	    if (main_init(hwnd) != SUCCESS_MITION) 
        {
            main_destory(hwnd);
            throw FriendlyError("GameMain","メイン初期化処理","?","?" );
        }
		//ここで、初期化が終了したので、正式なプロシージャをウィンド君に贈呈.
		SetWindowLong(hwnd, GWL_WNDPROC,(long)GameProc);

	    UpdateWindow(hwnd);         //ウィンド表示開始
	    SetFocus(hwnd);
	    //ShowCursor(0);//マウスを表示しない
    }
    catch(...)
    {
       	CommDraw->End();
        MSGOUT("プログラムの緊急停止!!","プログラムの初期化に問題があります.\n プログラムを緊急停止します.");
        return -1;
    }
    //メッセージループ部
   	while( 1 )
    {
   		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    	{
	    	if( msg.message == WM_QUIT )	break;
		    TranslateMessage( &msg );
   			DispatchMessage( &msg );
        } 
	    else 
		{
			if (fActive)	PostMessage( hwnd, WM_TIMER, 0, 0 );
			else            WaitMessage();
		}
    }
  	CommDraw->End();
	return msg.wParam;
}

//ダミーコールバック.
//ゲーム初期化中につまらないメッセージをバカ(ゲイツ窓)が送ってくれるので、
//それを回避するためにダミーのプロシージャを作っておく.
LRESULT CALLBACK DummyProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:          //ウィンドの終了メッセージ
		PostQuitMessage(0);   //終了メッセージの発行
		break;
	default:
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

//コールバック.
LRESULT CALLBACK GameProc(HWND hwnd , UINT message, WPARAM wParam,LPARAM lParam)
{
    unsigned long TempTimeGet;
	static unsigned long NowTime = 0;

	switch(message)
	{
	case WM_ACTIVATEAPP:
		fActive = (BOOL) wParam;	
		break;
	case WM_PAINT:             //絵画メッセージ
		Backsurface->OnPaint();
		break;
	case WM_MOVE:				//ウィンドが動いちゃったとき.
		Backsurface->OnMove(lParam);
		break;
	case WM_MOUSEMOVE:     //マウスが動かされたとき
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
		if (wParam & MK_LBUTTON	) 
		{
              	        MouseEL  = true;    //押され続けているを立てる
		}
		else
		{
			if (wParam & MK_RBUTTON	) 
			{
				        MouseER  = true;
			}
			else
			{
			 	MouseER  = false;
				MouseEL  = false;
			}
		}
		break;
	case WM_LBUTTONDOWN:   //左ボタン
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
        MouseCL  = true ;  //一回だけのダウンのクリア
		if (wParam & MK_RBUTTON) MouseCR  = true ;  //一回だけのダウンのクリア
		MouseEL  = false ;
		MouseER  = false ;
		break;
	case WM_RBUTTONDOWN:  //右ボタン
		MouseRX  = LOWORD(lParam);
		MouseRY  = HIWORD(lParam);
        MouseCR  = true ;  //一回だけのダウンのクリア
		if (wParam & MK_LBUTTON) MouseCL  = true ;  //一回だけのダウンのクリア
		MouseEL  = false ;
		MouseER  = false ;
		break;
	case WM_LBUTTONUP:   //左ボタン
		MouseEL  = false ;
		MouseCL = false;
		break;
	case WM_RBUTTONUP:  //右ボタン
		MouseER  = false ;
		MouseCR = false;
		break;

	case WM_TIMER:            //タイマーだよーん
        TempTimeGet = timeGetTime();
		NOW_TIME(TempTimeGet - NowTime);	//計測
		if (TempTimeGet - NowTime >= INTERVAL)
		{
			//1フレーム進む
			NowTime = TempTimeGet;

			MousePX = MouseRX;
			MousePY = MouseRY;

			main_rtn(hwnd);

			MouseCR = false;
			MouseCL = false;
		}
		break;
	case WM_KEYDOWN:
		LastKey = (unsigned char) wParam;
		break;
    case WM_CLOSE:
		main_destory(hwnd);   //メインルーチンの破棄
		END_TIME(INTERVAL);	//計測終了
        DestroyWindow(hwnd);
		break;
	case WM_DESTROY:          //ウィンドの終了メッセージ
		PostQuitMessage(0);   //終了メッセージの発行
		break;
 	default:
        return Application->DefProc(hwnd,message,wParam,lParam);
	}
	return 0;
}
