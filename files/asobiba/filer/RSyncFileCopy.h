// RSyncFileCopy.h: RSyncFileCopy クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSYNCFILECOPY_H__F1A7474F_33DE_4FA5_A918_4EEE72535D3A__INCLUDED_)
#define AFX_RSYNCFILECOPY_H__F1A7474F_33DE_4FA5_A918_4EEE72535D3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "Rfile.h"
#include "Rfiles.h"
#include "RLock.h"

class RSyncFileCopy  
{
public:
	RSyncFileCopy();
	virtual ~RSyncFileCopy();
	
	void Run(string inReadPath , Rfile* ioRead,
			 string inWritePath , Rfile* ioWrite) throw();

	enum	//enum hack!
	{
		BUFFER_SIZE=65535,
		BUFFER_COUNT=3,
	};

	static void Test();
	static void FileCheck(const char* inSrc,const char* inDest);
	static void SuperCopy(const char* inSrc,const char* inDest);

private:

//	char	Buffer[BUFFER_COUNT][BUFFER_SIZE];
	char*	Buffer[BUFFER_COUNT];
};

#endif // !defined(AFX_RSYNCFILECOPY_H__F1A7474F_33DE_4FA5_A918_4EEE72535D3A__INCLUDED_)
