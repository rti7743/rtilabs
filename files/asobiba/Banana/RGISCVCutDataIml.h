// RGISCVCutDataIml.h: RGISCVCutDataIml クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_)
#define AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStdioFile.h"

class RGISCVCutDataIml  
{
public:
	RGISCVCutDataIml();
	virtual ~RGISCVCutDataIml();

	//ヘッダーを書き込まなければ成らないときに呼ばれる.
	//この中で、 ioFile に対して適切なデータを fwrite してください。
	//間違っても ioFileを close したり seekしたりしないこと!!
	virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount) = 0;
};


#endif // !defined(AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_)
