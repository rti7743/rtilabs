#ifndef __TSUPER_LIST_SERVER_SRC
#define __TSUPER_LIST_SERVER_SRC

#define __RTI_CHECK

#include <stdio.h>

template <class __ALLOC_DATA>
class __TSuperListServerSrc
{
public:
	__TSuperListServerSrc()
	{
		Init();
	};
	virtual ~__TSuperListServerSrc()
	{
		Clear();
	};

//初期化.
	virtual void Init()
	{
		//データの先頭.
		m_StartMemory	=	NULL;
		//データの終わり
		m_EndMemory		=	NULL;
		//データの件数. m_Size 分までです.
		m_Size			=	0;
	}
//サービス.
	//破棄
	virtual void Release()
	{
		delete this;
	}
	//データの確保.
	virtual __ALLOC_DATA* New()
	{
		//データの作成.
		__ALLOC_DATA*	theData = DataNew();
		if (theData== NULL) return NULL;

		theData->m_Next = NULL;

		if (m_StartMemory == NULL)
		{//先頭がなかったら、作成.
			m_StartMemory	=	theData;
			m_EndMemory		=	theData;
			__RTI_CHECK(m_Size == 0);
		}
		else
		{//2件目以降
			m_EndMemory->m_Next = theData;
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
		}
		//データが増えた.
		m_Size++;

		return theData;
	}
	//データの物理的な削除.
	virtual void RealDel(__ALLOC_DATA* inData)
	{
		Del(inData);

		DataDelete( inData );
	}
	//データの論理的な削除.
	virtual void Del(__ALLOC_DATA* inData)
	{
		//特別処理.
		m_Size--;
		if (m_StartMemory == inData)
		{//最初のデータ.
			if (m_EndMemory == inData)
			{//最初であり最後である.
				__RTI_CHECK(m_Size == 0);
				Init();
			}
			else
			{//次のデータを先頭にする. 次は保証されます.
				m_StartMemory = (__ALLOC_DATA*)inData->m_Next;
			}
		}
		else if (m_EndMemory == inData)
		{//最後のデータの一つ前を算出. (一つ前があることは保証される.)
			m_EndMemory = GetPrev(m_EndMemory);
		}
		else
		{//ターゲットの一つ前も 一つ先も保証される.
			((__ALLOC_DATA*)GetPrev(inData))->m_Next = inData->m_Next;
		}
	}
	//すべてのデータの破棄.
	virtual void Clear()
	{
		//データ.
		__ALLOC_DATA*		theIT;
		//データ.
		__ALLOC_DATA*		theITNext;
		//データの数.
		unsigned int		theDataNum;

		theDataNum = m_Size;
		theIT = m_StartMemory;

		//すべて破棄.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			theITNext = (__ALLOC_DATA*)theIT->m_Next;

			DataDelete( theIT );

			theIT = theITNext;
		}
		Init();
	}
	//現在の論理的サイズの取得.
	virtual unsigned int GetCount(){			return m_Size;	}
	//一つ前のリストを取得.
	virtual __ALLOC_DATA* GetPrev(__ALLOC_DATA* inData)
	{
		//データ.
		__ALLOC_DATA*		theIT;
		//データの数.
		unsigned int		theDataNum;

		theDataNum = m_Size;
		theIT = m_StartMemory;

		//すべてに問い合わせ.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			if ( ((__ALLOC_DATA*)(theIT->m_Next)) == inData)
			{
				return theIT;
			}
			theIT = (__ALLOC_DATA*)theIT->m_Next;
		}
		return NULL;
	}
	//指定場所のデータを取得.
	virtual __ALLOC_DATA* At(unsigned int inNo)
	{
		if (inNo >= m_Size) return NULL;

		//データ.
		__ALLOC_DATA*		theIT;
		//データの数.
		unsigned int		theDataNum;

		theDataNum = inNo;
		theIT = m_StartMemory;

		//すべて破棄.
		for( ; theDataNum > 0 ; theDataNum--)  
		{
			theIT = (__ALLOC_DATA*)theIT->m_Next;
		}
		return theIT;
	}

	//デバッグ.
	void Test()
	{
		puts("*******");
		printf("m_StartMemory		%p\r\n",m_StartMemory);
		printf("m_EndMemory			%p\r\n",m_EndMemory);
		printf("m_Size				%u\r\n",m_Size);
		puts("*******");
	}
protected:
	//データ確保.
	virtual __ALLOC_DATA* DataNew()
	{
		return new __ALLOC_DATA;
	}
	//データ破棄.
	virtual  void DataDelete(__ALLOC_DATA* inData)
	{
		delete inData;
	}

	//データの先頭.
	__ALLOC_DATA*		m_StartMemory;
	//データの論理的な終わり(ユーザーに対してお知らせする確保している量)
	__ALLOC_DATA*		m_EndMemory;
	//データの件数. m_Size 分までです.
	unsigned int		m_Size;
};

#endif
