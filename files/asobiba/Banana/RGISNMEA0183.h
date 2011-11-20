// RGISNMEA0183.h: RGISNMEA0183 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISNMEA0183_H__C19DD491_16CE_4DD7_A291_A9A236F671E7__INCLUDED_)
#define AFX_RGISNMEA0183_H__C19DD491_16CE_4DD7_A291_A9A236F671E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISKeiidoData.h"
#include "RException.h"

struct SatellitesState
{
	bool	Alive;

	int		Elevation;	//仰角 度
	int		Direction;	//方位 度
	int		SNR;		//信号の強さ??	デシベル
};
//GPSの衛星の数...
//GPS衛星が新たに打ちあがったら増やしてください っても 2004/7 にもう一気追加されたのね。
//衛星age杉
const int SATELLITES_MAX = 29;

//NMEA0183 の解析.
//http://bg66.soc.i.kyoto-u.ac.jp/forestgps/nmea.html
class RGISNMEA0183  
{
public:
	RGISNMEA0183();
	virtual ~RGISNMEA0183();

	void Create(int inScale);
	void Analize(const char* inBuffer ) throw(RException);


	bool getParam(LPOINT * outXY, int * outSpeed , int * outDirection ) const
	{
		if (!Alive) return false;

		*outXY = XY;
		*outSpeed = Speed;
		*outDirection = Direction;

		return true;
	}
	bool getSatellites(SatellitesState outSatellitesState[SATELLITES_MAX]) const
	{
		if (!Alive) return false;

		memcpy(outSatellitesState , State , sizeof(SatellitesState) * SATELLITES_MAX);

		return true;
	}

	static void test();
private:
	void ConvertXY(const char* inB,const char* inL );


private:
	bool			Alive;	//有効ですか?
	LPOINT			XY;
	int				Speed;	//メートル 1knot = 1852m
	int				Direction ;	//進行方向 度
	SatellitesState	State[SATELLITES_MAX];	//衛星のステータス.

	
	string			OldB,OldL;
	string			OldSpeed;

	int				Scale;

};

#endif // !defined(AFX_RGISNMEA0183_H__C19DD491_16CE_4DD7_A291_A9A236F671E7__INCLUDED_)
