// NativeSupportPicture.h: NativeSupportPicture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NATIVESUPPORTPICTURE_H__89AD731F_F5A3_4CAA_B241_8E1EE8807C88__INCLUDED_)
#define AFX_NATIVESUPPORTPICTURE_H__89AD731F_F5A3_4CAA_B241_8E1EE8807C88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include <ole2.h>
#include <olectl.h>
#include "RIOCannotOpenException.h"
#include "RNotSupportException.h"

class NativeSupportPicture  
{
public:
	NativeSupportPicture();
	virtual ~NativeSupportPicture();

	//ロード
	//失敗は RNotSupportException
	void Load(const string inFilename) throw();

	//イメージのサイズの取得
	void getImageSize(LONG *outWidth,LONG *outHeight) const;

	//イメージをデバイスコンテキストに描画
	//失敗は RNotSupportException
	void getImage(HDC ioMemDC , LONG inWidth,LONG inHeight) const throw();

private:
	IPicture*		OlePicture;
	HGLOBAL			Bm;
};

#endif // !defined(AFX_NATIVESUPPORTPICTURE_H__89AD731F_F5A3_4CAA_B241_8E1EE8807C88__INCLUDED_)
