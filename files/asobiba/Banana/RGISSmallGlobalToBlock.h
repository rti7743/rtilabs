// RGISSmallGlobalToBlock.h: RGISSmallGlobalToBlock クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLGLOBALTOBLOCK_H__A7A8088C_434E_4D25_809F_8060C350EC92__INCLUDED_)
#define AFX_RGISSMALLGLOBALTOBLOCK_H__A7A8088C_434E_4D25_809F_8060C350EC92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
const int NEXT_LINE_BLOCK	= 4096;

//グローバル座標をブロック座標に変換.
class RGISSmallGlobalToBlock  
{
public:
	//ブロックに分断
	void Create(unsigned long inX , unsigned long inY) 
	{
		const int y = inY / 65536;
		const int x = inX / 65536;

		this->Block = (y) * (NEXT_LINE_BLOCK) + (x);
		Point.x = (unsigned short) (inX - (x * 65536));
		Point.y = (unsigned short) (inY - (y * 65536));
	}
	unsigned long getBlock() const
	{
		return Block;
	}
	const SPOINT * getPoint() const
	{
		return &Point;
	}
private:
	unsigned long	Block;
	SPOINT			Point;
};

#endif // !defined(AFX_RGISSMALLGLOBALTOBLOCK_H__A7A8088C_434E_4D25_809F_8060C350EC92__INCLUDED_)
