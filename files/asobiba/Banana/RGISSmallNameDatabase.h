// RGISSmallNameDatabase.h: RGISSmallNameDatabase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLNAMEDATABASE_H__09A68C0D_B511_4902_ABB7_A5291674883C__INCLUDED_)
#define AFX_RGISSMALLNAMEDATABASE_H__09A68C0D_B511_4902_ABB7_A5291674883C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"

class RGISSmallNameDatabase  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallNameDatabase();
	virtual ~RGISSmallNameDatabase();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	void Delete();

	const char* getPoint(unsigned int inP) const
	{
		ASSERT( inP < DataCount);

		return Data[inP];
	}
	unsigned int getCount() const
	{
		return DataCount;
	}

private:
	const char **			Data;
	int						DataSize;
	unsigned int			DataCount;
};

#endif // !defined(AFX_RGISSMALLNAMEDATABASE_H__09A68C0D_B511_4902_ABB7_A5291674883C__INCLUDED_)
