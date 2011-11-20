// RGISNameDatabase.h: RGISNameDatabase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISNAMEDATABASE_H__B4BE0C23_2D95_4079_A3ED_06AD63379113__INCLUDED_)
#define AFX_RGISNAMEDATABASE_H__B4BE0C23_2D95_4079_A3ED_06AD63379113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISJoin.h"

typedef vector<string> UniqStrings;


class RGISNameDatabase  
{
public:
	RGISNameDatabase();
	virtual ~RGISNameDatabase();

	unsigned short setName(const char* inName) throw(RException);
	const string getName(unsigned short inIndex) const;
	unsigned short getCount() const
	{
		return Strings.size();
	}

	static JoinTempFormatScript getPackInfo(const string & inDirectory , unsigned char inSimbol) ;

private:
	UniqStrings		Strings;
};

#endif // !defined(AFX_RGISNAMEDATABASE_H__B4BE0C23_2D95_4079_A3ED_06AD63379113__INCLUDED_)
