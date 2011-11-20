// RNeoIStreamble.h: RNeoIStreamble クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNEOISTREAMBLE_H__CB53F5A1_DDB7_46B4_BA25_EB6DD559DBB7__INCLUDED_)
#define AFX_RNEOISTREAMBLE_H__CB53F5A1_DDB7_46B4_BA25_EB6DD559DBB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"

//ストリームのユーティリティを使用できることをあらわすための
//純粋仮想クラス
//別に template でやってもいいけど、サイズが大きくなるといやなんで。。。
class RNeoIStreamble  
{
public:
	virtual int Read(char* outBuffer , int inBuferSize) = 0;
	virtual int Write(const char* inBuffer , int inBuferSize) = 0;
};


#endif // !defined(AFX_RNEOISTREAMBLE_H__CB53F5A1_DDB7_46B4_BA25_EB6DD559DBB7__INCLUDED_)
