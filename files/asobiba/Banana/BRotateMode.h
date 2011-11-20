// BRotateMode.h: BRotateMode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BROTATEMODE_H__F36AC663_AD47_471D_AD01_0C4275D5207B__INCLUDED_)
#define AFX_BROTATEMODE_H__F36AC663_AD47_471D_AD01_0C4275D5207B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RWindowBase.h"
#include "RMathAngle.h"

class BRotateMode
{
public:
	BRotateMode();
	virtual ~BRotateMode();

	void Create(const RWindowBase * inWindow);
	bool isAlive() const;
	void OnMouseDown(int x,int y, RAngle deg);
	RAngle OnMouseMove(int x, int y);
	void OnMouseUp();
private:
	bool	Alive;
	POINT	O;
	RAngle	StartAngle;
	RAngle	Deg;
};


#endif // !defined(AFX_BROTATEMODE_H__F36AC663_AD47_471D_AD01_0C4275D5207B__INCLUDED_)
