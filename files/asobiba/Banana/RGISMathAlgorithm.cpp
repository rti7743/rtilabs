// RGISMathAlgorithm.cpp: RGISMathAlgorithm ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISMathAlgorithm.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////

RGISMathAlgorithm::RGISMathAlgorithm(double a , double f)
{
	this->PI  = 4 * atan2((double)1,(double)1); // ‰~Žü—¦
	this->RD  = PI / 180.f;       // radian / degree

	this->A = a;
	this->F = f;

	this->E2  = 2.f*F - F*F;   // ‘æ1—£S—¦
	this->ED2 = E2 / (1.f-E2);  // ‘æ2—£S—¦

	double e4 = E2 * E2;
	double e6 = e4 * E2;
	double e8 = e6 * E2;

	this->CAD = A * (1.f-E2) * ( 1.0 + 3.0/4.0*E2 + 45.0/64.0*e4 + 175.0/256.0*e6 + 11025.0/16384.0*e8 );
	this->CBD = A * (1.f-E2) * (       3.0/4.0*E2 + 15.0/16.0*e4 + 525.0/512.0*e6 +   2205.0/2048.0*e8 );
	this->CCD = A * (1.f-E2) * (                    15.0/64.0*e4 + 105.0/256.0*e6 +   2205.0/4096.0*e8 );
	this->CDD = A * (1.f-E2) * (                                    35.0/512.0*e6 +    315.0/2048.0*e8 );
	this->CED = A * (1.f-E2) * (                                                       315.0/16384.0*e8);
}

RGISMathAlgorithm::~RGISMathAlgorithm()
{

}


// Gauss-Klueger
DoublePoint RGISMathAlgorithm::ll2xy(double b,double dl) const
{
	DoublePoint xy;

	double dl2 = dl * dl;
	double dl4 = dl2 * dl2;

	double sb = sin(b);
	double cb = cos(b);
	double cb3 = cb * cb * cb;
	double cb5 = cb3 * cb * cb;
	double tb2 = sb * sb / cb / cb;
	double tb4 = tb2 * tb2;

	double eta2 = ED2 * cb * cb;
	double rn = A / (1.f-F) / sqrt(1.f+eta2); // ‰K“Ñü‹È—¦”¼Œa

	xy.x = lom(b);
	xy.x += rn / 2.f  * sb * cb  * dl2;
	xy.x += rn / 24.f * sb * cb3 * (5.f -  tb2 + 9.f* eta2 + 4.f * eta2 * eta2 ) * dl4;
	xy.x += rn / 720.f* sb * cb5 * (61.f - 58.f * tb2 + tb4 ) * dl4 * dl2;

	xy.y = rn * cb * dl;
	xy.y += rn / 6.f   * cb3 * ( 1.f - tb2  + eta2 ) * dl * dl2;
	xy.y += rn / 120.f * cb5 * ( 5.f - 18.f * tb2 + tb4 ) * dl * dl4;

	return xy;
}

// Ô“¹‚©‚çˆÜ“xB‚Ü‚Å‚ÌŽqŒßüŒÊ’·
double RGISMathAlgorithm::lom(double b) const
{
	return CAD*b - CBD/2.f*sin(2.f*b) + CCD/4.f*sin(4.f*b)  - CDD/6.f*sin(6.f*b) + CED/8.f*sin(8.f*b);
}

//‰K“Ñü‹È—¦”¼Œa
double RGISMathAlgorithm::bouyuu(double eta2) const
{
	return A / (1.f - F) / sqrt(1.f + eta2);
}


// ‘È‰~‘ÌÀ•W -> ’¼ŒðÀ•W
Double3Point RGISMathAlgorithm::llh2xyz(double b,double l,double h) const
{ 
	b *= this->RD;
	l *= this->RD;
	double sb = sin(b);
	double cb = cos(b);
	double rn = this->A / sqrt(1 - this->E2 *sb*sb);

	Double3Point xyz;
	xyz.x = (rn+h) * cb * cos(l);
	xyz.y = (rn+h) * cb * sin(l);
	xyz.z = (rn*(1- this->E2 )+h) * sb;

	return xyz;
}

// ’¼ŒðÀ•W -> ‘È‰~‘ÌÀ•W
Double3Point RGISMathAlgorithm::xyz2llh(double x,double y,double z) const
{ 
	double bda = sqrt(1- this->E2 );

	double p = sqrt(x*x + y*y);
	double t = atan2(z, p*bda);
	double st = sin(t);
	double ct = cos(t);
	double b = atan2( z+ this->E2 * this->A / bda*st*st*st, p - this->E2 * this->A *ct*ct*ct);
	double l = atan2(y, x);

	double sb = sin(b);
	double rn = this->A / sqrt(1-this->E2*sb*sb);
	double h = p/cos(b) - rn;

	Double3Point xyz;
	xyz.x = b/this->RD;
	xyz.y = l/this->RD;
	xyz.z = h;

	return xyz;
}
