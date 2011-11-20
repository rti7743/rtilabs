// RGISMath.h: RGISMath クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISMATH_H__D81D98AB_66BC_4B06_8FDD_D5FB9911625C__INCLUDED_)
#define AFX_RGISMATH_H__D81D98AB_66BC_4B06_8FDD_D5FB9911625C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISMathAlgorithm.h"


//このクラスは、
//http://homepage3.nifty.com/Nowral/
//のソースコードを参考に書かれています。
//
//参考にしたソースは GPL なので、
//このソースコードも GPL になります。
class RGISMath  
{
public:
	RGISMath();
	virtual ~RGISMath();

	int setScale();

	//座標系
	static const RGISMathAlgorithm	Tokyo;
	static const RGISMathAlgorithm	WGS84;

	//GPS(Tokyo)が取得した座標を直行座標に変換
	LPOINT GPSBLTo(double inB,double inL ,int inScale) const;

	//経緯度を25000の直行座標に変換
	LPOINT KeiidoTo(double inToukei,double inHokui ,int inScale) const;

	DoublePoint DMS2UTM(double inHokui ,double inToukei , const RGISMathAlgorithm * inModel) const;
	DoublePoint UTM2DMS(double x , double y , int zn, const RGISMathAlgorithm * inModel) const;
	DoublePoint DMS2JMG(double inHokui ,double inToukei , int zn, const RGISMathAlgorithm * inModel) const;
	DoublePoint JMG2DMS(double x , double y , int zn, const RGISMathAlgorithm * inModel) const;
	DoublePoint TransModel(double inHokui , double inToukei ,
		const RGISMathAlgorithm * inSrcModel ,const RGISMathAlgorithm * inDestModel ) const;

	static void RGISMath::test();
private:
	DoublePoint RGISMath::getJMGZone(int zn) const;

	double PI;
	double RD;
};

#endif // !defined(AFX_RGISMATH_H__D81D98AB_66BC_4B06_8FDD_D5FB9911625C__INCLUDED_)
