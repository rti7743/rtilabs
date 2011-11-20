// RGISCvLight.h: RGISCvLight クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCVLIGHT_H__56C1CEA9_F958_4FA4_9AEF_E5E197CE497D__INCLUDED_)
#define AFX_RGISCVLIGHT_H__56C1CEA9_F958_4FA4_9AEF_E5E197CE497D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISNode.h"
#include "RGISSlp.h"

class RGISCvLight  
{
public:
	RGISCvLight();
	virtual ~RGISCvLight();

	bool DoLightAndNode(const RGISNode * inNode , const RGISNodeIntArray * inNodeArray , 
						 const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const;
	bool DoLight(const RGISSlp * inSlp,const GISINTArray* inSrc , GISINTArray* outDest) const;
private:
	//inSrc と inDest の2点を通る直線上に inTarget がありますか?
	bool IsLineOn(const UPOINT inSrc , const UPOINT inTarget , const UPOINT inDest ) const;
	//このノード郡に自分は含まれていますか?
	bool isNode(	const RGISNode * inNode , const RGISNodeIntArray * inNodeArray ,
								const RGISSlp * inSlp , const UPOINT srcPoint ) const;
};

#endif // !defined(AFX_RGISCVLIGHT_H__56C1CEA9_F958_4FA4_9AEF_E5E197CE497D__INCLUDED_)
