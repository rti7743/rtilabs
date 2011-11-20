// BClickMode.h: BClickMode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCLICKMODE_H__70E9D057_1D3E_4D85_85F1_99E6263A0F1E__INCLUDED_)
#define AFX_BCLICKMODE_H__70E9D057_1D3E_4D85_85F1_99E6263A0F1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class BClickMode  
{
public:
	BClickMode();
	virtual ~BClickMode();

	void OnMouseDown(int x, int y)
	{
		DownPoint.x = x;
		DownPoint.y = y;
	}
	bool OnMouseUp(int x, int y) const
	{
		return ( abs(DownPoint.x - x) < 5 && abs(DownPoint.y - y) < 5);
	}
private:

	POINT	DownPoint;

};

#endif // !defined(AFX_BCLICKMODE_H__70E9D057_1D3E_4D85_85F1_99E6263A0F1E__INCLUDED_)
