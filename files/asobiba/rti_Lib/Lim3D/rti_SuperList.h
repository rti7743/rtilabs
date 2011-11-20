#ifndef __TSUPER_LIST_SERVER
#define __TSUPER_LIST_SERVER

#include "rti_SuperListSrc.h"

class __TSuperListClient;

template <class __ALLOC_DATA>
class __TSuperListServer : public __TSuperListServerSrc<__ALLOC_DATA>
{
public:
	//デストラクタ
	virtual ~__TSuperListServer(){}
//初期化.
	virtual void Init()
	{
		m_IsClear = false;
		__TSuperListServerSrc<__ALLOC_DATA>::Init();
	}
//サービス.
	//データの論理的な削除.
	virtual void Del(__ALLOC_DATA* inData)
	{
		if (m_IsClear == true) return;

		__TSuperListServerSrc<__ALLOC_DATA>::Del(inData);
	}

	//すべてのデータの破棄.
	virtual void Clear()
	{
		m_IsClear = true;

		__TSuperListServerSrc<__ALLOC_DATA>::Clear();

		m_IsClear = false;
	}

	//最初のデータの取得.
	__ALLOC_DATA* FindFirst()
	{
		m_FindFirstService = m_StartMemory;
		return m_StartMemory;
	}
	//次のデータの取得.
	__ALLOC_DATA* FindNext()
	{
		//次はないかも.
		if ( m_FindFirstService == NULL) return NULL;

		if ( m_FindFirstService == m_EndMemory)
		{//これが最後のデータ.
			m_FindFirstService = NULL;
			return m_FindFirstService;
		}
		
		//次のデータの取得.
		m_FindFirstService = (__ALLOC_DATA*)m_FindFirstService->m_Next;
		return m_FindFirstService;
	}

protected:
	//データ確保.
	virtual __ALLOC_DATA* DataNew()
	{
		register __ALLOC_DATA* theRet = new __ALLOC_DATA;
		theRet->Mount(this);
		return theRet;
	}
	//データ破棄.
	virtual  void DataDelete(__ALLOC_DATA* inData)
	{
		inData->Release();
	}
private:
	//Clear 中だったら true
	bool				m_IsClear;
	//FindFirst FindNext のワーク.
	__ALLOC_DATA*		m_FindFirstService;
};
#endif