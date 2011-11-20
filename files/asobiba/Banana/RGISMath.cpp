// RGISMath.cpp: RGISMath クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "stdafx.h"
#include "RGISMath.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const RGISMathAlgorithm	RGISMath::Tokyo(6377397.155 , 1.f / 299.152813);
const RGISMathAlgorithm	RGISMath::WGS84(6378137.f , 1.f / 298.257223);

class RGISKeiidoData2
{
private:
	double	DB;
public:

	RGISKeiidoData2()
	{
		this->DB = 0;
	}

	RGISKeiidoData2(int inDo , int inFun , double inByou )
	{
		setInt(inDo , inFun , inByou );
	}
	void setInt(int inDo , int inFun , double inByou)
	{
		ASSERT(inFun >= 0 && inFun <= 60 );
		ASSERT(inByou >= 0 && inByou <= 60 );

		this->DB = ((double)inDo) + ((double)inFun) / 60.f + inByou / 3600.f;
	}
	void setDouble(double x)
	{
		this->DB = x;
	}
	void setInt(unsigned long x)
	{
        double ans = x / (double)360000;
//        s = s + " 北緯 " + (int)ans + "度";
		int dd = (int)ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + "分";
		int mm = ans;

        ans -= (int)ans;
        ans *= (double)60;

		setInt(dd , mm , ans);
	}

	double toDouble() const
	{
//		return (double)Do *3600 + Fun *60 + Byou + (M / 10);
//		return Do + Fun / 60.0 + Byou / 3600.0 + M / 360000.0;
		return this->DB;
	}

	const string toString() const
	{
		double temp = this->DB;
		int dd = floor(temp);
		temp = (temp - dd) * 60;

		int mm = (int)floor( temp );
		temp = (temp - mm) * 60;

		int ss = (int)floor(temp);

		int ms = (int) ((temp - ss) * 100) ;

		return PrintFormat("%d/%d/%d.%d" , dd , mm , ss , ms);
	}

	bool operator==(const RGISKeiidoData2 & inTarget) const
	{
		return inTarget.DB == this->DB;
	}

	static void test()
	{
		{
			RGISKeiidoData2 kei(36,3,4.5);
			double k = kei.toDouble();

			RGISKeiidoData2 kei2;
			kei2.setDouble(k);

			ASSERT(kei == kei2);
		}
		{
			RGISKeiidoData2 kei(44,0,3.24);
			double k = kei.toDouble();

			RGISKeiidoData2 kei2;
			kei2.setDouble(44.0009);

			ASSERT(kei == kei2);

			string r;
			ASSERT( (r = kei.toString() ) == "44/0/3.24");
		}
	}
};

RGISMath::RGISMath()
{
	PI  = 4 * atan2( (double)1,(double)1); // 円周率
	RD  = PI / 180;       // radian / degree
}


RGISMath::~RGISMath()
{

}

//tokyo 測地系を WGS84に変換します.
//http://member.nifty.ne.jp/Nowral/02_DATUM/02_DATUM.html
class RGISDATUM
{
public:
	// 測地系変換
	 void TransDatum( double inB , double inL ,double *outB , double *outL) 
	 {
		// データム諸元
		// (WGS 84)
		double a_w = 6378137;          // 赤道半径
		double f_w = 1 / 298.257223;   // 扁平率
		double e2_w = 2*f_w - f_w*f_w; // 第1離心率
		// (Tokyo)
		double a_t = 6377397.155;
		double f_t = 1 / 299.152813;
		double e2_t = 2*f_t - f_t*f_t;
		// 並行移動量 [m]
		// e.g. x_t + dx_t = x_w etc.
		double dx_t = -148;
		double dy_t =  507;
		double dz_t =  681;

		// 測地系変換
		double h = 0;
//		if( document.myForm.FromTo.selectedIndex == 1 ) { // WGS 84 -> Tokyo
//			pvec = llh2xyz(b, l, h, a_w, e2_w);
//			pvec = xyz2llh(pvec[0]-dx_t, pvec[1]-dy_t, pvec[2]-dz_t, a_t, e2_t);
//		}
//		else { // Tokyo -> WGS 84
		llh2xyz(inB, inL, h, a_t, e2_t);
		xyz2llh(this->X +dx_t, this->Y+dy_t, this->Z+dz_t, a_w, e2_w);
//		}
		
		*outB = this->X;
		*outL = this->Y;
	}
private:
	
	void llh2xyz(double b,double l,double h,double a,double e2) 
	{ // 楕円体座標 -> 直交座標
		double rd = 3.141592 / 180;
		b *= rd;
		l *= rd;
		double sb = sin(b);
		double cb = cos(b);
		double rn = a / sqrt(1-e2*sb*sb);

		this->X = (rn+h) * cb * cos(l);
		this->Y = (rn+h) * cb * sin(l);
		this->Z = (rn*(1-e2)+h) * sb;
	}

	void xyz2llh(double x,double y,double z,double a,double e2) 
	{ // 直交座標 -> 楕円体座標
		double rd = 3.141592 / 180;
		double bda = sqrt(1-e2);

		double p = sqrt(x*x + y*y);
		double t = atan2(z, p*bda);
		double st = sin(t);
		double ct = cos(t);
		double b = atan2(z+e2*a/bda*st*st*st, p-e2*a*ct*ct*ct);
		double l = atan2(y, x);

		double sb = sin(b);
		double rn = a / sqrt(1-e2*sb*sb);
		double h = p/cos(b) - rn;

		this->X = b/rd;
		this->Y = l/rd;
		this->Z = h;
	}

private:
	double		X,Y,Z;
};

DoublePoint RGISMath::TransModel(double inHokui , double inToukei ,
				const RGISMathAlgorithm * inSrcModel ,const RGISMathAlgorithm * inDestModel ) const
{
	DoublePoint xy = {0};	//インプリメントしてません
	return xy;
}



LPOINT RGISMath::GPSBLTo(double inB,double inL , int inScale) const
{
	ASSERT(inScale >= 100 );

	double b;
	double l;
	int a;
	a = (int)(inB / 100);
	b = a + (inB - a * 100) / 60;

	a = (int)(inL / 100);
	l = a + (inL - a * 100) / 60;

	RGISDATUM da;
	da.TransDatum(b , l , &b , &l );

	return KeiidoTo( l , b ,inScale );

}

//経緯度を25000の直行座標に変換
LPOINT RGISMath::KeiidoTo(double inToukei,double inHokui , int inScale) const
{
	ASSERT(inScale >= 100 );

	__int64 x,y ;
	x = (__int64)(inToukei * 36000000);
	y = (__int64)(inHokui * 36000000);

	__int64 t = (__int64) (x + 1000.0);
	double tmp1 = (double)(int)(x / (double)1000) * (double)1000;
	double tmp2 = (double)(int)(x / (double)100) * (double)100;
	if(!  (tmp2 - tmp1 > (double)400))
		x = t;
	t = (__int64) (y + 1000.0);
	tmp1 = (double)(int)(y / (double)1000) * (double)1000;
	tmp2 = (double)(int)(y / (double)100) * (double)100;
	if(! (tmp2 - tmp1 > (double)400))
		y = t;

	LPOINT xy;
	xy.x = (unsigned long)(x / inScale);
	xy.y = 0xffffffff - (unsigned long)(y / inScale);

	return xy;
}



DoublePoint RGISMath::DMS2UTM(double inToukei ,double inHokui , const RGISMathAlgorithm * inModel) const
{
	//経度
	double l = inToukei;
	//緯度
	double b = inHokui;
	// ゾーン < 西境界含む
	double zn = (int)((l+180) / 6) % 60 + 1; 


	double l0 = zn*6 - 183; // 原点
  
	DoublePoint xy;
	xy = inModel->ll2xy(b * this->RD, (l-l0) * this->RD);
	double m0 = 0.9996; // 縮尺係数
	xy.y *= m0;
	xy.x *= m0;
	xy.x += 500000;

	//小数点第3位以降切り捨て
	xy.y = floor(xy.y * 1000)  / 1000.0;
	xy.x = floor(xy.x * 1000)  / 1000.0;

	return xy;

}

DoublePoint RGISMath::UTM2DMS(double x , double y , int zn, const RGISMathAlgorithm * inModel) const
{
	double l0 = zn * 6.f - 183;
	double eps = 0.001 / 3600.f * this->RD; // 収束条件 < 1000分の1秒

	double m0 = 0.9996; // 縮尺係数
	y -= 500000;
	x /= m0;
	y /= m0;

	double db = 1;
	double dl = 1;

	double b = x / inModel->getCAD();
	double cb = cos(b);
	double eta2 = inModel->getED2() * cb * cb;

	double rn = inModel->bouyuu(eta2);	//卯酉線曲率半径
	double l = y / rn / cb;

	while(db >  eps || dl > eps)
	{
		DoublePoint xyd = inModel->ll2xy(b, l);

		db = (x - xyd.x) / inModel->getCAD();
		cb = cos(b);
		eta2 = inModel->getED2() * cb * cb;
		rn = inModel->bouyuu(eta2);	//卯酉線曲率半径
		dl = (y-xyd.y) / rn / cb;

		b += db;
		l += dl;
	}

	DoublePoint xy;
	xy.x = l / this->RD + l0;
	xy.y = b / this->RD;
	return xy;

}


DoublePoint RGISMath::DMS2JMG(double inToukei ,double inHokui , int zn, const RGISMathAlgorithm * inModel) const
{
	//経度(142)
	double l = inToukei;
	//緯度(44)
	double b = inHokui;

	//ゾーンごとの原点を求める.
	DoublePoint o = getJMGZone(zn - 1);

	DoublePoint xy;
	xy = inModel->ll2xy(b * this->RD, (l - o.x ) * this->RD);
	double m0 = 0.9999; // 縮尺係数
	xy.x -= inModel->lom(o.y * this->RD);
	xy.x *= m0;
	xy.y *= m0;

	//小数点第3位以降切り捨て
	xy.y = floor(xy.y * 1000)  / 1000.0;
	xy.x = floor(xy.x * 1000)  / 1000.0;

	return xy;
}

DoublePoint RGISMath::JMG2DMS(double x , double y , int zn, const RGISMathAlgorithm * inModel) const
{
	//ゾーンごとの原点を求める.
	DoublePoint o = getJMGZone(zn - 1);

	double eps = 0.001 / 3600.f * this->RD; // 収束条件 < 1000分の1秒

	double m0 = 0.9999; // 縮尺係数
	x /= m0;
	y /= m0;
	x += inModel->lom(o.y * this->RD);

	double db = 1;
	double dl = 1;

	double b = x / inModel->getCAD();
	double cb = cos(b);
	double eta2 = inModel->getED2() * cb * cb;

	double rn = inModel->bouyuu(eta2);	//卯酉線曲率半径
	double l = y / rn / cb;

	while(db >  eps || dl > eps)
	{
		DoublePoint xyd = inModel->ll2xy(b, l);

		db = (x - xyd.x) / inModel->getCAD();
		cb = cos(b);
		eta2 = inModel->getED2() * cb * cb;
		rn = inModel->bouyuu(eta2);	//卯酉線曲率半径
		dl = (y-xyd.y) / rn / cb;

		b += db;
		l += dl;
	}

	DoublePoint xy;
	xy.x = l / this->RD + o.x;
	xy.y = b / this->RD;
	return xy;

}


DoublePoint RGISMath::getJMGZone(int zn) const
{
	static 
		DoublePoint	xy[] = 
				{{(129.f + 30.f/60.f	),	(33.f)}	,
				{(131.f				),	(33.f)}	,
				{(132.f + 10.f/60.f	),	(36.f)}	,
				{(133.f + 30.f/60.f	),	(33.f)}	,
				{(134.f + 20.f/60.f	),	(36.f)}	,
				{(136.f				),	(36.f)}	,
				{(137.f + 10.f/60.f	),	(36.f)}	,
				{(138.f + 30.f/60.f	),	(33.f)}	,
				{(139.f + 50.f/60.f	),	(36.f)}	,
				{(140.f + 50.f/60.f	),	(40.f)}	,
				{(140.f + 15.f/60.f	),	(44.f)}	,
				{(142.f + 15.f/60.f	),	(44.f)}	,
				{(144.f + 15.f/60.f	),	(44.f)}	,
				{(142.f				),	(26.f)}	,
				{(127.f + 30.f/60.f	),	(26.f)}	,
				{(124.f				),	(26.f)}	,
				{(131.f				),	(26.f)}	,
				{(136.f				),	(20.f)}	,
				{(154.f				),	(26.f)}	,
	};

	//座標原点の経度、緯度を取得
	return xy[zn];
}






void RGISMath::test()
{
	RGISKeiidoData2::test();

	{
		RGISMath math;
		LPOINT xy = math.GPSBLTo( 3642.1913 , 12947.6355 , 100);
						 
		ASSERT(xy.x == 46724973);
		ASSERT(xy.y == 4281753109);
	}
	{
		//経緯度から平面直角へ
		RGISKeiidoData2 a;
		a.setDouble(43.043819);
		RGISKeiidoData2 b;
		b.setDouble(141.319932);
		
		double bb = b.toDouble() ;
		double aa = a.toDouble() ;

		RGISMath math;
		DoublePoint loca = math.DMS2JMG( 
			bb,
			aa,
			12,
			&RGISMath::WGS84
			);

		ASSERT( (int)loca.x == -105804);
		ASSERT( (int)loca.y == -75777);

		//平面直角座標系から、経緯度へ
		RGISMath math2;
		DoublePoint kiido = math2.JMG2DMS( (int)loca.x , (int)loca.y , 12 , &RGISMath::WGS84);
		RGISKeiidoData2 za;
		za.setDouble(kiido.y);
		RGISKeiidoData2 zb;
		zb.setDouble(kiido.x);

	
	}
}
