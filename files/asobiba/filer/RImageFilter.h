// RImageFilter.h: RImageFilter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGEFILTER_H__18628813_51BE_442A_AE3D_DD4E66B5F956__INCLUDED_)
#define AFX_RIMAGEFILTER_H__18628813_51BE_442A_AE3D_DD4E66B5F956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include <objbase.h>
#include <WTYPES.H>
#import "C:\WINNT\System32\dxtmsft.dll" named_guids exclude("RemotableHandle","wireHDC","__MIDL_IWinTypes_0009") 
#include "RImage.h"

#include "RNotSupportException.h"

class RImageFilter  
{
public:
	RImageFilter();
	virtual ~RImageFilter();

	void Create(const RImage* inImage) throw();

private:

};

#endif // !defined(AFX_RIMAGEFILTER_H__18628813_51BE_442A_AE3D_DD4E66B5F956__INCLUDED_)
