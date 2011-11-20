// RImgdecmp.h: RImgdecmp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMGDECMP_H__2CF95DF5_9954_4197_8C64_34AA54B1974D__INCLUDED_)
#define AFX_RIMGDECMP_H__2CF95DF5_9954_4197_8C64_34AA54B1974D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32_WCE

#include "comm.h"
#include "RAutoLibraryWithException.h"
#include "RConferException.h"
#include "imgdecmp.h"

class RImgdecmp
{
public:
	RImgdecmp()
	{
	}
	virtual ~RImgdecmp()
	{
	}
	void Create() throw(RException)
	{
		Imgdecmp.Create("Imgdecmp.dll");
		this->DecompressImageIndirect = (DecompressImageIndirectDef) Imgdecmp.GetProcAddress("DecompressImageIndirect");
	}

	typedef HRESULT (WINAPI *DecompressImageIndirectDef)(DecompressImageInfo *pParams);
	DecompressImageIndirectDef	DecompressImageIndirect;

private:
	RAutoLibraryWithException	Imgdecmp;
};

#endif



#endif // !defined(AFX_RIMGDECMP_H__2CF95DF5_9954_4197_8C64_34AA54B1974D__INCLUDED_)
