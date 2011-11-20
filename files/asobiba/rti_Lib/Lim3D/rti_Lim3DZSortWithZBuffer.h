#ifndef	___LIM3D_ZSORT_WITH_ZBUFFER_H	//重複防止
#define ___LIM3D_ZSORT_WITH_ZBUFFER_H

#include <./Lim3D/rti_Lim3DModel.h>
#include <./Lim3D/rti_Lim3DDefine.h>

class TLim3DPolygon;

//最大1万ポリゴン!! できるかなぁ(ため息)
#define MAX_POLYGON	10000

//Zbuffer の利点を生かすために ZSort します(笑) つまり、手前からかいていきます.
class	TLim3DZSortWithZBuffer 
{
public:
	TLim3DZSortWithZBuffer();
	~TLim3DZSortWithZBuffer();

	//新規ポリゴン登録.
	bool Add(TLim3DPolygon* inPolygon);
	//すべてのポリゴンの破棄.
	void Clear();
	//破棄.
	void Release();
	//視点に近いやつからならべていきます.
	void Sort();
	//すべて絵画.
	void AllDraw();
protected:
	//実際にソートするところ
	static void QuickSort(TLim3DPolygon*  a[], int first, int last);

	//ポリゴンを入れておく配列(速度優先のため固定です(涙))
	TLim3DPolygon* 	m_PoolPolygon[MAX_POLYGON];
	int				m_NowLast;
private:
};

#endif							//重複防止
