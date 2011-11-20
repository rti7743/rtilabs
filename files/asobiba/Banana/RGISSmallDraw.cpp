// RGISSmallDraw.cpp: RGISSmallDraw クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallDraw.h"
#include "RDebug.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


void RGISSmallDraw::Polygon(const LPOINT * inBlockStart,const char* inCVData , int CVCount , int inSize , COLORREF16 inRGB ) const
{
//		static nnn = 0;
//		Draw->DebugSave("test/snap" + num2str(nnn++) + ".bmp");


	static LPOINT point[256]; 
	static POINT  Npoint[256]; 
	int i;
	ASSERT(CVCount < 256);

	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	RStdioMemory memory;
	memory.Open(inCVData);

	int x,y;
	x = memory.readGetUS();
	y = memory.readGetUS();

	point[0].x = inBlockStart->x + x;
	point[0].y = inBlockStart->y + y;
	for(i = 1; i < CVCount ; i++)
	{
		x += memory.readGetC();
		y += memory.readGetC();
		point[i].x = inBlockStart->x + x;
		point[i].y = inBlockStart->y + y;
	}
	//一点でも画面内に現れる場合は、そのラインを描画します。
	for(i = 0; i < CVCount ; i++)
	{
		if (point[i].x >= GlobalDisplay->left && point[i].x <= GlobalDisplay->right && 
			point[i].y >= GlobalDisplay->top && point[i].y <= GlobalDisplay->bottom  )
		{
			//描画の必要あり.
			break;
		}
	}

	if (i != CVCount)
	{
		Npoint[0] = Effect->Attach( &point[0] );
		for(i = 1; i < CVCount ; i++)
		{
			Npoint[i] = Effect->Attach( &point[i] );
			Draw->Line( Npoint[i - 1] , Npoint[i] , inRGB,inSize);
		}
	}
}



void RGISSmallDraw::TextOut(const LPOINT * inBlockStart,const char* inCVData , int CVCount ,const char * inName , RGISSmallDupKill * ioDupKill) const
{
	//文字列がない場合は没.
	if (inName[0] == '\0') return ;

	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	RStdioMemory memory;
	memory.Open(inCVData);

	static LPOINT point ;
	static POINT  Npoint; 

	int x,y;
	x = memory.readGetUS();
	y = memory.readGetUS();

	for(int i = 1; i < CVCount/2 ; i++)
	{
		x += memory.readGetC();
		y += memory.readGetC();
	}
	point.x = inBlockStart->x + x;
	point.y = inBlockStart->y + y;

	if (
		point.x >= GlobalDisplay->left && point.x <= GlobalDisplay->right && 
		point.y >= GlobalDisplay->top && point.y <= GlobalDisplay->bottom  )
	{
		Npoint = Effect->Attach( &point );
		//初めて表示するやつだったら表示する.
		if ( ioDupKill->Uniq(inName) )
		{
			Draw->TextOut(Npoint, inName );
		}
	}
}


void RGISSmallDraw::Paint(const LPOINT * inBlockStart,const char* inCVData , int CVCount , COLORREF16 inRGB) const
{
	static LPOINT point[256]; 
	static POINT  Npoint[256]; 
	int i;
	ASSERT(CVCount < 256);

	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	RStdioMemory memory;
	memory.Open(inCVData);

	int x,y;
	x = memory.readGetUS();
	y = memory.readGetUS();

	point[0].x = inBlockStart->x + x;
	point[0].y = inBlockStart->y + y;
	for(i = 1; i < CVCount ; i++)
	{
		x += memory.readGetC();
		y += memory.readGetC();
		point[i].x = inBlockStart->x + x;
		point[i].y = inBlockStart->y + y;
	}
	//一点でも画面内に現れる場合は、そのラインを描画します。
	for(i = 0; i < CVCount ; i++)
	{
		if (point[i].x >= GlobalDisplay->left && point[i].x <= GlobalDisplay->right && 
			point[i].y >= GlobalDisplay->top && point[i].y <= GlobalDisplay->bottom  )
		{
			//描画の必要あり.
			break;
		}
	}

	if (i != CVCount)
	{
		Npoint[0] = Effect->Attach( &point[0] );

		POINT move;
		POINT hoPoint;
		POINT midPoint;
		for(i = 1; i < CVCount ; i++)
		{
			Npoint[i] = Effect->Attach( &point[i] );

			move.x = (Npoint[i].x - Npoint[i - 1].x);
			move.y = (Npoint[i].y - Npoint[i - 1].y);

			if ( FastHypot(move.x , move.y) < 3) continue ;

			//中点
			midPoint.x = Npoint[i - 1].x + (move.x)/2;
			midPoint.y = Npoint[i - 1].y + (move.y)/2;

			//垂直に交わる式.
			POINT move2 = {0};
			POINT t;
			if (abs(move.y) > abs(move.x))
			{
				if (move.x == 0)
				{
					t.x = 0;
					t.y = 1;
				}
				else
				{
					t.x = 0;
					t.y = 1;
				}
			}
			else
			{
				if (move.y == 0)
				{
					t.x = 1;
					t.y = 0;
				}
				else
				{
					t.x = 1;
					t.y = 0;
				}
			}
			hoPoint.x = midPoint.x;
			hoPoint.y = midPoint.y;
			move2.x = t.x * (move.x > 0 ? 1 : -1 );
			move2.y = t.y * (move.y > 0 ? 1 : -1 );
			bool donotDraw = false;
			do{
				hoPoint.x -= move2.y;
				hoPoint.y += move2.x;

				if (hoPoint.x < 0 || hoPoint.x >= Draw->getWidth() ||
					hoPoint.y < 0 || hoPoint.y >= Draw->getHeight() )
				{
					donotDraw = true;
					break ;
				}
				if (Draw->getPixel(hoPoint.x , hoPoint.y ) == inRGB)
				{
					donotDraw = true;
					break ;
				}
			}
			while( IsLineOn(&Npoint[i - 1] , &hoPoint , &Npoint[i] ) );

			if (!donotDraw)
			{
				Draw->Paint(hoPoint ,inRGB , inRGB);
				Draw->setPixel(hoPoint.x , hoPoint.y , RGB24TOBGB16(RGB(255,0,0)) );
			}
		}
	}
}

void RGISSmallDraw::Icon(const LPOINT * inXY, int inBitmapNo ,COLORREF16 inRGB,  const char * inName) const
{
	//文字列がない場合は没.
	if (inName[0] == '\0') return ;

	static POINT Npoint; 
	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	if (
		inXY->x >= GlobalDisplay->left && inXY->x <= GlobalDisplay->right && 
		inXY->y >= GlobalDisplay->top && inXY->y <= GlobalDisplay->bottom  )
	{
		Npoint = Effect->Attach( inXY );
		Draw->setMark(Npoint , inBitmapNo );

		if (inName != NULL)
		{
			Npoint.x += 16;
			Draw->TextOut(Npoint , inName );
		}
	}
}

void RGISSmallDraw::TranslucenceBox(const LPOINT * inXY,unsigned char inSize , unsigned char inColor)
{
	static POINT Npoint; 
	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	if (
		inXY->x >= GlobalDisplay->left && inXY->x <= GlobalDisplay->right && 
		inXY->y >= GlobalDisplay->top && inXY->y <= GlobalDisplay->bottom  )
	{
		Npoint = Effect->Attach( inXY );
		Draw->TranslucenceBox(Npoint , inSize ,  inColor);
	}
}

void RGISSmallDraw::Icon(const LPOINT * inBlockStart ,const SPOINT * inXY,int inBitmapNo ,COLORREF16 inRGB, const char * inName) const
{
	//文字列がない場合は没.
	if (inName[0] == '\0') return ;

	static LPOINT point; 
	static POINT Npoint; 
	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	point.x = inBlockStart->x + inXY->x;
	point.y = inBlockStart->y + inXY->y;

	if (
		point.x >= GlobalDisplay->left && point.x <= GlobalDisplay->right && 
		point.y >= GlobalDisplay->top && point.y <= GlobalDisplay->bottom  )
	{
		Npoint = Effect->Attach( &point );
		Draw->setMark(Npoint , inBitmapNo );

		Npoint.x += 16;
		Draw->TextOut(Npoint , inName );
	}
}

void RGISSmallDraw::TextOut(const LPOINT * inBlockStart ,const SPOINT * inXY,COLORREF16 inRGB, const char * inName) const
{
	//文字列がない場合は没.
	if (inName[0] == '\0') return ;

	static LPOINT point; 
	static POINT Npoint; 
	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();

	point.x = inBlockStart->x + inXY->x;
	point.y = inBlockStart->y + inXY->y;

	if (
		point.x >= GlobalDisplay->left && point.x <= GlobalDisplay->right && 
		point.y >= GlobalDisplay->top && point.y <= GlobalDisplay->bottom  )
	{
		Npoint = Effect->Attach( &point );
		Draw->TextOut(Npoint , inName );
	}
}
void RGISSmallDraw::Point(const LPOINT * inBlockStart ,const SPOINT * inXY, int inSize , COLORREF16 inRGB , unsigned long inSuji) const
{
	static LPOINT point; 
	static POINT Npoint; 

	const LRECT * GlobalDisplay = Effect->getGlobalDrawDisplay();
	point.x = inBlockStart->x + inXY->x;
	point.y = inBlockStart->y + inXY->y;

	if (
		point.x >= GlobalDisplay->left && point.x <= GlobalDisplay->right && 
		point.y >= GlobalDisplay->top && point.y <= GlobalDisplay->bottom  )
	{
//		RDrawPen pen(inSize,inRGB);

		Npoint = Effect->Attach( &point );
		Draw->SujiOut(Npoint , inSuji );
	}
}

//inSrc と inDest の2点を通る直線上に inTarget がありますか?
bool RGISSmallDraw::IsLineOn(const POINT * inSrc , const POINT * inTarget , const POINT * inDest ) const
{
	if (inSrc->y == inDest->y)
	{
		if (inSrc->y == inTarget->y)
		{
			return true;
		}
	}
	else if (inSrc->x == inDest->x)
	{
		if (inSrc->x == inTarget->x)
		{
			return true;
		}
	}
	else
	{
		int x = (inTarget->y - inSrc->y) * (inDest->x - inSrc->x) / (inDest->y - inSrc->y)  + inSrc->x;
		if (inTarget->x - 3 >= x && inTarget->x + 3 <= x)	//3は適当な許容範囲.
		{
			return true;
		}
	}
	return false;
}


void RGISSmallDraw::Boxes( unsigned char * inLevel    ,const LPOINT * inBlockStart,const unsigned short * inPallet) const
{
	Draw->Boxes(Effect, inLevel    , inBlockStart, inPallet) ;
}



void RGISSmallDraw::test()
{
}
