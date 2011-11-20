// RGISMathAlgorithm.h: RGISMathAlgorithm クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_)
#define AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//このクラスは、
//http://homepage3.nifty.com/Nowral/
//のソースコードを参考に書かれています。
//
//参考にしたソースは GPL なので、
//このソースコードも GPL になります。

class RGISMathAlgorithm  
{
public:
	RGISMathAlgorithm(double a , double f);
	virtual ~RGISMathAlgorithm();
public:
	// Gauss-Klueger
	DoublePoint RGISMathAlgorithm::ll2xy(double b,double dl) const;
	// 赤道から緯度Bまでの子午線弧長
	double RGISMathAlgorithm::lom(double b) const;
	//???
	double RGISMathAlgorithm::eta2(double b) const;
	//卯酉線曲率半径
	double RGISMathAlgorithm::bouyuu(double eta2) const;

	// 楕円体座標 -> 直交座標
	Double3Point RGISMathAlgorithm::llh2xyz(double b,double l,double h) const;
	// 直交座標 -> 楕円体座標
	Double3Point RGISMathAlgorithm::xyz2llh(double x,double y,double z) const;

double getED2() const
{
	return this->ED2;
}
double getCAD() const
{
	return this->CAD;
}

private:
	double PI;
	double RD;

	double A;
	double F;

	// 第1離心率
	double E2;
	// 第2離心率
	double ED2;

	double CAD;
	double CBD;
	double CCD;
	double CDD;
	double CED;

};

#endif // !defined(AFX_RGISMATHALGORITHM_H__EC25D7A9_EE39_4E97_B203_ABF79C07E3C2__INCLUDED_)
