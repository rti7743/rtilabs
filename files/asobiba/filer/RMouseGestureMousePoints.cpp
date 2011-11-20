// RMouseGestureMousePoints.cpp: RMouseGestureMousePoints クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMouseGestureMousePoints.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMouseGestureMousePoints::RMouseGestureMousePoints()
{
	CompensationDistrance = 10;
}

RMouseGestureMousePoints::~RMouseGestureMousePoints()
{

}

//軌跡を追加
//追加された場合は true
//ポリシー違反で追加されない場合は false .
bool RMouseGestureMousePoints::Insert(int x,int y)
{
	int size = Points.size();
	if (size >= 1)
	{
		//前回の値
		POINT last = Points[size - 1];

		//前回の値から Distrance ピクセル離れていること
		if ( abs(last.x - x) <= this->CompensationDistrance && abs(last.y - y) <= this->CompensationDistrance )	return false;

		if (size >= 2)
		{
			//前々会 (2つ前)と　前回(1つ前)の傾きが同じ方向だったら、
			//前回の消去し、今回の値を入力する.
			POINT moreLast = Points[size - 2];
			if (	Normalize(moreLast.x - last.x) == Normalize(last.x - x) &&
					Normalize(moreLast.y - last.y) == Normalize(last.y - y)	)
			{
				//前回のを上書き
				POINT p;	p.x = x;	p.y = y;
				Points[size - 1] = p;
				return false;
			}
		}
	}
	//んぢゃ、追加してみっか。
	POINT p;	p.x = x;	p.y = y;
	Points.push_back( p );

	return true;

}

