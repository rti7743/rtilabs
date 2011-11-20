// RGIS2500Head.cpp: RGIS2500Head クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGIS2500Head.h"
#include "RGISSlm.h"
#include "Sec2Utm.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS2500Head::RGIS2500Head()
{
	this->Scale = 0;
}

RGIS2500Head::~RGIS2500Head()
{

}

void RGIS2500Head::Create(const char * inStr, int inScale) throw(RException)
{
	//FH
	//図名
	//ファイル名 
	//レコード総数
	//要素の総数
	//座標系フラグ
	//座標系番号
	//数値化区分
	//図郭左下角のＸ座標
	//図郭左下角のＹ座標
	//図郭右上角のＸ座標
	//図郭右上角のＹ座標
	//図郭左上角のＸ座標
	//図郭左上角のＹ座標
	//図郭右下角のＸ座標
	//図郭右下角のＹ座標

	int c = 
		sscanf(inStr , "FH,%128[^,],%256[^,],%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
			Zumai , Filename , &Record , &Count, &ZahyoFlg , &ZahyoNumber , &Kubun ,
			&LowerLeft.x , &LowerLeft.y ,
			&UpperRight.x , &UpperRight.y ,
			&UpperLeft.x , &UpperLeft.y ,
			&LowerRight.x , &LowerRight.y );
	if ( c != 15 )
	{
		throw RException(string("") + "ヘッダのパースに失敗しました これだけの数しかパースで着ませんでした:" + num2str(c));
	}
	if ( ZahyoNumber <= 0 || ZahyoNumber > 19)
	{
		throw RException(string("") + "ヘッダのパースに失敗しました 座標系は 1 ～ 19 の範囲でなければいけません");
	}
	if ( ZahyoFlg !=2 )
	{
		throw RException(string("") + "ヘッダのパースに失敗しました 座標系は 2 でなければなりません");
	}

	this->Scale = inScale;
}


#include "RGISMath.h"

UPOINT RGIS2500Head::convertXY(double x,double y) const
{
	ASSERT( this->Scale >= 100);

	DoublePoint dxy;
	RGISMath math;
	dxy = math.JMG2DMS(	(x + LowerLeft.x) ,
						(y + LowerLeft.y) ,
						ZahyoNumber,
						&RGISMath::WGS84
				);
	return LtoU( math.KeiidoTo( dxy.x , dxy.y ,this->Scale) );
}


void RGIS2500Head::test()
{
	//テストは破棄.
}

