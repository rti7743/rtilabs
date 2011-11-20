// RListenner.h: RListenner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLISTENNER_H__3F93E71B_BFB8_47CD_A73E_755BE63A88CA__INCLUDED_)
#define AFX_RLISTENNER_H__3F93E71B_BFB8_47CD_A73E_755BE63A88CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<typename _T>
class RListenner
{
public:
	virtual void Fire(const _T * inChangeObject) const = 0;
};


#endif // !defined(AFX_RLISTENNER_H__3F93E71B_BFB8_47CD_A73E_755BE63A88CA__INCLUDED_)
