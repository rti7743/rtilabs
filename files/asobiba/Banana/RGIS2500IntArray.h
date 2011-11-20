// RGIS2500IntArray.h: RGIS2500IntArray クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_)
#define AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"
#include "RGIS2500Head.h"

//アークの最大値 アークは、 4096 以上の要素を保持しない
const int MAX_ARC_BUFFER	=	4096;
//アークの一要素の長さの最大値
const int MAX_ARC_STRING_BUFFER	=	256;

//ホリゴンの最大値 アークは、 4096 以上の要素を保持しない
const int MAX_PRG_BUFFER	=	4096;
//ホリゴンの一要素の長さの最大値
const int MAX_PRG_STRING_BUFFER	=	256;

class RGIS2500PointArray  
{
public:
	RGIS2500PointArray();
	virtual ~RGIS2500PointArray();

	void Create(int inMax , const char**  inStrArray , const RGIS2500Head* inHead) throw(RParseException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	UPOINT getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	//スケールを変更し、ディスプレイ座標にします。
	void ConvertDisplayScal(int inShift) throw(RException);

private:
	unsigned int	Count;
	UPOINT*			Points;
};

class RGIS2500IntArray  
{
public:
	RGIS2500IntArray();
	virtual ~RGIS2500IntArray();

	void Create(int inMax , const char ** inStrArray) throw(RParseException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	int getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}

private:
	unsigned int	Count;
	int*			Points;
};

#endif // !defined(AFX_RGIS2500INTARRAY_H__EE7475C5_FC67_4F63_97C4_FDBDE1B06507__INCLUDED_)
