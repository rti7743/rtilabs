// RGISNameDatabaseDisk.h: RGISNameDatabaseDisk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISNAMEDATABASEDISK_H__3C3BE105_C2C6_47C1_B3ED_079DBE7CE240__INCLUDED_)
#define AFX_RGISNAMEDATABASEDISK_H__3C3BE105_C2C6_47C1_B3ED_079DBE7CE240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

class RGISNameDatabaseDisk  
{
public:
	RGISNameDatabaseDisk();
	virtual ~RGISNameDatabaseDisk();

	unsigned short Write(const string & inFilename , const string & inName) throw (RException);

	static void test();

};

#endif // !defined(AFX_RGISNAMEDATABASEDISK_H__3C3BE105_C2C6_47C1_B3ED_079DBE7CE240__INCLUDED_)
