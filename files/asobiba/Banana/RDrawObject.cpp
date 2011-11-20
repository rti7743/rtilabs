// RDrawObject.cpp: RDrawObject クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RDrawObject.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDrawObject::RDrawObject()
{
}

RDrawObject::~RDrawObject()
{

	Delete();
}

void RDrawObject::Create(RDIBBitmap * poolBitmap , int inWidth , int inHeight , const RGISSmallShareImage* poolShareImage) throw (RException)
{
	PoolBitmap = poolBitmap;
	Width = inWidth;
	Height = inHeight;

	//フォントキャッシュの作成.
	Cache.Create(100 , 50 , 10 , "ＭＳ ゴシック" , false , false , false , false ,SHIFTJIS_CHARSET);
	SujiCahce.Create(7 , "ＭＳ ゴシック" , false , false , false , false ,SHIFTJIS_CHARSET);

	PoolShareImage = poolShareImage;
}

void RDrawObject::Delete()
{
}


void RDrawObject::Line(const POINT & inFrom , const POINT & inTo , COLORREF16 inColor , int inSize)
{
	RGraphics::Line(PoolBitmap , inFrom.x , inFrom.y , inTo.x , inTo.y , inColor , inSize);
}


void RDrawObject::TextOut(const POINT & inPoint,const string & inStr)
{
	if ( !inStr.empty() )
		Cache.TextOut(PoolBitmap , inPoint.x , inPoint.y , inStr);
}
void RDrawObject::SujiOut(const POINT & inPoint,unsigned int inSuji)
{
	SujiCahce.TextOut(PoolBitmap->getDC() , inPoint.x , inPoint.y , inSuji);
}



void RDrawObject::BitBlt(int sx,int sy,int sw,int sh,int ex,int ey)
{
	::BitBlt(PoolBitmap->getDC() , sx,sy,sw,sh , PoolBitmap->getDC() ,ex ,ey , SRCCOPY);
}


void RDrawObject::Black( COLORREF16 color)
{
	RGraphics::Clear(PoolBitmap, color  );

}


void RDrawObject::setPixel(int x,int y , COLORREF16 inColor )
{
	RGraphics::PSet(PoolBitmap , x, y , inColor) ;
}

unsigned short RDrawObject::getPixel(int x,int y )
{
	return *PoolBitmap->getImage(x,y);
}

void RDrawObject::TranslucenceBox(const POINT & inPoint , int inSize , unsigned char inColor)
{
	RGraphics::TranslucenceBox(PoolBitmap ,
		inPoint.x - inSize, inPoint.y - inSize, inSize * 2, inSize * 2, 
		BBananaConfig::getInstance()->getColor(COLOR_PEN1 + (inColor - 1))
	);
}



void RDrawObject::setMark(const POINT & inPoint , int inBitmapNo)
{
	RGraphics::Copy( PoolShareImage->getIconImage() , inBitmapNo * 16 , 0 , 16 , 16 , PoolBitmap , inPoint.x, inPoint.y );
}


void RDrawObject::Paint(const POINT & inPoint, COLORREF16 inDrawColoer ,COLORREF16 inTarget)
{
	if (inPoint.x < 0 || inPoint.x >= this->Width || inPoint.y < 0 || inPoint.y >= this->Height)
		return ;
	RGraphics::Paint(PoolBitmap , inPoint.x , inPoint.y , inTarget , &Clipper );
}

//中央に十字を書く
void RDrawObject::CenterCross()
{
	//画面中央に十字を書きます.
	RGraphics::Line( PoolBitmap , Width / 2 - 5, Height / 2, Width / 2 + 5, Height / 2 , COLORREF16(RGB(128,0,0)) , 1);
	RGraphics::Line( PoolBitmap , Width / 2 , Height / 2 - 5, Width / 2, Height / 2 + 5, COLORREF16(RGB(128,0,0)) , 1);
}


//メッシュ間隔
const int MESH_INTERVAL	=	256;
//一ラインにいくつメッシュがあるか?
const int MESH_LINE		=	(65536/MESH_INTERVAL);

void RDrawObject::Boxes(const RGISSmallDrawEffect* inEffect ,unsigned char * inLevel    ,const LPOINT * inBlockStart,const COLORREF16 * inPallet) const
{

	inEffect->Boxes( getWidth() ,getHeight() ,
		(unsigned short*)PoolBitmap->getImage() , inLevel    ,inBlockStart,inPallet) ;

/*
	POINT screen = {0,0};
	int w = getWidth();
	int h = getHeight();

	unsigned short * image = (unsigned short*)PoolBitmap->getImage() ;

	for(screen.y = 0 ; screen.y < h ; screen.y++)
	{
		for(screen.x = 0 ; screen.x < w ; screen.x++)
		{
			if (*image == COLOR_9 )
			{
				LPOINT xy = inEffect->RevAttach(&screen);
//				ASSERT(nxy.x == xy.x && nxy.y == xy.y);

				if (xy.x >= inBlockStart->x && xy.y >= inBlockStart->y &&
					xy.x <= inBlockStart->x+65536 && xy.y <= inBlockStart->y+65536 )
				{
					int count = ((xy.x - inBlockStart->x) / MESH_INTERVAL) + ((xy.y - inBlockStart->y) / MESH_INTERVAL * MESH_LINE);
//					ASSERT(count >= 0 && count <= 65536);

					*image = inPallet[inLevel[count]];
				}
			}
			image++;
		}
	}
*/
}
