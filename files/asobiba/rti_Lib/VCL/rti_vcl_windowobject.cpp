
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_windowobject.h>

TWinObject::TWinObject()
{
}

/**********************************************************/
// コンポーネント作成
/**********************************************************/
bool TWinObject::CreateObject(TObject* hOwner,char *Caption,DWORD dwStyle,
                       int x,int y,int w,int h,bool NULLid,WNDPROC WndPorc)
{
    HWND whOnwner;

    if (!hOwner)	whOnwner = NULL ;
    else
	{ 
		whOnwner = hOwner->GethWnd();
		dwStyle = dwStyle | WS_CHILD;//オーナーがあるので子ウィンドですね (^^;;
	}

    if (NULLid)	Id = 0;
    else        Id = Application->GetId();

	//ウィンド作成
    hWnd = CreateWindowEx(
        NULL,
		ClassName,             //クラス名
		Caption,                     //ウィンドタイトル
		dwStyle,                     //ウィンドスタイル
		x,                           //ウィンドの左上X座標
		y,                           //ウィンドの左上Y座標
		w,                           //幅
		h,                           //高さ
		whOnwner,                    //親ウィンドのハンドル
		(HMENU)Id,               //?のハンドル
		Application->GethInstance(),      //インスタンスハンドル
		NULL                         //拡張用のパラメータ
		);
    //ウィンド作成に失敗
    if (!hWnd) return false;
    //自分の初期化
	InitComponent();

    //VCLで使用するためにサブクラス化する
    CreateSubClass(WndPorc);

	//メインフォームのウィンドハンドルゲットだぜー
    if (!Application->hWnd) Application->hWnd =  hWnd;
    return true;

}

/**********************************************************/
//初期化
/**********************************************************/
void TWinObject::InitComponent()
{
    //ユーザ定義の裏技利用
    SetWindowLong( GethWnd() , GWL_USERDATA, (LONG)this);

}

/**********************************************************/
/* オリジナルクラス作成*/
/**********************************************************/
bool TWinObject::CreateOlignalClass(WNDPROC WndPorc)
{
	static int ClassNameunick = 0;
	WNDCLASS  wcls;     //クラス定義用の構造体
    wsprintf(ClassName,"_FORM%d",ClassNameunick++);

	//ウィンドクラス定義部
	wcls.hInstance = Application->GethInstance();         //インスタンスハンドル
	wcls.lpszClassName = ClassName;//クラス名
	wcls.lpfnWndProc = WndPorc;           //ウィンドプロシージャ
    wcls.style = CS_BYTEALIGNCLIENT;                     //クラス名
	wcls.hIcon = LoadIcon(NULL,IDI_APPLICATION); //アイコンハンドル
	wcls.hCursor = LoadCursor(NULL,IDC_ARROW);   //カーソルハンドル
	wcls.lpszMenuName = NULL;                    //メニュー
	wcls.cbClsExtra = 0;                         //拡張
	wcls.cbWndExtra = 0;                         //拡張

	wcls.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH	);

	//ウィンドクラス制作部
	if (!RegisterClass(&wcls) ) 
    {
        return false; //ウィンド制作に失敗
    }
    return true;

}

/**********************************************************/
/* サブクラス化を実行*/
/**********************************************************/
void TWinObject::CreateSubClass(WNDPROC WndPorc)
{
    // 元々のスタティックウィンドウプロシージャの保存
	True_StaticProc = (FARPROC)GetWindowLong( GethWnd() , GWL_WNDPROC);
	// スタティックウィンドウのサブクラス化
	SetWindowLong( GethWnd() , GWL_WNDPROC, (LONG)WndPorc);
}

/**********************************************************/
/* サブクラス化を解除*/
/**********************************************************/
void TWinObject::DestroySubClass()
{
    if (IsWindow( GethWnd() ))
        SetWindowLong(  GethWnd()  , GWL_WNDPROC, (LONG)True_StaticProc);
}

/**********************************************************/
/* コンポーネント削除の通常処理*/
/**********************************************************/
void TWinObject::DestroyComponent()
{
    //壊れたオブジェクトだったら無視.
    if (Kind == OBJECT_KIND_BAD) return;
    OwerKill        = true;
	//サブクラス化解除する.
	DestroySubClass();
    //自分の持っていたすべてのコントロールを閉じる
    ChildDestroy();
    //自分のオーナーに自分が消えることを通知
    IamDestroy();
    //ユーザ定義の裏技利用の解除
    if (IsWindow( GethWnd() ) )
        SetWindowLong( GethWnd() , GWL_USERDATA, (LONG)0);
}
