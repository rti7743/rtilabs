// RGISGPSLog.cpp: RGISGPSLog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISGPSLog.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const string RGISGPSLog::getFilename() const
{
	char buf [30 + 1];

	SYSTEMTIME time;
	::GetLocalTime(&time);
	_snprintf(buf , 30 , "\\gpslog_%04d_%02d_%02d.log" , time.wYear , time.wMonth , time.wDay);

	return LogDirectory + buf ;
}

void RGISGPSLog::Logout(const char * inLog) const
{
	if (this->LogType == GPS_LOGTYPE_NONE) return ;


	RStdioFile LogFile;
	LogFile.Open( getFilename() , "a+b");
	LogFile.fwrite(inLog , strlen(inLog) );
}