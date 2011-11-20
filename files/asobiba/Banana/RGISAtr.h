// RGISAtr.h: RGISAtr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISATR_H__FD7709F6_708C_472F_890E_B769BB5C1B0C__INCLUDED_)
#define AFX_RGISATR_H__FD7709F6_708C_472F_890E_B769BB5C1B0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

struct AtrData
{
	int	Type;						//図式分類コード
	int No;							//個別番号

	string	attrib[4];				//属性
};

class RGISAtr  
{
public:
	RGISAtr();
	virtual ~RGISAtr();

	void Create(const string & inFilename, int inScale) throw(RException);
	void Delete();

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const AtrData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

private:
	unsigned int			Count;
	AtrData*	Points;

};

#endif // !defined(AFX_RGISATR_H__FD7709F6_708C_472F_890E_B769BB5C1B0C__INCLUDED_)
