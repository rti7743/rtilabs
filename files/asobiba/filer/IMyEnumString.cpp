// IMyEnumString.cpp: IMyEnumString クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "IMyEnumString.h"
#include <shlobj.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

IMyEnumString::IMyEnumString(LPUNKNOWN inUnkRef,WStringList inTargetStrings)
{
	m_cRef = 0;
	m_pUnkRef = inUnkRef;

	m_iCur = 0;
	TargetStrings = inTargetStrings;

	//IMalloc 確保
	SHGetMalloc(&m_pmem);
}

IMyEnumString::~IMyEnumString()
{
	//IMalloc 解放
	m_pmem->Release();
	return;
}

STDMETHODIMP IMyEnumString::QueryInterface(REFIID riid, LPVOID *ppv)
{
	*ppv = NULL;

	if (IID_IUnknown == riid || IID_IEnumString == riid)
		*ppv = (LPVOID)this;

	if (*ppv)
	{
		((LPUNKNOWN)*ppv)->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) IMyEnumString::AddRef(void)
{
	// このクラスの参照カウントをインクリメントする
	// 親クラスもあればそれもインクリメント
	++m_cRef;
	if(m_pUnkRef) 
		m_pUnkRef->AddRef();
	return m_cRef;
}

STDMETHODIMP_(ULONG) IMyEnumString::Release(void)
{
	//IMalloc を解放.
	if (m_pmem)
		m_pmem->Release();

	// このオブジェクトを解放
	// 親があればそれも解放
	if(m_pUnkRef) 
		m_pUnkRef->Release();

	if (0L != --m_cRef)
		return m_cRef;

	delete this;
	return 0;
}



/*
 * IEnumString::Next
 *
 * 目的 -> 次のアイテムを渡す
 *
 * 引数:
 *  cstr    ULONG      pstrに返す文字列の個数
 *  pstr    LPOLESTR*  戻り値を入れるポインタ
 *  pulstr  ULONG*     実際に返した数を入れる変数へのポインタ
 *
 * 戻り値:
 *          HRESULT    成功時		S_OK
 *                     失敗時		S_FALSE
 */
STDMETHODIMP IMyEnumString::Next(ULONG cstr, LPOLESTR *pstr, ULONG *pcstr)
{
	ULONG cReturn = 0L;
	ULONG maxcount = cstr;

	*pcstr = 0L;	// ←戻り値を初期化
	*pstr = NULL;	// ←

	// このクラスが文字列を持ってなかったら失敗
	if (TargetStrings.empty() )
		return S_FALSE;

	// pcstrがNULLだったら、１個のアイテムを取得時以外は失敗
	if (!pcstr)
	{
		if (cstr != 1L)
			return E_POINTER;
	}

	int stringCount = TargetStrings.size() ;

	// リストの最後だったら失敗
	if (m_iCur >= stringCount )
		return S_FALSE;

	// 要求した数まで返せるだけ返す
	while (m_iCur < stringCount && cstr > 0)
	{
		int	size;

		// コピーする文字列の文字数を計算
		size = (wcslen( TargetStrings[m_iCur].c_str() )+1);

		// メモリ確保
		pstr[cReturn] = (WCHAR*)m_pmem->Alloc(size * sizeof(WCHAR));
		if(pstr[cReturn])
			wcscpy(pstr[cReturn], TargetStrings[m_iCur].c_str() );

		// 次のアイテムに移る
		m_iCur++;
		cReturn++;
		cstr--;
	}

	if (pcstr)
		*pcstr = cReturn;

	if (cReturn == maxcount)
		return S_OK;
	return S_FALSE;
}

/*
 * IEnumString::Skip
 *
 * 目的 -> アイテムをn個飛ばす
 *
 * 引数:
 *  cSkip  ULONG  飛ばす個数
 *
 * 戻り値:
 *  HRESULT	 成功時                        S_OK
 *			 要求した数だけとばせないとき  S_FALSE
 */
STDMETHODIMP IMyEnumString::Skip(ULONG cSkip)
{
	if (((m_iCur + cSkip) >= TargetStrings.size() ) || TargetStrings.empty() )
		return S_FALSE;

	m_iCur += cSkip;
	return S_OK;
}

/*
 * IEnumString::Reset
 *
 * 目的 -> 現在位置を最初に戻す
 *
 * 引数 -> なし
 */
STDMETHODIMP IMyEnumString::Reset(void)
{
	m_iCur = 0;
	return S_OK;
}

/*
 * IEnumString::Clone
 *
 * 目的 -> これと同じ状態のIEnumStringを作る
 *
 * 引数:
 *  ppEnum LPENUMSTRING* 作った新しいオブジェクトのポインタを受け取る変数
 */
STDMETHODIMP IMyEnumString::Clone(LPENUMSTRING *ppEnum)
{
	*ppEnum = NULL;

	// 同じのを作る
	IMyEnumString	*pNew = new IMyEnumString(m_pUnkRef, TargetStrings);

	if (!pNew)
		return E_OUTOFMEMORY;

	// 参照カウントを増やす
	pNew->AddRef();

	// 位置をそろえる
	pNew->m_iCur = m_iCur;

	*ppEnum = pNew;
	return S_OK;
}
