// RMouseGestureMakeZende.h: RMouseGestureMakeZende クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_)
#define AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RMouseGestureMousePoints.h"

const int MAX_ZONDE = 12;

class RMouseGestureZonde
{
public:
	RMouseGestureZonde()
	{
		ZeroMemory(&Hit , sizeof(unsigned char) * MAX_ZONDE);
	}
	//ヒット率の取得.
	int getHit(int inIndex) const
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		return Hit[inIndex];
	}
	//ヒット率の設定.
	void setHit(int inIndex,unsigned char inVal)
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		Hit[inIndex] = inVal;
	}
	//ヒット率をカウントアップ
	void upHit(int inIndex)
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <  MAX_ZONDE);
		Hit[inIndex] ++;
	}

	//一致率の計算.
	static int Rating(const RMouseGestureZonde & inMy , const RMouseGestureZonde & inTarget)
	{
		return 0;
	}

private:
	//ヒットデータ.
	unsigned char	Hit[MAX_ZONDE];
};

//直線の交差は http://www.hidecnet.ne.jp/~sinzan/tips/c/c_tip15.htm を参考にしました。
class RMouseGestureMakeZende  
{
private:
	class LINE	//線分、直線
	{
	public:
		LINE(POINT a,POINT b)
		{
			this->a = a;
			this->b = b;
		}
		LINE(int sx,int sy , int ex,int ey)
		{
			this->a.x = sx;
			this->a.y = sy;
			this->b.x = ex;
			this->b.y = ey;
		}
		POINT	a; // 始点
		POINT	b; // 終点
	};
public:
	RMouseGestureMakeZende();
	virtual ~RMouseGestureMakeZende();

	//Zonde法により解析します。
	RMouseGestureZonde Convert(MOUSE_GESTURE_MOUSE_POINTS inPoints);
private:
	//交差を判定.
	// 線分 e1, e2 が交差するか判定する。
	// 線分 e1, e2 が交差すれば真、さもなくば偽を返す
	bool Intersect( LINE e1, LINE e2 );
	// 点が直線上に存在する場合には『直線を跨ぐ』と判断する。
	// 点 a, b が直線 e を跨いで存在するか判定する。
	// 点 a, b が直線 e を跨いでいれば真、さもなくば偽を返す
	bool Straddle( LINE e, POINT a, POINT b );
	// 点 p が有向線分 e(a,b) のどちら側にあるか調べる。
	// 点 p が有向線分 e の　右側にある場合  1 を、
	// 　   　有向線分 e の直線上にある場合  0 を、
	// 　   　有向線分 e の　左側にある場合 -1 を返す
	int Side( POINT p, LINE e );

};

#endif // !defined(AFX_RMOUSEGESTUREMAKEZENDE_H__086775A0_88B7_4659_907F_B84CB8534E62__INCLUDED_)
