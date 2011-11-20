#include "StartWithExeFile.h"
#ifdef DRAW_CANVAUS_LIFE

#ifndef ___DRAWCANVAUSH
#define ___DRAWCANVAUSH

/****************************************************************/
//自由絵画
/****************************************************************/
#include <.\Graphics\rti_graphics.h>
class TDrawCanvaus
{
	BMD	Canvaus;	//キャンバス
	int	Size;		//筆のサイズ
	int SizeHarf;
	int	Color;		//筆のいろ
	int	Es;			//消しゴムのいろ
public:
	//コンストラクタ
	TDrawCanvaus(){};
	//キャンバス実行
	void Draw(BMD TarGet,int x,int y);
	//キャンバス設定
	void SetCanvaus(BMD c){Canvaus = c;};
	//筆のサイズ設定
	void SetSize(int c){Size = c; SizeHarf = c >> 1; };
	//筆のいろ設定
	void SetColor(int c){Color = c;};
	//消しゴムのいろ設定
	void SetEs(int c){Es = c;};
	BMD GetCanvaus(){return Canvaus;};
};

#endif

#endif  //LIFE END
