// RGISGPSLog.h: RGISGPSLog クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISGPSLOG_H__18D8A7C5_E251_4F6C_8668_8575EC186DD2__INCLUDED_)
#define AFX_RGISGPSLOG_H__18D8A7C5_E251_4F6C_8668_8575EC186DD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStdioFile.h"

class RGISGPSLog
{
public:
	enum GPS_LOGTYPE
	{
		GPS_LOGTYPE_NONE	= 0,
		GPS_LOGTYPE_NMA0183	= 1,
		GPS_LOGTYPE_POT	= 2,
	};

	void Create(GPS_LOGTYPE inLogType , const string & inLogDirectory)
	{
		this->LogType = inLogType;
		this->LogDirectory = inLogDirectory;
	}

	void Logout(const char * inLog) const;
private:
	const string getFilename() const;
private:
	GPS_LOGTYPE LogType;
	string LogDirectory;
};

#endif // !defined(AFX_RGISGPSLOG_H__18D8A7C5_E251_4F6C_8668_8575EC186DD2__INCLUDED_)
