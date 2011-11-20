// RStdioFile.h: RStdioFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTDIOFILE_H__76E8CC3C_3C3C_4B05_9C45_B3EC2ED04D96__INCLUDED_)
#define AFX_RSTDIOFILE_H__76E8CC3C_3C3C_4B05_9C45_B3EC2ED04D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"

class RStdioFile  
{
public:
	RStdioFile();
	virtual ~RStdioFile();

	//開く.
	void Open(string inFilename,string inMode) throw (RIOCannotOpenException);

	//閉じる.
	void Close();

	//読み込む
	string ReadLine() throw (RIOCannotReadException);

	//書き込む
	void WriteLine(string inWrittingString) throw (RIOCannotWriteException);

	//おわりっすか?
	bool IsEof();
private:
	FILE	*FilePointer;
};

#endif // !defined(AFX_RSTDIOFILE_H__76E8CC3C_3C3C_4B05_9C45_B3EC2ED04D96__INCLUDED_)
