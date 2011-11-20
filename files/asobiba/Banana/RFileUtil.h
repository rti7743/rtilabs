// RFileUtil.h: RFileUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEUTIL_H__845FF38E_E32B_4543_BC6D_6BD2FCD1B04D__INCLUDED_)
#define AFX_RFILEUTIL_H__845FF38E_E32B_4543_BC6D_6BD2FCD1B04D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RException.h"

class RFileUtil  
{
public:
	RFileUtil();
	virtual ~RFileUtil();

	//行数を数える.
	static int LineCount(const string & inFilename) throw(RException);

	static void test();
};

#endif // !defined(AFX_RFILEUTIL_H__845FF38E_E32B_4543_BC6D_6BD2FCD1B04D__INCLUDED_)
