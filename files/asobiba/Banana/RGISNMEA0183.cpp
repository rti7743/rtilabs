// RGISNMEA0183.cpp: RGISNMEA0183 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISNMEA0183.h"
#include "RToken.h"
#include "RGISMath.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////




RGISNMEA0183::RGISNMEA0183()
{
	Alive = false;
	Scale = 0;

	memset(State , 0 , sizeof(SatellitesState) * SATELLITES_MAX);
}

RGISNMEA0183::~RGISNMEA0183()
{

}

void RGISNMEA0183::Create(int inScale)
{
	ASSERT(inScale >= 100);

	this->Scale = inScale;
}

void RGISNMEA0183::ConvertXY(const char* inB,const char* inL )
{

	//前と同じなら何もしない.
	if ( (strcmp( OldB.c_str() , inB) == 0) || (strcmp( OldL.c_str() , inL) == 0)  ) return ;

	OldB = inB;
	OldL = inL;

	RGISMath math;
	this->XY = math.GPSBLTo( atof(inB)  , atof(inL) , this->Scale);

}

void RGISNMEA0183::Analize(const char* inBuffer) throw(RException)
{
	//先頭に $ があるはず.
	const char* nmea = strchr(inBuffer , '$');
	if (nmea == NULL) return ;	//junk

	nmea ++;	//$の読み飛ばし.

	RToken token;
	token.setToken(nmea , ',');

	const char * w1;
	const char * w2;
	const char * w3;
	const char * w4;
	const char * type = token.nextToken();
	
	if ( strcmp(type , "GPGGA" ) == 0)
	{//GGA
		//測位時刻（UTC）
		token.nextToken();

		w1 = token.nextToken();
		w2 = token.nextToken();
		w3 = token.nextToken();
		w4 = token.nextToken();

		if (w1 == NULL || w2 == NULL || w3 == NULL || w4 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GGA 解析中にエラー 緯度が取得できません");
		}
		if (*w1 != '\0' && *w2 != '\0' && *w3 != '\0' && *w4 != '\0')
		{
			ConvertXY( w1 , w3 );
		}
		//クオリティ
		w1 = token.nextToken();
		if (w1 == NULL)				  
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GGA 解析中にエラー クオリティが取得できません");
		}
		if (*w1 != '\0')
		{
			if (atoi(w1) == 0)			Alive = false;
			else						Alive = true;
		}

		//以下すべて無視!
	}
	else if ( strcmp(type , "GPGLL" ) == 0)
	{//GLL
		w1 = token.nextToken();
		w2 = token.nextToken();
		w3 = token.nextToken();
		w4 = token.nextToken();

		if (w1 == NULL || w2 == NULL || w3 == NULL || w4 == NULL) 
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GGA 解析中にエラー 緯度が取得できません");
		}
		if (*w1 != '\0' && *w2 != '\0' && *w3 != '\0' && *w4 != '\0')
		{
			ConvertXY( w1 , w3 );
		}
		//測位時刻（UTC）
		token.nextToken();
		//ステータス
		w1 = token.nextToken();
		if (w1 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GLL 解析中にエラー ステータスが取得できません");
		}
		if (*w1 != '\0')
		{
			if (w1[0] == 'V')			Alive = false;
			else						Alive = true;
		}
	}
	else if ( strcmp(type , "GPRMC" ) == 0)
	{//RMC
		//測位時刻（UTC）
		token.nextToken();
		//ステータス
		w1 = token.nextToken();
		if (w1 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 RMC 解析中にエラー ステータスが取得できません");
		}
		if (*w1 != '\0')
		{
			if (w1[0] == 'V')			Alive = false;
			else						Alive = true;
		}
		w1 = token.nextToken();
		w2 = token.nextToken();
		w3 = token.nextToken();
		w4 = token.nextToken();

		if (w1 == NULL || w2 == NULL || w3 == NULL || w4 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GGA 解析中にエラー 緯度が取得できません");
		}
		if (*w1 != '\0' && *w2 != '\0' && *w3 != '\0' && *w4 != '\0')
		{
			ConvertXY( w1 , w3 );
		}
		//対地速度（ノット）
		w1 = token.nextToken();
		if (w1 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 RMC 解析中にエラー 対地速度（ノット）が取得できません");
		}
		if (*w1 != '\0')
		{
			if ( strcmp(OldSpeed.c_str(), w1 ) != 0 )
			{
				OldSpeed = w1;
				this->Speed = atof(w1) * 1852;
			}
		}

		//進行方向（度，真北）
		w1 = token.nextToken();
		if (w1 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 RMC 解析中にエラー 進行方向（度，真北）が取得できません");
		}
		if (*w1 != '\0')
		{
			this->Direction = atoi(w1);
		}
	}
	else if ( strcmp(type , "GPVTG" ) == 0)
	{//VTG
		//進行方向（度，真北）
		w1 = token.nextToken();
		if (w1 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 VTG 解析中にエラー 進行方向（度，真北）が取得できません");
		}
		if (*w1 != '\0')
		{
			this->Direction = atoi(w1);
		}
		//磁北に対する進行方向 （度）
		token.nextToken();
		//対地速度（ノット）
		w1 = token.nextToken();
		w2 = token.nextToken();
		if (w1 == NULL || w2 == NULL)
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 VTG 解析中にエラー 対地速度（ノット）が取得できません");
		}
		if (*w1 != '\0' && *w2 != '\0')
		{
			if ( strcmp(OldSpeed.c_str(), w1 ) != 0 )
			{
				OldSpeed = w1;
				this->Speed = atof(w1) * 1852;
			}
		}
		//対地速度（km/h）
		w1 = token.nextToken();
		w2 = token.nextToken();
		//モード；A = 単独測位，D = DGPS，N = 無効
		w1 = token.nextToken();
		if (w1 == NULL)				 
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 VTG 解析中にエラー モードが取得できません");
		}
		if (*w1 != '\0')
		{
			if (w1[0] == 'N')			Alive = false;
			else						Alive = true;
		}
	}
	else if ( strcmp(type , "GPGSV" ) == 0)
	{//GSV
		//全メッセージ数
		token.nextToken();
		//メッセージ番号
		w1 = token.nextToken();
		if (w1 == NULL)			
		{
			throw RException(EXCEPTIONTRACE + "NMEA0183 GSV 解析中にエラー メッセージ番号が取得できません");
		}
		if (*w1 != '\0')
		{
			//最初のメッセージの場合は、衛星情報をすべて初期化する.
			if ( atoi(w1) == 1)
			{
				memset(State , 0 , sizeof(SatellitesState) * SATELLITES_MAX);
			}
		}
		//受信可能衛星数
		token.nextToken();
		//各種衛星の読み込み(最大4つの可変長) 
		const char * w3;
		const char * w4;
		for(int i = 0; i < 4 ; i++)
		{
			w1 = token.nextToken();
			w2 = token.nextToken();
			w3 = token.nextToken();
			w4 = token.nextToken();

			if (w1 == NULL || w2 == NULL || w3 == NULL || w4 == NULL) break;

			if (*w1 != '\0' && *w2 != '\0' && *w3 != '\0' && *w4 != '\0')
			{
				//01 = 衛星番号，40 = 仰角（度），083 = 方位（度），46 = SNR（デシベル）
				int satelaliteNumber = atoi(w1) - 1;
				if (satelaliteNumber >= SATELLITES_MAX || satelaliteNumber < 0)
				{
//					throw RException(EXCEPTIONTRACE + "NMEA0183 GSV 解析中にエラー 衛星の番号が範囲外です");
					continue;	//GPSの衛星はどんどん増えるので冷害にせずに無視するようにする.
				}

				SatellitesState * ac = &this->State[satelaliteNumber];

				int elevation = atoi(w2);
				int direction = atoi(w3);
				int snr = atoi(w4);
				ac->Alive = true;
				ac->Elevation = elevation;
				ac->Direction = direction;
				ac->SNR = snr;
			}
		}
	}
}

void RGISNMEA0183::test()
{
	{
		string t = "$GPGGA,032432.746,4404.6928,N,14500.6852,E,1,06,2.0,-29.1,M,,,,0000*15";
		RGISNMEA0183 nmea0183;
		nmea0183.Create(100);
		nmea0183.Analize(t.c_str() );

		LPOINT p;
		int speed;
		int direction;
		nmea0183.getParam( &p , &speed , &direction);
		ASSERT(p.x == 52202625);
		ASSERT(p.y == 4279098267);




		t = "$GPGSV,3,1,09,29,76,326,37,26,68,283,39,10,60,106,33,23,53,105,*71\r\n";
		nmea0183.Analize(t.c_str() );
		t = "$GPGSV,3,2,09,17,39,185,39,08,35,052,32,28,15,100,,24,10,168,14*71\r\n";
		nmea0183.Analize(t.c_str() );
		t = "$GPGSV,3,3,09,09,09,204,39*4C\r\n";
		nmea0183.Analize(t.c_str() );

		SatellitesState satellitesState[SATELLITES_MAX];
		nmea0183.getSatellites(satellitesState);

		ASSERT( satellitesState[7].Alive );
		ASSERT( satellitesState[7].Elevation == 35 );
		ASSERT( satellitesState[7].Direction == 52 );
		ASSERT( satellitesState[7].SNR == 32 );

		ASSERT( satellitesState[8].Alive );
		ASSERT( satellitesState[8].Elevation == 9 );
		ASSERT( satellitesState[8].Direction == 204 );
		ASSERT( satellitesState[8].SNR == 39 );

		ASSERT( satellitesState[9].Alive );
		ASSERT( satellitesState[9].Elevation == 60 );
		ASSERT( satellitesState[9].Direction == 106 );
		ASSERT( satellitesState[9].SNR == 33 );

		ASSERT( satellitesState[16].Alive );
		ASSERT( satellitesState[16].Elevation == 39 );
		ASSERT( satellitesState[16].Direction == 185 );
		ASSERT( satellitesState[16].SNR == 39 );

		ASSERT( satellitesState[22].Alive );
		ASSERT( satellitesState[22].Elevation == 53 );
		ASSERT( satellitesState[22].Direction == 105 );
		ASSERT( satellitesState[22].SNR == 0 );

		ASSERT( satellitesState[23].Alive );
		ASSERT( satellitesState[23].Elevation == 10 );
		ASSERT( satellitesState[23].Direction == 168 );
		ASSERT( satellitesState[23].SNR == 14 );

		ASSERT( satellitesState[25].Alive );
		ASSERT( satellitesState[25].Elevation == 68 );
		ASSERT( satellitesState[25].Direction == 283 );
		ASSERT( satellitesState[25].SNR == 39 );

		ASSERT( satellitesState[28].Alive );
		ASSERT( satellitesState[28].Elevation == 76 );
		ASSERT( satellitesState[28].Direction == 326 );
		ASSERT( satellitesState[28].SNR == 37 );
	}
}

