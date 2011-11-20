// RMouseGestureMakeZende.cpp: RMouseGestureMakeZende クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMouseGestureMakeZende.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMouseGestureMakeZende::RMouseGestureMakeZende()
{

}

RMouseGestureMakeZende::~RMouseGestureMakeZende()
{

}

//Zonde法により解析します。
RMouseGestureZonde RMouseGestureMakeZende::Convert(MOUSE_GESTURE_MOUSE_POINTS inPoints)
{
	RMouseGestureZonde ret;
	const int size = inPoints.size() ;

	//描画されたジェスチャーがぴったりおさまる枠を求めます。
	RECT CanvsRect;
	CanvsRect.left = INT_MAX;
	CanvsRect.top = INT_MAX;
	CanvsRect.right = INT_MIN;
	CanvsRect.bottom = INT_MIN;
	for(int i = 0 ; i < size ; i++)
	{
		POINT p = inPoints[i];
		if ( CanvsRect.left > p.x)	CanvsRect.left = p.x;
		if ( CanvsRect.right < p.x)	CanvsRect.right = p.x;
		if ( CanvsRect.top > p.y)	CanvsRect.top = p.y;
		if ( CanvsRect.bottom < p.y)	CanvsRect.bottom = p.y;
	}

	int canvsWidth = CanvsRect.right - CanvsRect.left;
	int canvsHegith = CanvsRect.bottom - CanvsRect.top;

	//直線との交差により zonde を求めます。
	for(i = 1 ; i < size ; i++)
	{
		LINE target(inPoints[i - 1] , inPoints[i]);
		//1    2      345    6      7    
		//  \  |    /  |   \ |    /      
		//8-------/-----------\-----     
		//     |/      |     |/  \       
		//    /|  \    |    /|     \     
		//  /  |    \  | /   |      \    
		//9------------------------12    
		// 10  |      /|\    |   /       
		//    \|   /   |  \  | /         
		//11-----\-----------/------     
		//   / |   \   |  /  |\          
		///    |     \ /     |  \        

		//1
		if (Intersect( target ,  LINE(CanvsRect.left , CanvsRect.top		, CanvsRect.right , CanvsRect.bottom) )	)
			ret.upHit(1 -1);
		//2
		if (Intersect( target ,  LINE(canvsWidth / 4, CanvsRect.top		, canvsWidth / 4, CanvsRect.bottom) )	)
			ret.upHit(2 -1);
		//3
		if (Intersect( target ,  LINE(canvsWidth / 2, CanvsRect.top		, CanvsRect.left , canvsHegith / 2) )	)
			ret.upHit(3 -1);
		//4
		if (Intersect( target ,  LINE(canvsWidth / 2, CanvsRect.top		, canvsWidth / 2 , CanvsRect.bottom) )	)
			ret.upHit(4 -1);
		//5
		if (Intersect( target ,  LINE(canvsWidth / 2, CanvsRect.top		, CanvsRect.right , canvsHegith / 2) )	)
			ret.upHit(5 -1);
		//6
		if (Intersect( target ,  LINE(canvsWidth / 4 * 3, CanvsRect.top		, canvsWidth / 4 * 3, CanvsRect.bottom) )	)
			ret.upHit(6 -1);
		//7
		if (Intersect( target ,  LINE(CanvsRect.right , CanvsRect.top		, CanvsRect.left , CanvsRect.bottom) )	)
			ret.upHit(7 -1);
		//8
		if (Intersect( target ,  LINE(CanvsRect.left , canvsHegith / 4		, CanvsRect.right , canvsHegith / 4) )	)
			ret.upHit(8 -1);
		//9
		if (Intersect( target ,  LINE(CanvsRect.left , canvsHegith / 2		, CanvsRect.right , canvsHegith / 2) )	)
			ret.upHit(9 -1);
		//10
		if (Intersect( target ,  LINE(CanvsRect.left , canvsHegith / 2		, canvsWidth / 2 , CanvsRect.bottom) )	)
			ret.upHit(10 -1);
		//11
		if (Intersect( target ,  LINE(CanvsRect.left , canvsHegith / 4 *3	, CanvsRect.right , canvsHegith / 4 *3) )	)
			ret.upHit(9 -1);
		//12
		if (Intersect( target ,  LINE(CanvsRect.right , canvsHegith / 2		, canvsWidth / 2 , CanvsRect.bottom) )	)
			ret.upHit(12 -1);
	}
	return ret;
}


//交差を判定.
// 線分 e1, e2 が交差するか判定する。
// 線分 e1, e2 が交差すれば真、さもなくば偽を返す
bool RMouseGestureMakeZende::Intersect( LINE e1, LINE e2 )
{
	// 線分 e1, e2 を対角線とする２つの長方形が交わらない場合、線分 e1, e2 が交わることはない。
	if ( min(e1.a.x,e1.b.x) > max(e2.a.x,e2.b.x) ) return 0;
	if ( min(e1.a.y,e1.b.y) > max(e2.a.y,e2.b.y) ) return 0;
	if ( min(e2.a.x,e2.b.x) > max(e1.a.x,e1.b.x) ) return 0;
	if ( min(e2.a.y,e2.b.y) > max(e1.a.y,e1.b.y) ) return 0;

	// 線分 e2 が直線 e1 を跨ぎ、かつ、線分 e1 が直線 e2 を跨ぐならば、線分 e1, e2 は交差する。
	return Straddle( e1, e2.a, e2.b ) && Straddle( e2, e1.a, e1.b );
}//intersect

// 点が直線上に存在する場合には『直線を跨ぐ』と判断する。
// 点 a, b が直線 e を跨いで存在するか判定する。
// 点 a, b が直線 e を跨いでいれば真、さもなくば偽を返す
bool RMouseGestureMakeZende::Straddle( LINE e, POINT a, POINT b )
{
	return 0 >= ( Side(a,e) * Side(b,e) );
}//straddle

// 点 p が有向線分 e(a,b) のどちら側にあるか調べる。
// 点 p が有向線分 e の　右側にある場合  1 を、
// 　   　有向線分 e の直線上にある場合  0 を、
// 　   　有向線分 e の　左側にある場合 -1 を返す
int RMouseGestureMakeZende::Side( POINT p, LINE e )
{
	int     n;
	POINT	p1, p2, p3;

	// 有向線分 (p2,p1), (p2,p3) の外積を求める
	p1 = p; p2 = e.a; p3 = e.b;
	n  = p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y);

	if      ( n > 0 ) return  1; // 右
	else if ( n < 0 ) return -1; // 左
	else              return  0; // 線上
}//side

