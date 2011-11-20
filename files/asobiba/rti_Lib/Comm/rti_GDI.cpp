#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_ChangeDisplay.h>	//GDI 解像度変換
#include <.\Game\rti_GameMain.h>

//初期化.
static bool GDIInit(HWND hwnd,int w,int h,int bps);
//終了処理.
static bool GDIEnd();
//ファイル読み込み
static bool GDIGraphicsMake(BMD puf,const char* filename);
static bool GDIGraphicsMakeSub(BMD bmd,HDC memDC);
//領域確保
static bool GDIGraphicsAlloc(BMD bmd);
//領域解放
static bool GDIGraphicsFree(BMD bmd);
//ロック
static bool GDIGraphicsLock(BMD bmd);
//アンロック
static bool GDIGraphicsUnLock(BMD bmd);
//強引に bmd に DC を作成する (^^;;;
static HDC GDIGetDC(BMD bmd);
//DC を消し去る. (DC に対する操作がフィードバックされる.)
static bool GDIDeleteDC(BMD bmd,HDC hdc);
//DC->BMD の中身を更新する.
static bool GDIUpdataDC(BMD bmd,HDC hdc);
//現在の絵画モードの取得
static UseDrawMode  GDIGetMode();
//リフレッシュ促進.
static void GDIRef(HWND hwnd);
//フルスクリーンにしろ!
static bool GDIDoFullScreen(bool CoerceChange);
//ウィンドモードにしろ!!
static bool GDIDoWindow(bool CoerceChange);
//絵画ポイントの設定(GDIでは無効です)
static void GDISetDrawPoint(int x,int y);

//GDIが利用する初期化ルーチンだぴょん.
static struct _CommDraw CommDrawGDI = 
	{GDIInit,GDIEnd,GDIGraphicsMake,GDIGraphicsAlloc,GDIGraphicsFree,
		GDIGraphicsLock,GDIGraphicsUnLock,GDIGetDC,GDIDeleteDC,GDIUpdataDC,GDIGetMode,GDIRef,
		GDIDoFullScreen,GDIDoWindow,GDISetDrawPoint};
//解像度変換
static TChangeDisplay TCS;

//GDIの処理採用するってこと.
void GDIAdoption()
{
	CommDraw = &CommDrawGDI;
}

//初期化処理.
bool GDIInit(HWND hwnd,int w,int h,int bps)
{
	NowScreenMode.fFullScreen = false; //GDI モードは、ウィンドモードで起動.
	NowScreenMode.iWidth      = w;
	NowScreenMode.iHeight     = h;
	NowScreenMode.iBpp        = bps;
	return true;
}

//終了処理.
bool GDIEnd()
{
	if (NowScreenMode.fFullScreen)
	{
		//フルスクーリンだったら解像度を元に戻す.
		TCS.Restoration();
	}
	return true;
}

//フルスクーリン
bool GDIDoFullScreen(bool CoerceChange = false)
{
	if (!CoerceChange)
		if (NowScreenMode.fFullScreen)	return false;	//すでにフルスクーリンです.
    //解像度変更
	if ( TCS.Change(NowScreenMode.iWidth,NowScreenMode.iHeight,0) )
	{
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, FULLWINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		GameForm->Move(0,0);	//これをしないと、 ゲイツ窓98 では、おかしくなる.
		NowScreenMode.fFullScreen = true;
		return true;
	}
	else
	{
		FriendlyError("GDIDoFullScreen","フルスクーリン設定",
			"このビデオカードorディスプレイは、この解像度をサポートしていません.",
			"ビデオカードなどをまともなものと交換してください");
		return false;
	}
}

//ウィンドモード
bool GDIDoWindow(bool CoerceChange = false)
{
	if (!CoerceChange)
		if (!NowScreenMode.fFullScreen)	return false;	//すでにウィンドモードです.
    //解像度変更
	if ( TCS.Restoration() )
	{
    	SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, WINDOW_STYLE);
	    GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
        //GameForm->Align(ALIGN_MOVE_CENTER);
		GameForm->Move(100,100);
        InvalidateRect(NULL,NULL,TRUE);	//これをしないと、再絵画されない 窓がたまにでる.
    	NowScreenMode.fFullScreen = false;
		return true;
	}
	else
	{
		FriendlyError("GDIDoWindow","ウィンドモードにするために画面を元に戻す設定",
			"原因不明","原因不明です.");
		return false;
	}
}

bool GDIGraphicsMake(BMD puf,const char* filename)
{
    BITMAP bmp_info;
    HDC memDC = NULL;            //メモリデバイスコンテキスト
    HDC hdc   = NULL;
    HBITMAP hBit = NULL;         //ビットマップだよん
    try
    {
	    hdc = GetDC(NULL);                     //デバイスコンテキスト取得
        if (!hdc)
			throw (FriendlyError("GraphicsMake","デバイスコンテキスト取得","?","?"));

    	memDC = CreateCompatibleDC(hdc); //メモリDC取得
        if (!memDC)
			throw (FriendlyError("GraphicsMake","MemDC取得","?","?"));

        if ( !ReleaseDC(NULL,hdc) )		//DC 開放
			 throw (FriendlyError("GraphicsMake","デバイスコンテキスト破棄","",""));

	    hBit = (HBITMAP) LoadImage(NULL,filename,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作
	    if (!hBit) throw (FileNotOpen("GraphicsMake",filename) );

        GetObject(hBit, sizeof(BITMAP), &bmp_info);
        puf->w = bmp_info.bmWidth;
        puf->h = bmp_info.bmHeight;
    
	    SelectObject(memDC,hBit);   //ビットマップ選択
    
	    if ( !GDIGraphicsMakeSub(puf,memDC) )
			 throw (FriendlyError("GraphicsMake","画像領域確保&転送","メモリ足りてますか?","メモリを増やしてください"));
        if ( DeleteDC(memDC) ) memDC = NULL;       //メモリDC開放
        else  throw (FriendlyError("GraphicsMake","MemDC削除","?","?"));

        if ( DeleteObject(hBit) ) hBit = NULL;    //ビットマップハンドル削除
        else  throw (FriendlyError("GraphicsMake","HBITMAP削除","?","?"));
    }
    catch(...)
    {
		if (hdc)	ReleaseDC(NULL,hdc);		   //DC破棄
	    if (memDC)  DeleteDC(memDC);       //メモリDC開放
	    if (hBit)   DeleteObject(hBit);    //ビットマップハンドル削除
	   	return false;
    }
	return true;
}

//領域確保
bool GDIGraphicsMakeSub(BMD bmd,HDC memDC)
{
    if (! CommDraw->GraphicsAlloc(bmd) ) return false;

	//画像転送(手抜き～)
	BitBlt( (HDC)(bmd->Tag) ,0,0,bmd->w,bmd->h, memDC,0,0,SRCCOPY);

	return true;
}

//領域確保
bool GDIGraphicsAlloc(BMD bmd)
{
	unsigned char Pow2W;
	int X,Y;
	BITMAPINFO bm_info;
	HBITMAP hBit;
	HDC hdc,memDC;

	X = bmd->w ;
	Y = bmd->h ;
	//2のべき数調べ.
	CheckPow2(&Pow2W,X);
	bmd->Pow2W = Pow2W;

	hdc = GetDC(NULL);

	memset(&bm_info,0,sizeof(BITMAPINFO));
	bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bm_info.bmiHeader.biWidth = X;
	bm_info.bmiHeader.biHeight = -(Y+1);	// + 1 しないとヤバです (^^;;
	bm_info.bmiHeader.biPlanes = 1;
	bm_info.bmiHeader.biBitCount = 24;
	bm_info.bmiHeader.biCompression = BI_RGB;
//	bm_info.bmiHeader.biSizeImage = (X * 3) * (Y+1);
	hBit = CreateDIBSection( hdc, &bm_info , DIB_RGB_COLORS, (void **)&bmd->bm, NULL, 0 );
	memDC = CreateCompatibleDC( hdc );
	if( ! memDC )
	{
		DeleteObject( hBit );
		return	false;
	}
	SelectObject( memDC, hBit );
	//ふつーの DC の削除.
	ReleaseDC(NULL,hdc);
	//GDI モードはメモリデバイスコンテキストを Tag に入れます.
	bmd->Tag = memDC;

	return true;						//大成功!! (^-^
}

//領域解放
bool GDIGraphicsFree(BMD bmd)
{
	HDC memDC = (HDC) bmd->Tag;
	HBITMAP hBit = (HBITMAP)GetCurrentObject(memDC,OBJ_BITMAP);

	DeleteObject( hBit );
	DeleteDC( memDC ) ;

	return true;
}

//ロック
bool GDIGraphicsLock(BMD bmd)
{
	return true;
}

//アンロック
bool GDIGraphicsUnLock(BMD bmd)
{
	return true;
}

//強引に bmd に DC を作成する (^^;;;
HDC GDIGetDC(BMD bmd)
{
	return (HDC)bmd->Tag	;						//メモリデバイスコンテキストを返す.
}

//DC を消し去る. (DC に対する操作がフィードバックされる.)
bool GDIDeleteDC(BMD bmd,HDC hdc)
{
	return true;
}
 

//DC->BMD の中身を更新する.
bool GDIUpdataDC(BMD bmd,HDC hdc)
{
	return true;
}

//現在の絵画モードの取得
UseDrawMode GDIGetMode()
{
	return USE_GDI_MODE;
}

//リフレッシュ促進.
void GDIRef(HWND hwnd)
{
	InvalidateRect(hwnd,NULL,false);
}

//絵画ポイントの設定(GDIでは無効です)
void GDISetDrawPoint(int x,int y)
{
}

