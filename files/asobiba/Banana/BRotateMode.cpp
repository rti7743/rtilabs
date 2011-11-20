// BRotateMode.cpp: BRotateMode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BRotateMode.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BRotateMode::BRotateMode()
{
	this->Alive = false;
}

BRotateMode::~BRotateMode()
{

}
void BRotateMode::Create(const RWindowBase * inWindow)
{
	this->Alive = false;

	RECT rc;
	inWindow->GetClientRect(&rc);
	this->O.x = rc.right / 2;
	this->O.y = rc.bottom / 2;
}

bool BRotateMode::isAlive() const
{
	return this->Alive;
}

void BRotateMode::OnMouseDown(int x,int y , RAngle deg)
{
	if (this->Alive) return ;

	this->Deg = deg;
	this->StartAngle = (RAngle) DDD(atan2( (double) (this->O.y - y) , (double) (this->O.x - x) ));
	this->Alive = true;
}

RAngle BRotateMode::OnMouseMove(int x, int y)
{
	ASSERT(this->Alive);

	RAngle d = (RAngle) DDD(atan2( (double) (this->O.y - y) , (double) (this->O.x - x) ));
	return this->Deg + (d - this->StartAngle);
}
void BRotateMode::OnMouseUp()
{
	this->Alive = false;
}
