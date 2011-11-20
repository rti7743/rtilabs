// RDataLoad.h: RDataLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDATALOAD_H__47AD9145_DDCB_41E8_AAF7_10FF6E08DC86__INCLUDED_)
#define AFX_RDATALOAD_H__47AD9145_DDCB_41E8_AAF7_10FF6E08DC86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"

class RDataLoad  
{
public:
	RDataLoad();
	virtual ~RDataLoad();

	static string Load(string inSetupFilename,string inSection,string inKey) 
				throw(RNotFoundException,RIOCannotOpenException,RIOCannotReadException);
	static void Save(string inSetupFilename,string inSection,string inKey , string inData) 
				throw(RNotFoundException,RIOCannotOpenException,RIOCannotWriteException);
};

#endif // !defined(AFX_RDATALOAD_H__47AD9145_DDCB_41E8_AAF7_10FF6E08DC86__INCLUDED_)
