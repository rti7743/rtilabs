// IMyEnumString.h: IMyEnumString クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_)
#define AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include <shldisp.h>

class IMyEnumString : public IEnumString  
{
public:
	IMyEnumString(LPUNKNOWN inUnkRef, WStringList inTargetStrings);
	~IMyEnumString();

	// IUnknownインターフェイスのメソッド
	STDMETHODIMP         QueryInterface(REFIID iid, LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	// IEnumStringインターフェイスのメソッド
	STDMETHODIMP         Next(ULONG cstr, LPOLESTR *rgelt, ULONG *pcstr);
	STDMETHODIMP         Skip(ULONG celt);
	STDMETHODIMP         Reset(void);
	STDMETHODIMP         Clone(IEnumString **ppenum);

private:
	ULONG           m_cRef;     // 参照カウント
	LPUNKNOWN       m_pUnkRef;  // 参照カウントを増やすIUnknownのポインタ
	ULONG           m_iCur;     // 現在のリスト位置
	WStringList		TargetStrings;
	IMalloc			*m_pmem;	// IMallocインターフェイスのポインタ
};

#endif // !defined(AFX_IMYENUMSTRING_H__91A26DE2_E89C_4D08_8AB7_C2C5C577A562__INCLUDED_)
