#ifndef TBASEOBJECTH
#define TBASEOBJECTH

#include <.\VCL\rti_vcl_windowobject.h>
#include <.\Base\rti_rgn.h>
#include <windows.h>

#define ALIGN_TOP			0//トップに引き伸ばしあわせ
#define ALIGN_DOWN			1//ダウンに引き伸ばしあわせ
#define ALIGN_LEFT			2//レフトに引き伸ばしあわせ
#define ALIGN_RIGHT			3//ライトに引き伸ばしあわせ
#define ALIGN_CLIANT		4//クライアントの大きさにあわせる
#define ALIGN_MOVE_TOP		6//トップに動かす
#define ALIGN_MOVE_DOWN		7//ダウンに動かす
#define ALIGN_MOVE_LEFT		8//レフトに動かす
#define ALIGN_MOVE_RIGHT	9//ライトに動かす
#define ALIGN_MOVE_CENTER	10//中央に持ってくる

#define JENERIC_FONT_CHANGE 0   //フォントを遺伝

class TBaseObject : public TWinObject 
{
#ifdef RGN_LIFE
    TRgn *Rgn;
#endif
public:
	//ディフォルト
	static LRESULT CALLBACK StaticProc
		(HWND hWnd , UINT message,WPARAM wParam,LPARAM lParam);
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hWnd , UINT message,WPARAM wParam,LPARAM lParam);
    //連続配置が必要なコンポーネントのコントロールの再配置を促す
    virtual void RefControl() {};
 
    TBaseObject();
    ~TBaseObject();
    void SetRgn(char *filename,char flg);

	void Align(int iFlg);               //位置あわせ
    void Jeneric(unsigned char kind);   //遺伝
    HDC SpGetDC();                      //今までの経過をすべて集約したDC の作成

    //マウスが押された
    MouseEvent  OnMouseDown;
    MouseEvent  OnMouseUp;
    MouseEvent  OnMouseMove;
    //キーが押された
    KeyEvent    OnKeyDown;
    KeyEvent    OnKeyUp;
    //フォーカス
    SimpleEvent OnEnterFocus;
    SimpleEvent OnExitFocus;
    //終了していいの? true 終了  / false まだ.
    BoolEvent OnClose;

	//そのオブジェクトをはき
	void Destroy(){DestroyWindow((HWND)hWnd);};
	//そのオブジェクトを表示する
	void Show() {ShowWindow((HWND)hWnd,SW_SHOW);};
	//そのオブジェクトを隠す
	void Hide() {ShowWindow((HWND)hWnd,SW_HIDE);};
	//そのオブジェクトを有効にする
	void Enabled() {EnableWindow((HWND)hWnd,TRUE);};
	//そのオブジェクトを無効にする
	void UnEnabled() {EnableWindow((HWND)hWnd,FALSE);};
	//そのオブジェクトを移動させる
	void Move(int x,int y,int w,int h)
				{MoveWindow((HWND)hWnd,x,y,w,h,TRUE);};
	//そのオブジェクトを移動させる
	void Move(int x,int y);

	//そのオブジェクトのキャプションを得る
	void GetCaption(char *buf,int buf_size)
				{GetWindowText((HWND)hWnd, buf, buf_size);};
	//そのオブジェクトのキャプションをセットする
	void SetCaption(char *buf)
				{SetWindowText((HWND)hWnd, buf);};

};

#endif

