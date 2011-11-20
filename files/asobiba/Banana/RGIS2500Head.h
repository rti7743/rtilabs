// RGIS2500Head.h: RGIS2500Head クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_)
#define AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"



class RGIS2500Head  
{
public:
	RGIS2500Head();
	virtual ~RGIS2500Head();

	void Create(const char * inStr , int inScale) throw(RException);

	unsigned int getCount() const
	{
		return Count;
	}

	UPOINT convertXY(double x,double y) const;

	static void test();

private:
	//図名
	char Zumai[128];
	//ファイル名 
	char Filename[256];
	//レコード総数
	int Record;
	//要素の総数
	int Count;
	//座標系フラグ
	int ZahyoFlg;
	//座標系番号
	int ZahyoNumber;
	//数値化区分
	int Kubun;

	DoublePoint UpperLeft;
	DoublePoint LowerLeft;
	DoublePoint UpperRight;
	DoublePoint LowerRight;

	int			Scale;
};

#endif // !defined(AFX_RGIS2500HEAD_H__479B561D_4975_46B7_B1B9_1FF5FD2A594E__INCLUDED_)
