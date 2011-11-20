// RGISSmallParentImp.h: RGISSmallParentImp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPARENTIMP_H__52B6DFB0_C41D_4343_B11A_D7CA6CFE07C8__INCLUDED_)
#define AFX_RGISSMALLPARENTIMP_H__52B6DFB0_C41D_4343_B11A_D7CA6CFE07C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

class RGISSmallParentImp  
{
public:
	RGISSmallParentImp();
	virtual ~RGISSmallParentImp();

	//シンボルの解決
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) = 0;
	//データのセット
	virtual void setData(unsigned long inBlock ,char* ioBuffer  , char * ioUserDataBuffer) = 0;
	//ユーザーデータのリロード.
	virtual void UserDataReload(char* ioUserDataBuffer) = 0;

};

#endif // !defined(AFX_RGISSMALLPARENTIMP_H__52B6DFB0_C41D_4343_B11A_D7CA6CFE07C8__INCLUDED_)
