// RRegistry.h: RRegistry クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RREGISTRY_H__85B69393_4178_4041_B708_0FAF190B2DED__INCLUDED_)
#define AFX_RREGISTRY_H__85B69393_4178_4041_B708_0FAF190B2DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

class RRegistry  
{
public:
	RRegistry()
	{
		Key = NULL;
	}
	virtual ~RRegistry()
	{
		Close();
	}

	void Open(HKEY inHKey , const string & inSubKey , REGSAM inDesired) throw (RWin32Exception)
	{
		Close();

	#ifndef _WIN32_WCE
		LONG ret = RegOpenKeyEx(inHKey , inSubKey.c_str() , 0 , inDesired , &Key);
	#else
		MY_USES_CONVERSION;
		LONG ret = RegOpenKeyEx(inHKey , _A2W(inSubKey.c_str()) , 0 , inDesired , &Key);
	#endif
		if ( ret != ERROR_SUCCESS )
		{
			throw RWin32Exception(ret , "レジストリ(%s)がオープンできません" , inSubKey);
		}
	}
	void Close()
	{
		if (Key != NULL)
		{
			RegCloseKey(Key);
			Key = NULL;
		}
	}

	//参照.
	const string QueryValue(const string & inName , DWORD * outType) const
	{
	#ifndef _WIN32_WCE
		unsigned long bufferSize = 1024;
		unsigned char buffer[1024];
		LONG ret = RegQueryValueEx(Key , inName.c_str() , 0 , outType , buffer , &bufferSize);
		if ( ret != ERROR_SUCCESS )
		{
			throw RWin32Exception(ret , "レジストリキー(%s)が取得できません" , inName);
		}
		return (char*)buffer;
	#else
		MY_USES_CONVERSION;
		unsigned long bufferSize = 1024;
		unsigned char buffer[1024];
		LONG ret = RegQueryValueEx(Key , _A2W(inName.c_str()) , 0 , outType , buffer , &bufferSize);
		if ( ret != ERROR_SUCCESS )
		{
			throw RWin32Exception(ret , "レジストリキー(%s)が取得できません" , inName);
		}
		return (char*)(buffer);
	#endif
	}

	
private:
	HKEY	Key;
};

#endif // !defined(AFX_RREGISTRY_H__85B69393_4178_4041_B708_0FAF190B2DED__INCLUDED_)
