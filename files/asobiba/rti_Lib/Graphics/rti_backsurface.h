#include "StartWithExeFile.h"
#ifdef BACKSURFACE_LIFE

#ifndef ___BACKSURFACEH
#define ___BACKSURFACEH
#include <.\Graphics\rti_graphics.h>
#include <.\Comm\rti_Comm.h>

extern BMD Sprite;

/*****************************************************************/
//裏画面の元になるやつ.	規格依存するやつら.
/*****************************************************************/
class	TBacksurfaceBase
{
protected:
	virtual	void Make() = 0;					//作成
	virtual	void Destroy() = 0;					//破棄
	virtual	void Catch_WM_PAINT() = 0;			//WM_PAINT のときの処理.
	virtual	void Catch_WM_MOVE(int x,int y) = 0;//WM_MOVE のときの処理.
};

/*****************************************************************/
//同じく裏画面の元になる	ただし、こっちは共有メソッド
/*****************************************************************/
class	TBacksurfaceComm : public TBacksurfaceBase
{
protected:
	HWND	hWnd;
	void CommMake(HWND hwnd);
public:
	void CommDestroy();
    void Cls(){ RtiGraphics::Cls(Sprite,0);};	//画面消去.
	void Ref(){	CommDraw->Ref(hWnd);};	//リフレッシュ
	void OnPaint(){Catch_WM_PAINT();};	//WM_PAINT のとき.
	void OnMove (int lParam)
		{Catch_WM_MOVE( (int) LOWORD(lParam),(int) HIWORD(lParam) );};	//WM_MOVE  のとき.
};

/*****************************************************************/
//GDI レベルの処理.
/*****************************************************************/
class	TGDIBackSurface : public TBacksurfaceComm
{
	void Make(){};			//作成
	void Destroy(){};			//破棄
	void Catch_WM_PAINT();				//WM_PAINT のときの処理.
	void Catch_WM_MOVE(int x,int y);	//WM_MOVE のときの処理.
public:
	TGDIBackSurface(HWND hwnd){CommMake(hwnd);};
};

/*****************************************************************/
//DirectDraw レベルの処理.
/*****************************************************************/
class	TDirectDrawBackSurface : public TBacksurfaceComm
{
	void Make(){};			//作成
	void Destroy(){};			//破棄
	void Catch_WM_PAINT();				//WM_PAINT のときの処理.
	void Catch_WM_MOVE(int x,int y);	//WM_MOVE のときの処理.
public:
	TDirectDrawBackSurface(HWND hwnd){CommMake(hwnd);};
};

/*****************************************************************/
//裏画面作成ルーチン.
//しかし、ユーザはこのクラスだけを操作するだけでよい.
//このクラスは上記のクラスをアダプターしてくれるのであるあるあるあるあるあるあるあるあるある
//		(C)Copylight	2000 1/24	R.T.I 
/*****************************************************************/
class TBacksurface
{
	TBacksurfaceComm *BacksurfaceRtn;
public:
	TBacksurface(HWND hwnd)
	{
		switch(CommDraw->GetMode())
		{
		case USE_GDI_MODE:
			BacksurfaceRtn = new TGDIBackSurface(hwnd);
			break;
		case USE_DIRECTDRAW_MODE:
			BacksurfaceRtn = new TDirectDrawBackSurface(hwnd);
			break;
		}
	};
	~TBacksurface()
    {
        BacksurfaceRtn->CommDestroy();
        delete BacksurfaceRtn;
    };

	void Cls(){BacksurfaceRtn->Cls();};			//クリア
	void Ref(){BacksurfaceRtn->Ref();};			//リフレッシュ

	void OnPaint(){BacksurfaceRtn->OnPaint();};	//ペイントのときの処理.
	void OnMove(int lParam){BacksurfaceRtn->OnMove(lParam);};	//ムーブのときの処理.
};


extern TBacksurface *Backsurface;

#endif

#endif //LIFE END
