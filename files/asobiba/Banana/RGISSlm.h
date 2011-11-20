// RGISSlm.h: RGISSlm クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_)
#define AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISKeiidoData.h"

//多分、緯度、経度を距離であらわしているやつだと思うけど、
//値がでかすぎて ulong でも収まらないので、縮めこみます。
//まぁ、日本さえ表示できればあとはどうでもいいので、いいんぢゃないの?(w
const __int64  GLOBAL_X_SCAL	=	5000000000;
const __int64  GLOBAL_Y_SCAL	=	 800000000;


class RGISMathZantei
{
public:
	RGISMathZantei()
	{
		this->Scale = 0;
	}
	void Create(int inScale)
	{
		ASSERT(inScale >= 100);
		this->Scale = inScale;
	}

	//25000空間データ地図の座標系を 
	//ulong に収まるように /100 して 10cm の精度に落とします。
	//ナビケーションなので別に問題ないはず
	UPOINT ConvertScale(unsigned __int64 inX,unsigned __int64 inY) const;
	//25000 空間データを経緯度に変換.
	static  void Kukan25000ToKeiido(__int64 inX, __int64 inY , RGISKeiidoData * ioToukei , RGISKeiidoData * ioHokui) ;
	//上の逆変換.
	static  UPOINT RevKukan25000ToKeiido(const RGISKeiidoData * inToukei ,const RGISKeiidoData * inHokui) ;
	static  unsigned long ConvertDisplayScalX(unsigned long ioX) 
	{
		return (ioX - 0) ;
	}
	static unsigned  long ConvertDisplayScalY(unsigned long ioY) 
	{
		return (0xffffffff - (((ioY) - 0)  ));
	}

private:
	int		Scale;
};

class RGISSlm  
{
public:
	RGISSlm();
	virtual ~RGISSlm();

	void Create(const string & inFilename,int inScale) throw(RException);

	//グローバル座標系に変換します。
	UPOINT ConvertGlobal(long x , long y) const
	{
		RGISMathZantei zantei;
		zantei.Create(this->Scale);

		return zantei.ConvertScale(X + x , Y + y);
	}
	//スケールを変更し、ディスプレイ座標にします。
	bool IsHit(const URECT * inDisplay) const;

	int getKey() const
	{
		return Key;
	}


	//25000空間データ地図の座標系から UTM に変換します。
	//また、このとき ulong に収まるように /100 して 10cm の精度に落とします。
	//ナビケーションなので別に問題ないはず
//	static UPOINT RGISSlm::Convert25000ToUTM(unsigned __int64 inX,unsigned __int64 inY);

	static void test();


private:
	//ファイル名は必ず 01234.slm のような数字なるのでそれを取得.
	int ParseKey(const string & inFilename) const;
private:
	//国土地理院の実座標
	__int64	X;
	__int64	Y;
	//ディスプレイに表示するときの座標に変換したもの.
	int		Key;

	int		Scale;
};

#endif // !defined(AFX_RGISSLM_H__BE6916DB_FE84_48DE_A5F5_6BF6E2D80382__INCLUDED_)
