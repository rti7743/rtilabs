
#include <windows.h>
#include <.\VCL\rti_vcl_base.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_debug.h>

#include <.\VCL\rti_vcl_form.h>

/*******************************************************************/
//コンストラクタ
/*******************************************************************/
TBaseObject::TBaseObject()
{
    //イベント
    OnMouseDown     = NULL;
    OnMouseUp       = NULL;
    OnMouseMove     = NULL;
    OnKeyDown       = NULL;
    OnKeyUp         = NULL;
    OnEnterFocus    = NULL;
    OnExitFocus     = NULL;
    OnClose         = NULL;
#ifdef RGN_LIFE
    //リージョン
    Rgn = NULL;
#endif
}

/*******************************************************************/
//デストラクタ
/*******************************************************************/
TBaseObject::~TBaseObject()
{
#ifdef RGN_LIFE
    //リージョン破棄
    delete Rgn;
#endif
}

/*******************************************************************/
//サイズ調整
/*******************************************************************/
void TBaseObject::Align(int iFlg)
{
	HWND BosshWnd;

	if (!hMyOwner)       BosshWnd = GetDesktopWindow();
	else                 BosshWnd = (HWND)GethOwerWnd(); 

	RECT CliantRcect;
	RECT MyRcect;
	RECT CliantCRcect;
	RECT MyCRcect;

	if (!GetWindowRect(BosshWnd,&CliantRcect)) return;
	if (!GetClientRect(BosshWnd,&CliantCRcect)) return;
	if (!GetWindowRect(GethWnd(),&MyRcect)) return;
	if (!GetClientRect(GethWnd(),&MyCRcect)) return;

	switch(iFlg)
	{
	case ALIGN_TOP:	//トップに引き伸ばしあわせ
		MoveWindow(GethWnd(),0,0,
						CliantRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_DOWN:		//ダウンに引き伸ばしあわせ
		MoveWindow(GethWnd(),0,CliantCRcect.bottom-MyCRcect.bottom,
						CliantRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_LEFT:		//レフトに引き伸ばしあわせ
		MoveWindow(GethWnd(),0,0,
						MyCRcect.right,CliantCRcect.bottom,TRUE);
		break;
	case ALIGN_RIGHT:		//ライトに引き伸ばしあわせ
		MoveWindow(GethWnd(),CliantCRcect.right-MyCRcect.right,0,
						MyCRcect.right,CliantCRcect.bottom,TRUE);
		break;
    case ALIGN_CLIANT:      //クライアントにあわせる
		MoveWindow(GethWnd(),0,0,
						CliantCRcect.right,CliantCRcect.bottom,TRUE);
        break;
	case ALIGN_MOVE_TOP:	//トップに動かす
		MoveWindow(GethWnd(),MyRcect.left,0,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_DOWN:	//ダウンに動かす
		MoveWindow(GethWnd(),MyRcect.left,CliantCRcect.bottom-MyCRcect.bottom,
						MyCRcect.right,MyRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_LEFT:	//レフトに動かす
		MoveWindow(GethWnd(),0,MyRcect.top,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_RIGHT:	//ライトに動かす
		MoveWindow(GethWnd(),CliantCRcect.right-MyCRcect.right,MyRcect.top,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
    case ALIGN_MOVE_CENTER:	//中央に持ってくる
		MoveWindow(GethWnd(),(CliantCRcect.right-MyCRcect.right)/2,
                        (CliantCRcect.bottom-MyCRcect.bottom)/2,
						MyCRcect.right,MyCRcect.bottom,TRUE);
        break;
	}
}

/*******************************************************************/
//指定の位置に移動
/*******************************************************************/
void TBaseObject::Move(int x,int y)
{
    RECT rt;
    GetWindowRect(GethWnd(),&rt);
    MoveWindow(GethWnd(),x,y,rt.right-rt.left,rt.bottom-rt.top,TRUE);
};

/*******************************************************************/
//遺伝
/*******************************************************************/
void TBaseObject::Jeneric(unsigned char kind)
{
    ObjectIT it;
    TBaseObject * TBO;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            TBO = ((TBaseObject*)(*it));
//            switch(kind)
//            {
//            }
            //再帰で詩文の子分まですべて汚染(ぉ します. チリチリチリチリ..
            TBO->Jeneric(kind);
            TBO->RefControl();
        }
};

/*******************************************************************/
//リージョン
/*******************************************************************/
void TBaseObject::SetRgn(char *filename,char flg)
{
#ifdef RGN_LIFE
    Rgn = new TRgn(hWnd,filename,flg);
#endif
}

/*******************************************************************/
//ディフォルト のるーちんだぴょーん
/*******************************************************************/
LRESULT CALLBACK TBaseObject::StaticProc(HWND hwnd , UINT message,
						 WPARAM wParam,LPARAM lParam)
{

	TBaseObject *Me = ((TBaseObject*)GetWindowLong(hwnd, GWL_USERDATA));
	if (message==WM_NCDESTROY)
		return 0;
	if (Me)
		return Me->UserDisPacth(hwnd ,message, wParam,lParam);
	else
		return DefWindowProc(hwnd,message,wParam,lParam);
}

/*******************************************************************/
//基本イベント のるーちんだぴょーん
/*******************************************************************/
LRESULT TBaseObject::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
	TObject *Obj;
	switch(message)
	{
	case WM_COMMAND:
		//メッセージを送ってきたオブジェクト君を調べる
		Obj = ((TObject*)
			GetWindowLong((HWND)lParam, GWL_USERDATA));
		//コマンドイベント発動
		if (!Obj) break;
		Obj->WmCommandCome(wParam);
		break;
    case WM_DESTROY:
        UserDestroy();
        return 0;
        break;
    case WM_KEYDOWN:    //キーダウン
        if (OnKeyDown) OnKeyDown(this,(int) wParam,(UINT)lParam);
        break;
    case WM_KEYUP:     //キーアップ
        if (OnKeyUp) OnKeyUp(this,(int) wParam,(UINT)lParam);
        break;
    case WM_SETFOCUS:   //フォーカスを受け取った
        if (OnEnterFocus) OnEnterFocus(this);
        break;
    case WM_KILLFOCUS:  //フォーカスを手放した
        if (OnExitFocus) OnExitFocus(this);
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN: //ボタンダウン
        //透明エリアにマウスダウンを伝える
        if (OnMouseDown) OnMouseDown(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: //ボタンアップ
        if (OnMouseUp) OnMouseUp(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_MOUSEMOVE: //マウスムーブ
        if (OnMouseMove) OnMouseMove(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_CLOSE: //クローズ
        if (OnClose) 
        {
            if (!OnClose(this))  return 0;
        }
 	default:
		if (Kind != OBJECT_KIND_FORM)
		    return (CallWindowProc( (WNDPROC)GetDefProc(), hwnd, message,wParam,lParam));
//		    return (CallWindowProc( (int (__stdcall *)(void))GetDefProc(), hwnd, message,wParam,lParam));
		else
	        return Application->DefProc(hwnd,message,wParam,lParam);
	}
	if (Kind != OBJECT_KIND_FORM)
	    return (CallWindowProc( (WNDPROC)GetDefProc(), hwnd, message,wParam,lParam));
//	    return (CallWindowProc( (int (__stdcall *)(void))GetDefProc(), hwnd, message,wParam,lParam));
	else
		return 0;
}
