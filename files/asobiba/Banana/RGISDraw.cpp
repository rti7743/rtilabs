// RGISDraw.cpp: RGISDraw クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISDraw.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

void RGISDraw::Polygon(const GISINTArray* inArray,int inSize , COLORREF inRGB) const
{
	/*
	static UPOINT point[2048]; 
	int i;
	int count = inArray->getCount();
	ASSERT(count < 2048);

	//一点でも画面内に現れる場合は、そのラインを描画します。
	bool beDraw = false;
	for(i = 0; i < count ; i++)
	{
		point[i] = Slp->getPoint(inArray->getPoint(i) - 1);
//		point[i].x = CLIPX(point[i].x) ;		point[i].y = CLIPY(point[i].y) ;
		if (beDraw == false && 
			point[i].x >= Display->left && point[i].x <= Display->right && 
			point[i].y >= Display->top && point[i].y <= Display->bottom  )
		{
			//描画の必要あり.
			beDraw = true;
		}
	}

	if (beDraw)
	{
		RDrawPen pen(inSize,inRGB);

		point[0].x -= Display->left ;		point[0].y -= Display->top ;
		Draw->MoveTo( point[0] ,&pen);
		for(i = 1; i < count ; i++)
		{
			point[i].x -= Display->left ;		point[i].y -= Display->top ;
			Draw->LineTo( point[i] );
		}
	}
	*/
}

void RGISDraw::Polygon(const RGIS2500PointArray* inArray,int inSize , COLORREF inRGB) const
{
	/*
	static UPOINT point[2048]; 
	int i;
	int count = inArray->getCount();
	ASSERT(count < 2048);

	//一点でも画面内に現れる場合は、そのラインを描画します。
	bool beDraw = false;
	for(i = 0; i < count ; i++)
	{
		point[i] = inArray->getPoint(i);

		if (beDraw == false && 
			point[i].x >= Display->left && point[i].x <= Display->right && 
			point[i].y >= Display->top && point[i].y <= Display->bottom  )
		{
			//描画の必要あり.
			beDraw = true;
		}
	}
			beDraw = true;

	if (beDraw)
	{
		RDrawPen pen(inSize,inRGB);

		point[0].x -= Display->left ;		point[0].y -= Display->top ;

//		TRACE("x,y %d,%d\n" , point[0].x , point[0].y);

		Draw->MoveTo( point[0] ,&pen);
		for(i = 1; i < count ; i++)
		{
			point[i].x -= Display->left ;		point[i].y -= Display->top ;

//			TRACE("x,y %d,%d\n" , point[i].x , point[i].y);

			Draw->LineTo( point[i] );
		}
	}
	*/
}

void RGISDraw::TexOut(UPOINT * inXY,const string & inStr , int inSize , COLORREF inRGB) const
{
	/*
	static UPOINT point; 

	if (
		inXY->x >= Display->left && inXY->x <= Display->right && 
		inXY->y >= Display->top  && inXY->y <= Display->bottom  )
	{
		RDrawPen pen(inSize,inRGB);

		point.x = inXY->x - Display->left ;		point.y = inXY->y - Display->top ;
//		Draw->TextOut(point , inStr , &pen);
	}
	*/
	 
}



