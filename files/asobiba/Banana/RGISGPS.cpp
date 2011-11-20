// RGISGPS.cpp: RGISGPS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISGPS.h"
#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//改行コードまで
class StreamCheckerForReturnCodeWithThread : public StreamChecker
{
public:
	StreamCheckerForReturnCodeWithThread(const RThread* inThread)
	{
		Separater = 0;
		ConectStart = 0;
		Thread = inThread;
	}
	~StreamCheckerForReturnCodeWithThread()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		for (int i = inAllReadyRead; i < inNowRead ; i ++) 
		{
			//スレッドが終了したい場合はとにかく終了.
			if ( !Thread->IsHoldUp()) return false;

			if ( inBuffer[i] == '\n' )
			{
				Separater = i - 1;
				ConectStart = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
	const RThread* Thread;
};


RGISGPS::RGISGPS()
{
}

RGISGPS::~RGISGPS()
{

}

void RGISGPS::Create(int port ,int speed ,int type ,int logType ,const string & logDir )
{
	Stop();

	NMEA0183LogRingBuffer.Create(NMEA0183_RESERVE_BUFFER_SIZE);
	LogFile.Create( (RGISGPSLog::GPS_LOGTYPE)logType , logDir );

	try
	{
		NMEA0183.Create(100);					//あとで
		SerialPort.Open(port , speed   );
	}
	catch(RException e)
	{
		throw ;
	}
}


//GPSを制御するためのメッセージを送信します.
void RGISGPS::SendGPSControlMessage(const string & inMessage) 
{
	RAutoLock al(&Lock);
	SerialPort.Write(inMessage.c_str() , inMessage.size() );
}


bool RGISGPS::getParam(LPOINT * xy, int * outSpeed , int * outDirection ,
					   SatellitesState outSatellitesState[SATELLITES_MAX]) const
{
	RAutoLock al(&Lock);

	return 
		NMEA0183.getParam(xy , outSpeed , outDirection ) &&	NMEA0183.getSatellites(outSatellitesState);
}

void RGISGPS::HoldUp()
{
	SerialPort.Close();

	RThread::HoldUp();
}

//ログを取得.
const string RGISGPS::getTailLog() const
{
	RAutoLock al(&Lock);

	return this->NMEA0183LogRingBuffer.getBuffer();
}



unsigned int RGISGPS::Run()
{
	char oneBuffer[NMEA0183_RESERVE_BUFFER_SIZE + 1];

	

	//一行筒取り出します.
	RStream stream;
	stream.Create(&SerialPort, NMEA0183_RESERVE_BUFFER_SIZE * 2);

	//一行の終わりの検出.
	StreamCheckerForReturnCodeWithThread lineEnd(this);

	try
	{
		while( IsHoldUp() )
		{
			stream.Read(oneBuffer , NMEA0183_RESERVE_BUFFER_SIZE , &lineEnd);
			if (!IsHoldUp()) break;

			//終端を入れる.
			oneBuffer[lineEnd.getNext() + 1] = '\0';
			LogFile.Logout(oneBuffer);

			try
			{
				//解析.
				RAutoLock al(&Lock);

				NMEA0183LogRingBuffer.Push(oneBuffer , lineEnd.getNext() );
				NMEA0183.Analize(oneBuffer);
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
			}
		}
	}
	catch(RException e)
	{
	}

	SerialPort.Close();

	return 1;
}

