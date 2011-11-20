// RMouseGestureMousePoints.h: RMouseGestureMousePoints クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_)
#define AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

typedef vector<POINT>	MOUSE_GESTURE_MOUSE_POINTS;

class RMouseGestureMousePoints  
{
public:
	RMouseGestureMousePoints();
	virtual ~RMouseGestureMousePoints();

	//軌跡を追加
	//追加された場合は true
	//ポリシー違反で追加されない場合は false .
	bool Insert(int x,int y);

	//軌跡データを消去
	void Clear()
	{
		Points.clear();
	}
	//ポイントを取得
	//インライン展開を激しくｷﾎﾞﾝﾇ.
	const MOUSE_GESTURE_MOUSE_POINTS	getPoints() const
	{
		return Points;
	}
private:
	// 1 , 0 , -1 の何れかに統一します。
	int Normalize(int inSa)
	{
		//まっすぐはぶれがあるので補正します。
		if ( ::abs(inSa) <= this->CompensationDistrance) return 0;
		if ( inSa > 0) return 1;
		return -1;
	}
private:
	//補正する距離
	int				CompensationDistrance;
	//マウスの軌跡. おきないから奇跡って言うんですよ.
	MOUSE_GESTURE_MOUSE_POINTS	Points;
};

#endif // !defined(AFX_RMOUSEGESTUREMOUSEPOINTS_H__37C3A391_0964_4F0D_BD44_5D336C5CD698__INCLUDED_)
