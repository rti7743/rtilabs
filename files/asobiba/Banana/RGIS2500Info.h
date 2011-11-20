// RGIS2500Info.h: RGIS2500Info クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500INFO_H__736AC5CB_D300_4CB6_8151_AC736DC41A49__INCLUDED_)
#define AFX_RGIS2500INFO_H__736AC5CB_D300_4CB6_8151_AC736DC41A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS2500IntArray.h"

class RGIS2500Info  
{
public:
	RGIS2500Info(const RGIS2500PointArray* inArray,const string & inName );
	RGIS2500Info(const LPOINT* inPoint,const string & inName );
	virtual ~RGIS2500Info()
	{
	}

	LPOINT getPoint() const
	{
		return XY;
	}
	const string getName() const
	{
		return Name;
	}
private:
	LPOINT	XY;
	string	Name;
};

typedef newVector<RGIS2500Info>	RGIS2500InfoVector;

//GIS2500 にこういう情報がありました。
//ほかの(25000)の地図にはこの情報はありませんか?
struct MessageGIS2500Info
{
	RGIS2500InfoVector	Tatemono;
	RGIS2500InfoVector	Eki;

};


#endif // !defined(AFX_RGIS2500INFO_H__736AC5CB_D300_4CB6_8151_AC736DC41A49__INCLUDED_)
