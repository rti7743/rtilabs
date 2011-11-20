// RGISCvLight.cpp: RGISCvLight クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISCvLight.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISCvLight::RGISCvLight()
{

}

RGISCvLight::~RGISCvLight()
{

}

//このノード郡に自分は含まれていますか?
bool RGISCvLight::isNode(	const RGISNode * inNode , const RGISNodeIntArray * inNodeArray ,
							const RGISSlp * inSlp , const UPOINT srcPoint ) const
{
	int nodeArrayCount = inNodeArray->getCount();
	for(int n = 0 ; n < nodeArrayCount ; n++)
	{
		int nodeAS = inNodeArray->getPoint(n) ;
		int pt = inNode->getPoint( nodeAS - 1)->PT;
		UPOINT destPoint = inSlp->getPoint(pt  - 1 );

		//ノードに記載されていた地点?
		if (srcPoint.x == destPoint.x && srcPoint.y == destPoint.y)
		{
			return true;
		}
	}
	return false;
}

bool RGISCvLight::DoLightAndNode(const RGISNode * inNode , const RGISNodeIntArray * inNodeArray , 
								 const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const
{
return false;
	//2点以上は必要.
	if (inSrc->getCount() < 2) return false;

	int count = inSrc->getCount();

	unsigned long * enablePoint = new unsigned long[count];
	int enableCount = 0;
	enablePoint[enableCount++] = inSrc->getPoint(0);	//先頭は省略不可.

	int start = 0;
	for(int i = 1 ; i < count - 1 ; i++)
	{
		int t1 = inSrc->getPoint(i) - 1;
		int t2 = inSrc->getPoint(start) - 1;
		int t3 = inSrc->getPoint(i + 1) - 1;
		UPOINT target = inSlp->getPoint(inSrc->getPoint(i) - 1);
		if ( !isNode(inNode , inNodeArray , inSlp , target) &&
			 IsLineOn(	inSlp->getPoint(inSrc->getPoint(start) - 1) ,
						target ,
						inSlp->getPoint(inSrc->getPoint(i + 1) - 1) 
						) ) 
		{
			//この点は、省略可能.
			TRACE("省略します %d\n" , i);
		}
		else
		{
			//この点は、省略不能.
			enablePoint[enableCount++] = inSrc->getPoint(i);
			start = i;
		}
	}
	//終端は省略不可
	enablePoint[enableCount++] = inSrc->getPoint(count - 1);	//先頭は省略不可.

	//省略できない場合
	if (enableCount == count )
	{
		delete [] enablePoint;
		return false;
	}

	//省略できる場合

	//新規に作り直す.
	outDest->Create(enablePoint , enableCount);

	delete [] enablePoint;

	return true;
}

bool RGISCvLight::DoLight(const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const
{
return false;
	//2点以上は必要.
	if (inSrc->getCount() < 2) return false;

	int count = inSrc->getCount();

	unsigned long * enablePoint = new unsigned long[count];
	int enableCount = 0;
	enablePoint[enableCount++] = inSrc->getPoint(0);	//先頭は省略不可.

	int start = 0;
	for(int i = 1 ; i < count - 1 ; i++)
	{
		if ( IsLineOn(	inSlp->getPoint(inSrc->getPoint(start) - 1) ,
						inSlp->getPoint(inSrc->getPoint(i) - 1) ,
						inSlp->getPoint(inSrc->getPoint(i + 1) - 1) 
						) ) 
		{
			//この点は、省略可能.
			TRACE("省略します %d\n" , i);
		}
		else
		{
			//この点は、省略不能.
			enablePoint[enableCount++] = inSrc->getPoint(i);
			start = i;
		}
	}
	//終端は省略不可
	enablePoint[enableCount++] = inSrc->getPoint(count - 1);	//先頭は省略不可.

	//省略できない場合
	if (enableCount == count )
	{
		delete [] enablePoint;
		return false;
	}

	//省略できる場合

	//新規に作り直す.
	outDest->Create(enablePoint , enableCount);

	delete [] enablePoint;

	return true;
}

//inSrc と inDest の2点を通る直線上に inTarget がありますか?
bool RGISCvLight::IsLineOn(const UPOINT  inSrc , const UPOINT  inTarget , const UPOINT inDest ) const
{
	if (inSrc.y == inDest.y)
	{
		if (inSrc.y == inTarget.y)
		{
			return true;
		}
	}
	else if (inSrc.x == inDest.x)
	{
		if (inSrc.x == inTarget.x)
		{
			return true;
		}
	}
	else
	{
		double x = (inTarget.y - inSrc.y) / ((double)(inDest.y - inSrc.y) / (inDest.x - inSrc.x))  + inSrc.x;
		if (inTarget.x - 3 >= x && inTarget.x + 3 <= x)	//3は適当な許容範囲.
		{
			return true;
		}
	}
	return false;
}
