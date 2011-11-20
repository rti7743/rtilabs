#include "StartWithExeFile.h"
#ifdef DRAW_CANVAUS_LIFE

/****************************************************************/
//自由絵画
/****************************************************************/
#include <.\Graphics\rti_DrawCanvaus.h>
#include <.\Base\rti_key.h>

//キャンバス実行
void TDrawCanvaus::Draw(BMD TarGet,int x,int y)
{
	int DX,DY;
	DX = MouseRX - x;
	DY = MouseRY - y;
	if (DX >= 0 && DX <= Canvaus->w && DY >= 0 && DY <= Canvaus->h)
	{
		if (MouseEL)
		{
			Box(Canvaus,DX-SizeHarf,DY-SizeHarf,DX+SizeHarf,DY+SizeHarf,Color);
		}
	}
	//キャンバスの絵画.
	::DirectDraw(Canvaus,0,0,Canvaus->w,Canvaus->h,TarGet,x,y);
}


#endif
