/**************************************************************/
//リティライブラリ DirectDraw エミュレーションモード.
/**************************************************************/

#include <.\Comm\rti_Comm.h>
#include <.\Comm\rti_DirectX.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Game\rti_GameMain.h>

#pragma comment(lib,"ddraw.lib")

#define	SetErrorMessage( ErrCase, String )	\
	case ErrCase: return (String); break;

static char* DDCreateFail( HRESULT hResult )
{
	switch( hResult ){
//		SetErrorMessage( DD_OK , "要求は成功し完了した。" );
		SetErrorMessage( DDERR_ALREADYINITIALIZED , "オブジェクトはすでに初期化されている。" );
		SetErrorMessage( DDERR_BLTFASTCANTCLIP , "DirectDrawClipper オブジェクトがIdirectDrawSurface3::BltFastメソッドの呼び出しに渡される転送元サーフェスにアタッチされている。" );
		SetErrorMessage( DDERR_CANNOTATTACHSURFACE , "要求されたサーフェスにはサーフェスをアタッチできない。" );
		SetErrorMessage( DDERR_CANNOTDETACHSURFACE , "要求されたサーフェスからサーフェスをデタッチできない。" );
		SetErrorMessage( DDERR_CANTCREATEDC , "Windowsはそれ以上のいかなるデバイスコンテキスト（ DC ）を作ることができない。" );
		SetErrorMessage( DDERR_CANTDUPLICATE , "プライマリサーフェス、3Dサーフェス、暗黙的に作られるサーフェスは複製できない。" );
		SetErrorMessage( DDERR_CANTLOCKSURFACE , "DCI サポートなしでプライマリサーフェスのロックをしようとしたため、このサーフェスへのアクセスが拒否された。" );
		SetErrorMessage( DDERR_CANTPAGELOCK , "サーフェスのページ ロックが失敗した。 ページ ロックはディスプレイ メモリ サーフェスあるいはエミュレートされたプライマリサーフェスの上では動作しない。" );
		SetErrorMessage( DDERR_CANTPAGEUNLOCK , "サーフェスのページ ロック解除が失敗した。 ページ ロック解除はディスプレイ メモリ サーフェスあるいはエミュレートされたプライマリサーフェスの上では動作しない。" );
		SetErrorMessage( DDERR_CLIPPERISUSINGHWND , "ウィンドウハンドルをすでにモニタしているDirectDrawClipper オブジェクトにクリッピングリストをセットしようとした。" );
		SetErrorMessage( DDERR_COLORKEYNOTSET , "転送元カラーキー が指定されていない。" );
		SetErrorMessage( DDERR_CURRENTLYNOTAVAIL , "現在サポートされていない。" );
		SetErrorMessage( DDERR_DCALREADYCREATED , "デバイスコンテキスト（DC）はすでにこのサーフェスに対し返されている。１サーフェスにつき1つのDCしか取得することはできない。" );
//		SetErrorMessage( DDERR_DEVICEDOESNTOWNSURFACE, "1 つの DirectDraw デバイスによって作成されたサーフェスは、別の DirectDraw デバイスが直接使用することはできない。" );
		SetErrorMessage( DDERR_DIRECTDRAWALREADYCREATED , "DirectDrawオブジェクトはすでに作成されている。" );
		SetErrorMessage( DDERR_EXCEPTION , "要求された処理を行う間に例外が発生した。" );
		SetErrorMessage( DDERR_EXCLUSIVEMODEALREADYSET , "すでに排他的モードがセットされているにもかかわらず、協調レベルをセットしようとした。" );
		SetErrorMessage( DDERR_GENERIC , "未定義のエラー状態である。" );
		SetErrorMessage( DDERR_HEIGHTALIGN , "与えられた矩形の高さは必要とされる整列の倍数ではない。" );
		SetErrorMessage( DDERR_HWNDALREADYSET , "DirectDraw協調レベル ウィンドウハンドルはすでに設定されている。ウィンドウハンドルはプロセスがサーフェスあるいは生成したパレットを保持している間はリセットできない。" );
		SetErrorMessage( DDERR_HWNDSUBCLASSED , "DirectDraw協調レベル ウィンドウハンドルが サブクラス化されているため、DirectDrawの状態のリストアが阻止された。" );
		SetErrorMessage( DDERR_IMPLICITLYCREATED , "暗黙的に作られたサーフェスをリストアしようとした。" );
		SetErrorMessage( DDERR_INCOMPATIBLEPRIMARY , "プライマリサーフェス 作成要求が既存のプライマリサーフェスと一致していない。" );
		SetErrorMessage( DDERR_INVALIDCAPS , "コールバック関数に渡された1つ以上の能力ビットが不正である。" );
		SetErrorMessage( DDERR_INVALIDCLIPLIST , "DirectDraw は与えられたクリッピングリスト をサポートしない。" );
		SetErrorMessage( DDERR_INVALIDDIRECTDRAWGUID , "DirectDrawCreate 関数に渡されるグローバルユニーク識別子（ GUID ）は有効な DirectDraw ドライバ識別子ではない。" );
		SetErrorMessage( DDERR_INVALIDMODE , "DirectDraw は要求されたモードをサポートしない。" );
		SetErrorMessage( DDERR_INVALIDOBJECT , "DirectDraw は無効なDirectDraw オブジェクトのポインタを受け取った。" );
		SetErrorMessage( DDERR_INVALIDPARAMS , "メソッドに渡された1つ以上のパラメータが正しくない。" );
		SetErrorMessage( DDERR_INVALIDPIXELFORMAT , "指定されたピクセルフォーマットは無効である。" );
		SetErrorMessage( DDERR_INVALIDPOSITION , "転送先上のオーバーレイの位置が不正である。" );
		SetErrorMessage( DDERR_INVALIDRECT , "与えられた矩形が無効である。" );
		SetErrorMessage( DDERR_INVALIDSURFACETYPE , "サーフェスが間違ったタイプであったため、要求された処理を実行できなかった。" );
		SetErrorMessage( DDERR_LOCKEDSURFACES , "1つ以上のサーフェスがロックされており、要求された処理に失敗した。" );
//		SetErrorMessage( DDERR_MOREDATA, "指定したバッファサイズが保有できる以上のデータが存在する。" );
		SetErrorMessage( DDERR_NO3D , "3Dハードウェアあるいはエミュレーションが存在しない。" );
		SetErrorMessage( DDERR_NOALPHAHW , "アルファ アクセラレーション ハードウェアが存在しないかあるいは利用できなかったため、要求された処理に失敗した。" );
		SetErrorMessage( DDERR_NOBLTHW , "ブロック転送ハードウェアが存在しない。" );
		SetErrorMessage( DDERR_NOCLIPLIST , "クリッピングリストが利用できない。" );
		SetErrorMessage( DDERR_NOCLIPPERATTACHED , "このサーフェス オブジェクトにはDirectDrawClipper オブジェクトをアタッチできない。" );
		SetErrorMessage( DDERR_NOCOLORCONVHW , "色変換ハードウェアが存在しない、あるいは利用できないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOCOLORKEY , "サーフェスは現在カラーキーを持っていない。" );
		SetErrorMessage( DDERR_NOCOLORKEYHW , "転送先カラーキーに対するハードウェアサポートがないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOCOOPERATIVELEVELSET , "IdirectDraw2::SetCooperativeLevelメソッドを呼び出すことなく作成関数が呼び出された。" );
		SetErrorMessage( DDERR_NODC , "DCは、これまでにこのサーフェス用に作られていなかった。" );
		SetErrorMessage( DDERR_NODDROPSHW , "DirectDraw ラスタ処理（ ROP ）ハードウェアが利用できない。" );
		SetErrorMessage( DDERR_NODIRECTDRAWHW , "ハードウェア専用DirectDraw オブジェクトを作成できない。ドライバはハードウェアをサポートしない。" );
		SetErrorMessage( DDERR_NODIRECTDRAWSUPPORT , "現在のディスプレイ ドライバはDirectDrawをサポートしていない。" );
		SetErrorMessage( DDERR_NOEMULATION , "ソフトウェア エミュレーションが利用できない。" );
		SetErrorMessage( DDERR_NOEXCLUSIVEMODE , "アプリケーションに対し排他的モードが要求されたが、アプリケーションは排他的モードを持っていない。" );
		SetErrorMessage( DDERR_NOFLIPHW , "可視サーフェスはフリッピングできない。" );
		SetErrorMessage( DDERR_NOGDI , "GDI が存在しない。" );
		SetErrorMessage( DDERR_NOHWND , "クリッピング通知がウィンドウハンドルを必要とする、あるいはウィンドウハンドルが協調レベル ウィンドウハンドルとしてあらかじめセットされていない。" );
		SetErrorMessage( DDERR_NOMIPMAPHW , "ミップマップ テクスチャ マッピングのハードウェアが存在していないか、あるいは利用できないので処理を実行できない。" );
		SetErrorMessage( DDERR_NOMIRRORHW , "ミラーリング ハードウェアが存在しないかあるいは利用できないため、処理が実行できない。" );
//		SetErrorMessage( DDERR_NONONLOCALVIDMEM, "非ローカルビデオメモリをサポートしないデバイスから、非ローカルビデオメモリを割り当てようとした。" );
//		SetErrorMessage( DDERR_NOOPTIMIZEHW, "デバイスは、最適化されたサーフェイスをサポートしない。" );
		SetErrorMessage( DDERR_NOOVERLAYDEST , "転送先確立のためのIdirectDrawSurface3::UpdateOverlayメソッド呼び出しをしていないオーバーレイに対し、IdirectDrawSurface3::GetOverlayPositionメソッドが呼び出された。" );
		SetErrorMessage( DDERR_NOOVERLAYHW , "オーバーレイ ハードウェアが存在していないかあるいは利用できないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOPALETTEATTACHED , "パレット オブジェクトがこのサーフェスにアタッチされていない。" );
		SetErrorMessage( DDERR_NOPALETTEHW , "16色あるいは256色のパレットに対するハードウェアサポートがない。" );
		SetErrorMessage( DDERR_NORASTEROPHW , "適切なラスタ処理ハードウェアが存在しないかあるいは利用できないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOROTATIONHW , "回転ハードウェアが存在していないか、あるいは利用できないため処理が実行できない。" );
		SetErrorMessage( DDERR_NOSTRETCHHW , "拡大ハードウェアが存在していないかあるいは利用できないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOT4BITCOLOR , "DirectDrawSurface オブジェクトは、要求された４ビット色パレットを使っていない。" );
		SetErrorMessage( DDERR_NOT4BITCOLORINDEX , "DirectDrawSurface オブジェクトは、要求された４ビットのカラーインデックスパレットを使っていない。" );
		SetErrorMessage( DDERR_NOT8BITCOLOR , "DirectDrawSurface オブジェクトは、要求された８ビット色パレットを使っていない。" );
		SetErrorMessage( DDERR_NOTAOVERLAYSURFACE , "非オーバーレイサーフェスに対し、オーバーレイ コンポーネントが呼び出された。" );
		SetErrorMessage( DDERR_NOTEXTUREHW , "テクスチャ マッピング ハードウェアが存在していないかあるいは利用できないため、処理が実行できない。" );
		SetErrorMessage( DDERR_NOTFLIPPABLE , "フリッピングできないサーフェスをフリッピングしようとした。" );
		SetErrorMessage( DDERR_NOTFOUND , "要求された項目は見つからなかった。" );
		SetErrorMessage( DDERR_NOTINITIALIZED , "オブジェクトが初期化される前に、CoCreateInstanceで作成された DirectDraw オブジェクトのインターフェイス メソッドを呼び出そうとした。" );
//		SetErrorMessage( DDERR_NOTLOADED, "サーフェイスは最適化されているが、まだメモリは割り当てられてない。" );
		SetErrorMessage( DDERR_NOTLOCKED , "ロックされていないサーフェスのロック解除が行われた。" );
		SetErrorMessage( DDERR_NOTPAGELOCKED , "未処理のページ ロックがないサーフェスのページ ロック解除が行われた。" );
		SetErrorMessage( DDERR_NOTPALETTIZED , "使用しているサーフェスはパレット ベースのサーフェスではない。" );
		SetErrorMessage( DDERR_NOVSYNCHW , "垂直帰線同期処理に対するハードウェアサポートがないため、処理を実行できない。" );
		SetErrorMessage( DDERR_NOZBUFFERHW , "Zバッファに対するハードウェアサポートがないため、ディスプレイ メモリでの Z バッファ作成あるいは Z バッファを使用したブロック転送が実行できない。" );
		SetErrorMessage( DDERR_NOZOVERLAYHW , "オーバーレイサーフェスは、ハードウェアがオーバーレイ Zオーダーをサポートしないため、Zオーダーが無視された。" );
		SetErrorMessage( DDERR_OUTOFCAPS , "要求された処理に必要なハードウェアがすでに割り当てられている。" );
		SetErrorMessage( DDERR_OUTOFMEMORY , "DirectDrawが処理を行うのに十分なメモリがない。" );
		SetErrorMessage( DDERR_OUTOFVIDEOMEMORY , "DirectDrawが処理を行うのに十分なディスプレイ メモリがない。" );
		SetErrorMessage( DDERR_OVERLAYCANTCLIP , "ハードウェアはクリッピング オーバーレイをサポートしない。" );
		SetErrorMessage( DDERR_OVERLAYCOLORKEYONLYONEACTIVE , "オーバーレイ上に1つ以上の有効なカラーキーを持とうとした。" );
		SetErrorMessage( DDERR_OVERLAYNOTVISIBLE , "IdirectDrawSurface3::GetOverlayPositionメソッドが不可視オーバーレイ上で呼び出された。" );
		SetErrorMessage( DDERR_PALETTEBUSY , "パレットがもう1つのスレッドによってロックされているため、アクセスが拒否された。" );
		SetErrorMessage( DDERR_PRIMARYSURFACEALREADYEXISTS , "この処理はすでにプライマリサーフェスを作成している。" );
		SetErrorMessage( DDERR_REGIONTOOSMALL , "IdirectDrawClipper::GetClipListメソッドに渡された領域が小さすぎる。" );
		SetErrorMessage( DDERR_SURFACEALREADYATTACHED , "すでにアタッチしているもう1つのサーフェスにサーフェスをアタッチしようとした。" );
		SetErrorMessage( DDERR_SURFACEALREADYDEPENDENT , "すでに他のサーフェスに依存しているサーフェスを依存させようとした。" );
		SetErrorMessage( DDERR_SURFACEBUSY , "サーフェスがもう1つのスレッドによってロックされているため、サーフェスへのアクセスが拒否された。" );
		SetErrorMessage( DDERR_SURFACEISOBSCURED , "サーフェスへのアクセスが、サーフェスが隠蔽されているため拒否された。" );
		SetErrorMessage( DDERR_SURFACELOST , "サーフェス メモリの不足のため、サーフェスへのアクセスが拒否された。 このサーフェスを表すDirectDrawSurface オブジェクトにはIdirectDrawSurface3::Restoreメソッドが呼び出されなければならない。" );
		SetErrorMessage( DDERR_SURFACENOTATTACHED , "要求されたサーフェスはアタッチできない。" );
		SetErrorMessage( DDERR_TOOBIGHEIGHT , "DirectDraw によって要求された高さが大きすぎる。" );
		SetErrorMessage( DDERR_TOOBIGSIZE , "DirectDraw によって要求された大きさが大きすぎる。しかし、個別の高さと幅は問題がない。" );
		SetErrorMessage( DDERR_TOOBIGWIDTH , "DirectDraw によって要求された幅が大きすぎる。" );
		SetErrorMessage( DDERR_UNSUPPORTED , "サポートされていない処理であった。" );
		SetErrorMessage( DDERR_UNSUPPORTEDFORMAT , "要求された FourCC フォーマットはDirectDraw によってサポートされていない。" );
		SetErrorMessage( DDERR_UNSUPPORTEDMASK , "要求されたピクセルフォーマットでの ビットマスク はDirectDraw によってサポートされていない。" );
		SetErrorMessage( DDERR_UNSUPPORTEDMODE , "ディスプレイは現在サポートされていないモードである。" );
		SetErrorMessage( DDERR_VERTICALBLANKINPROGRESS , "垂直帰線期間である。" );
//		SetErrorMessage( DDERR_VIDEONOTACTIVE, "ビデオポートは、非アクティブである。" );
		SetErrorMessage( DDERR_WASSTILLDRAWING , "このサーフェスから、またはサーフェスへの前回のブロック転送操作が完了していない。" );
		SetErrorMessage( DDERR_WRONGMODE , "異なったモードで作られたため、このサーフェスはリストアすることができない。" );
		SetErrorMessage( DDERR_XALIGN , "与えられた矩形を必要とされる範囲に水平一列に並べることができなかった。" );
 	}
	return 0;
}

//初期化.
static bool DDXInit(HWND hwnd,int w,int h,int bps);
//そのディスプレイモードが使えるかどうかチェック.
static bool FindOutDisplayModes();
//ディスプレイモード羅列.
static HRESULT WINAPI EnumModesCallBack( DDSURFACEDESC *lpDDSurfaceDesc , void * lpContext);
//終了処理.
static bool DDXEnd();
//ファイル読み込み
static bool DDXGraphicsMake(BMD puf,const char* filename);
static bool DDXGraphicsMakeSub(BMD bmd,HDC memDC,HBITMAP hBit);
//領域確保
static bool DDXGraphicsAlloc(BMD bmd);
//領域解放
static bool DDXGraphicsFree(BMD bmd);
//ロック
static bool DDXGraphicsLock(BMD bmd);
//アンロック
static bool DDXGraphicsUnLock(BMD bmd);
//強引に bmd に DC を作成する (^^;;;
static HDC DDXGetDC(BMD bmd);
//DC を消し去る. (DC に対する操作がフィードバックされる.)
static bool DDXDeleteDC(BMD bmd,HDC hdc);
//DC->BMD の中身を更新する.
static bool DDXUpdataDC(BMD bmd,HDC hdc);
//再絵画
static void DDXRef(HWND hwnd);
//現在の絵画モードの取得
static UseDrawMode  DDXGetMode();
//リフレッシュ促進.
static void DDXRef(HWND hwnd);
//フルスクーリン
static bool DDXDoFullScreen(bool CoerceChange);
//ウィンドモード
static bool DDXDoWindow(bool CoerceChange);
//プライマリーサーフェースの作成.
static bool ReMakePrimarySurface();
//絵画ポイントの設定(ウィンドモードのみに有効です.)
static void DDXSetDrawPoint(int x,int y);


//DDXが利用する初期化ルーチンだぴょん.
static struct _CommDraw CommDrawDDX = 
	{DDXInit,DDXEnd,DDXGraphicsMake,DDXGraphicsAlloc,DDXGraphicsFree,
		DDXGraphicsLock,DDXGraphicsUnLock,DDXGetDC,DDXDeleteDC,DDXUpdataDC,DDXGetMode,DDXRef,
		DDXDoFullScreen,DDXDoWindow,DDXSetDrawPoint};

static LPDIRECTDRAW			lpDD;		   //DDオブジェクト
static LPDIRECTDRAWSURFACE	lpDDSPrimary;  //プライマリサーフェース
static LPDIRECTDRAWCLIPPER	lpClipper;	   //ウィンドモードで使用するクリッパー	
static int DrawPointX,DrawPointY;		   //絵画位置(ウィンドモードのみで有効).	

//DirectXオブジェクト解放
#define RELEASEDD(a)  if( (a) ) { (a)->Release(); a = NULL;}      

//DDXの処理採用するってこと.
void DDXAdoption()
{
    lpDD         = NULL;	//念のため初期化
    lpDDSPrimary = NULL;	//これは、絶対に初期化しておいてください. (内部で最初の作成か作り直しかのチェックに使うから.)
	lpClipper    = NULL;	//これも絶対初期化すること.
	CommDraw = &CommDrawDDX;
}

//終了処理.
bool DDXInit(HWND hwnd,int w,int h,int bps)
{
	HRESULT			ret;
	NowScreenMode.fFullScreen = true; //DDX モードは、フルスクリーンで起動.
	NowScreenMode.iWidth      = w;
	NowScreenMode.iHeight     = h;
	NowScreenMode.iBpp        = bps;
	try
	{
		//DDオブジェクト作成
		ret = DirectDrawCreate( NULL, &lpDD, NULL);
		if( ret != DD_OK ) 
			throw (FriendlyError("DDXInit","DirectDrawオブジェクト作成",DDCreateFail(ret),"DDX起動は無理です"));
		if ( !DDXDoFullScreen(true) )
			throw (FriendlyError("DDXInit","DirectDrawフルスクリーン作成","こりぃは、DirectDraw起動は不可能です.","DDX起動は無理です"));
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//終了処理.
bool DDXEnd()
{
	//クリッパーくんの破棄
	RELEASEDD(lpClipper);
	//プライマリーちゃんを撲殺
	RELEASEDD( lpDDSPrimary );			

	//フルスクリーンのときはディスプレイモードを元に戻す.	
	if (NowScreenMode.fFullScreen)	lpDD->RestoreDisplayMode();
	//DirectDrawさんをめったざし.
	RELEASEDD( lpDD );					
	return true;
}

//フルスクーリン
bool DDXDoFullScreen(bool CoerceChange = false)
{
	//強行しない場合は、すでにその状態かチェック
	if (!CoerceChange)
		if (NowScreenMode.fFullScreen)	return false;	//すでにフルスクーリンです.

	HRESULT			ret;
	try
	{
	    //解像度変更できまっか?
		if ( ! FindOutDisplayModes() )
			throw		FriendlyError("DDXDoFullScreen","フルスクーリン設定",
						"このビデオカードorディスプレイは、この解像度をサポートしていません.",
						"ビデオカードなどをまともなものと交換してください");
		//ウィンドスタイル、大きさの変更.	
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, FULLWINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		//協調レベル設定
		ret = lpDD->SetCooperativeLevel(GameForm->GethWnd(),DDSCL_EXCLUSIVE |
										DDSCL_FULLSCREEN | 
										DDSCL_ALLOWREBOOT);
		if( ret != DD_OK )
			throw (FriendlyError("DDXInit","協調レベル設定",DDCreateFail(ret),"DDX起動は無理です"));
		//ディスプレイモード設定
		ret = lpDD->SetDisplayMode(NowScreenMode.iWidth , NowScreenMode.iHeight, NowScreenMode.iBpp);
		if( ret != DD_OK )
			throw (FriendlyError("DDXInit","ディスプレイモード設定",DDCreateFail(ret),"動作環境にディスプレイカードは対応してます?"));
		//クリッパーくんの破棄
		RELEASEDD(lpClipper);
		//プライマリーサーフェースの再作成.
		if ( !ReMakePrimarySurface() )
			throw 0;
		NowScreenMode.fFullScreen = true;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//ウィンドモード
bool DDXDoWindow(bool CoerceChange = false)
{
	//強行しない場合は、すでにその状態かチェック
	if (!CoerceChange)
		if (!NowScreenMode.fFullScreen)	return false;	//すでにウィンドモードです.
	HRESULT			ret;
	DDSURFACEDESC   ddsd;
	try
	{
		//ディスプレイモードを元に戻す.	
		ret = lpDD->RestoreDisplayMode();
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","ウィンドモードにするために画面を元に戻す設定",
							DDCreateFail(ret),"?");
		}
		//元のディスプレイモード取得
		ddsd.dwSize = sizeof(ddsd);	//絶対にこれを忘れるな!!! 死ぬぞ!
		ret = lpDD->GetDisplayMode(&ddsd);
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","現在の画面モード取得に失敗",DDCreateFail(ret),"?");
		}
		//このモードで Windowモードをできまっか?
		if (ddsd.ddpfPixelFormat.dwRGBBitCount != NowScreenMode.iBpp)
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","元の解像度で Windowモード動作をさせること",
					"元の解像度は、このアプリケーションを実行させるのための環境に適していません.","ウィンドモードはあきらめてください.");
		}
		//ウィンド関係の更新
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, WINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		//協調レベル設定
		ret = lpDD->SetCooperativeLevel(GameForm->GethWnd(),DDSCL_NORMAL);
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","協調レベル設定",DDCreateFail(ret),"Windowモード起動は無理"));
		}
		//プライマリーサーフェースの再作成.
		if ( !ReMakePrimarySurface() )
					throw 0;
		//クリッパーくんの更新.
		RELEASEDD(lpClipper);	//念のため現在のクリッパー君を始末.
		ret = lpDD->CreateClipper(0,&lpClipper,NULL);
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","クリッパー作成",DDCreateFail(ret),"Windowモード起動は無理"));
		}
		//目標ウィンドにクリッパー君を取り付ける.
		ret = lpClipper->SetHWnd(0,GameForm->GethWnd());
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","クリッパーをメインウィンドに設定すること",DDCreateFail(ret),"Windowモード起動は無理"));
		}
		//プライマリーにクリッパーを憑依させる.
		ret = lpDDSPrimary->SetClipper( lpClipper );
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","クリッパーをプライマーに設定すること",DDCreateFail(ret),"Windowモード起動は無理"));
		}
		NowScreenMode.fFullScreen = false;
        //GameForm->Align(ALIGN_MOVE_CENTER);
		GameForm->Move(100,100);
	}
	catch(...)
	{
		DDXDoFullScreen(true);	//強制的にフルスクーリンに戻す
		return false;
	}

	return true;
}

//ディスプレイモード列挙のコールバック.
HRESULT WINAPI EnumModesCallBack( DDSURFACEDESC *lpDDSurfaceDesc , void * lpContext)
{
	//探していたディスプレイモードかな?
	if (lpDDSurfaceDesc->dwWidth == NowScreenMode.iWidth &&
		lpDDSurfaceDesc->dwHeight == NowScreenMode.iHeight &&
		lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == NowScreenMode.iBpp)
	{
		//フルスクーリンで OK.
		*((BOOL*)(lpContext)) = TRUE;
		return DDENUMRET_CANCEL;
	}
	return DDENUMRET_OK;
}


//そのディスプレイモードが使えるかどうかチェック.
bool FindOutDisplayModes()
{
	DDSURFACEDESC DDSurfaceDesc = {0};
	BOOL retCheck = FALSE;
	//DDSURFACEDESC に探しているモードを設定.
	DDSurfaceDesc.dwSize  = sizeof(DDSURFACEDESC);
	DDSurfaceDesc.dwFlags = DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_WIDTH;
	DDSurfaceDesc.dwWidth = NowScreenMode.iWidth;
	DDSurfaceDesc.dwHeight= NowScreenMode.iHeight;
	DDSurfaceDesc.ddpfPixelFormat.dwRGBBitCount = NowScreenMode.iBpp;

	lpDD->EnumDisplayModes(DDEDM_REFRESHRATES , &DDSurfaceDesc , &retCheck , EnumModesCallBack)	;
	return (retCheck==TRUE);
}


//プライマリーサーフェースの作成.
bool ReMakePrimarySurface()
{
	HRESULT			ret;
	DDSURFACEDESC	ddsd;
	try
	{
		//今まで使ってきた、プライマリーちゃんを撲殺
		RELEASEDD( lpDDSPrimary );			
		//プライマリサーフェース作成
		memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;						//
		ddsd.dwBackBufferCount = 0;						//バックバッファ数
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;   //プライマリサーフェース指定
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary ,NULL);	//サーフェース作成
		if( ret != DD_OK ) 
			throw FriendlyError("DDXInit","プライマリサーフェース作成",DDCreateFail(ret),"なぜなんだぁー");
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//ビットマップの読み込み
bool DDXGraphicsMake(BMD puf,const char* filename)
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

	    hBit = (HBITMAP)LoadImage(NULL,filename,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作
	    if (!hBit) throw (FileNotOpen("GraphicsMake",filename) );

        GetObject(hBit, sizeof(BITMAP), &bmp_info);
        puf->w = bmp_info.bmWidth;
        puf->h = bmp_info.bmHeight;
    
	    SelectObject(memDC,hBit);   //ビットマップ選択

		//領域確保.
		if ( !DDXGraphicsAlloc(puf))
			 throw (FriendlyError("GraphicsMake","サーフェース作成","メモリ足りてますか?","メモリを増やしてください"));
		//領域確保.   
	    if ( !DDXGraphicsMakeSub(puf,memDC,hBit) )
			 throw (FriendlyError("GraphicsMake","画像をサーフェースに転送するの","?","?"));
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

//絵の転送
bool DDXGraphicsMakeSub(BMD bmd,HDC memDC,HBITMAP hBit)
{
	HDC hdc ;
	HRESULT			ret;
	IDirectDrawSurface* pdds = (IDirectDrawSurface*) bmd->Tag;	//DirectDraw オブジェクトの取得.
	try
	{
		//とりあえず、 DC を取得する.
		ret = pdds->GetDC(&hdc);
		if(ret != DD_OK) throw (FriendlyError("DDXGraphicsMakeSub","DC 取得",DDCreateFail(ret),""));
		//画像転送(手抜き～)
		BitBlt(hdc ,0,0,bmd->w,bmd->h, memDC,0,0,SRCCOPY);

		ret = pdds->ReleaseDC(hdc);
		if(ret != DD_OK) throw (FriendlyError("DDXGraphicsMakeSub","DC 返却",DDCreateFail(ret),""));

		unsigned char Pow2W;
		//2のべき数調べ.
		CheckPow2(&Pow2W,bmd->w);
		bmd->Pow2W = Pow2W;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//領域確保
bool DDXGraphicsAlloc(BMD bmd)
{
	HRESULT			ret;
	DDSURFACEDESC ddsd;			
	IDirectDrawSurface* pdds;	//サーフェースだもん
	DDCOLORKEY ddck;			//カラーキーだよ
	try
	{
		//サーフェースの作成.
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize=sizeof(ddsd);
		ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddsd.dwWidth=bmd->w;
		ddsd.dwHeight=bmd->h;
		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;
		ret = lpDD->CreateSurface(&ddsd,&pdds,NULL);
		if(ret != DD_OK)
			throw (FriendlyError("DDXGraphicsAlloc","サーフェース作成",DDCreateFail(ret),""));
		//カラーキーの設定
		ddck.dwColorSpaceLowValue = 0;		ddck.dwColorSpaceHighValue = 0;
		pdds->SetColorKey(DDCKEY_SRCBLT,&ddck);
	}
	catch(...)
	{
		return false;
	}
	//サーフェース仕方ないので、は Tag に記録しておきます.
	bmd->Tag = pdds;
	return true;						//大成功!! (^-^
}

//領域解放
bool DDXGraphicsFree(BMD bmd)
{
	return true;
}

//ロック
bool DDXGraphicsLock(BMD bmd)
{ 
	DDSURFACEDESC ddsd;
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));

	ddsd.dwSize = sizeof(ddsd);	//絶対にこれを忘れるな!!! 死ぬぞ!
	if (pdds->Lock(NULL,&ddsd,DDLOCK_WAIT,0) != DD_OK)
	{
		return false;
	}
	bmd->bm = (BYTE*)ddsd.lpSurface;
	return true;
}

//アンロック
bool DDXGraphicsUnLock(BMD bmd)
{
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->Unlock(bmd->bm) != DD_OK) return false;
	return true;
}

//強引に bmd に DC を作成する (^^;;;
HDC DDXGetDC(BMD bmd)
{
	HDC hdc;
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->GetDC(&hdc) != DD_OK) return NULL;

	return hdc	;						//メモリデバイスを返す.
}

//DC を消し去る. (DC に対する操作がフィードバックされる.)
bool DDXDeleteDC(BMD bmd,HDC hdc)
{
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->ReleaseDC(hdc) != DD_OK) return false;

	return true;
}
 

//DC->BMD の中身を更新する.
bool DDXUpdataDC(BMD bmd,HDC hdc)
{
	return true;
}


//現在の絵画モードの取得
UseDrawMode DDXGetMode()
{
	return USE_DIRECTDRAW_MODE;
}

//再絵画
void DDXRef(HWND hwnd)
{
	//プライマリーちゃんが ロスト☆ユニ以下略 していたら、
	//ゾンビのように大復活!!
	if(lpDDSPrimary->IsLost() == DDERR_SURFACELOST)
		lpDDSPrimary->Restore();		//大復活!
	//プライマリーちゃんに直接ぶっこく.
	if (NowScreenMode.fFullScreen)
		lpDDSPrimary->BltFast(0,0,
				((IDirectDrawSurface*)(Sprite->Tag)),NULL,DDBLTFAST_NOCOLORKEY);
	else
	{	//ウィンドモード  クソバカな仕様により、 BitFast は、使えない.
		RECT DestRc;
		DDBLTFX ddbltfx = {0};
		ddbltfx.dwSize = sizeof(ddbltfx);
		DestRc.left  = DrawPointX;
		DestRc.top   = DrawPointY;
		DestRc.right = DrawPointX + NowScreenMode.iWidth;
		DestRc.bottom= DrawPointY + NowScreenMode.iHeight;
		lpDDSPrimary->Blt(&DestRc,
				((IDirectDrawSurface*)(Sprite->Tag)),NULL,DDBLTFAST_NOCOLORKEY,&ddbltfx);
	}
}

//絵画ポイントの設定(ウィンドモードのみに有効です.)
void DDXSetDrawPoint(int x,int y)
{
	//ウィンドモードの場合は、そのパラメータの値になる.
	DrawPointX = x;	DrawPointY = y;
}	
