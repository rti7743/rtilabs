// RResorceImageLoad.h: RResorceImageLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRESORCEIMAGELOAD_H__6E4C9956_382D_4D65_8075_C15B98F1919E__INCLUDED_)
#define AFX_RRESORCEIMAGELOAD_H__6E4C9956_382D_4D65_8075_C15B98F1919E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDIBBitmap.h"
#include "RWin32Exception.h"

class RResorceImageLoad  
{
public:
	RResorceImageLoad();
	virtual ~RResorceImageLoad();

	void Load(RDIBBitmap * outImage, const string & inResourceName) throw(RWin32Exception);
	void LoadFile(RDIBBitmap * outImage, const string & inFilename) throw(RException);
};

#endif // !defined(AFX_RRESORCEIMAGELOAD_H__6E4C9956_382D_4D65_8075_C15B98F1919E__INCLUDED_)
