// RGISSmallShareImage.h: RGISSmallShareImage クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLSHAREIMAGE_H__EFC5429F_0118_402A_9296_DB8D0A3201CB__INCLUDED_)
#define AFX_RGISSMALLSHAREIMAGE_H__EFC5429F_0118_402A_9296_DB8D0A3201CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDIBBitmap.h"

class RGISSmallShareImage  
{
public:
	RGISSmallShareImage();
	virtual ~RGISSmallShareImage();

	void Create();

	//地図上で使用するイメージの取得.
	const RDIBBitmap* getIconImage() const
	{
		return &IconImage;
	}


private:
	//地図上で使用するイメージ
	RDIBBitmap IconImage;
};

#endif // !defined(AFX_RGISSMALLSHAREIMAGE_H__EFC5429F_0118_402A_9296_DB8D0A3201CB__INCLUDED_)
