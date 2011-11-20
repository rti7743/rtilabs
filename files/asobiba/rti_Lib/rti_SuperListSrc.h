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
		//データの物理的な終わり(実際に確保している量)
		m_RealMemory	=	NULL;
		//データの論理的な終わり(ユーザーに対してお知らせする確保している量)
		m_EndMemory		=	NULL;
		//実際に確保しているサイズ.
		m_RealSize		=	0;
		//データの件数. m_Size 分までです.
		m_Size			=	0;
	}
//サービス.
	//破棄
	virtual void Release()
	{
		delete this;
	}
	//物理的なデータ量を増やす.
	virtual void RealAlloc(unsigned int inAddNum)
	{
		int i = inAddNum;
		//先頭がなかったら、作成.
		if (m_StartMemory == NULL)
		{
			m_StartMemory = DataNew();
			m_RealMemory = m_StartMemory;
			m_EndMemory	 = m_StartMemory;
			//それで現在の候補が1つへったな.
			i --;
			//念のため NULL.
			m_StartMemory->m_Next = NULL;
			m_RealMemory->m_Next = NULL;
			m_EndMemory->m_Next = NULL;
		}

		for(  ; i > 0 ; i --)
		{
			m_RealMemory->m_Next = DataNew();
			m_RealMemory = (__ALLOC_DATA*)m_RealMemory->m_Next;
		}
		//物理的に確保しているメモリを増やす.
		m_RealSize += inAddNum;
		//念のため NULL.
		m_RealMemory->m_Next = NULL;
	}

	//データの確保.
	virtual __ALLOC_DATA* New()
	{
		//すでに確保済みのメモリだったら...
		if (m_RealMemory != m_EndMemory)
		{
			//現在使用サイズを増やす.
			m_Size ++;
			//次のメモリへ. 次ぎメモリも確保済みであることは保証されている.
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
			//現在終わりのメモリを返す.
			return m_EndMemory;
		}
		//そもそものデータがなかったら...
		if (m_StartMemory == NULL)
		{
			//仕方ないのでデータを確保.
			RealAlloc(1);

			m_Size = 1;
			return m_StartMemory;
		}
		//それが最後のデータなら...
		//if (m_RealMemory == m_EndMemory)  << 今のところこの if は不要.
		{
			//仕方ないのでデータを確保.
			RealAlloc(1);
			//現在使用サイズを増やす.
			m_Size ++;
			//次のメモリへ. 次ぎメモリも確保済みであることは保証されている.
			m_EndMemory = (__ALLOC_DATA*)m_EndMemory->m_Next;
			//現在終わりのメモリを返す.
			return m_EndMemory;
		}
		//だめだめ.
		return NULL;
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
		m_RealSize--;
		m_Size--;
		if (m_RealMemory == inData)
		{//これは、物理的に最後のデータ.
			__RTI_CHECK(m_RealSize == 0 && m_Size == 0);
			Init();
		}
		else	if (m_StartMemory == inData)
		{//最初のデータ.
			if (m_StartMemory == m_EndMemory)
			{//最初と最後がいっしょだから、両方 0 になる.
				__RTI_CHECK(m_Size == 0);
				m_StartMemory = NULL;
				m_EndMemory = NULL;
			}
			else
			{
				//次のデータを先頭にする. 次は保証されます.
				m_StartMemory = (__ALLOC_DATA*)inData->m_Next;
			}
		}
		else	if (m_EndMemory == inData)
		{//これは論理的な最後のデータ.
			//論理的に最後のデータの一つ前を算出. (一つ前があることは保証される.)
			m_EndMemory = GetPrev(m_EndMemory);
		}
		else
		{
			//ターゲットの一つ前も 一つ先も保証される.
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

		theDataNum = m_RealSize;
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
	//現在の物理的サイズの取得.
	virtual unsigned int GetRealCount(){		return m_RealSize;	}
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
		printf("m_RealMemory		%p\r\n",m_RealMemory);
		printf("m_EndMemory			%p\r\n",m_EndMemory);
		printf("m_RealSize			%u\r\n",m_RealSize);
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
	//データの物理的な終わり(実際に確保している量)
	__ALLOC_DATA*		m_RealMemory;
	//データの論理的な終わり(ユーザーに対してお知らせする確保している量)
	__ALLOC_DATA*		m_EndMemory;
	//実際に確保しているサイズ.
	unsigned int		m_RealSize;
	//データの件数. m_Size 分までです.
	unsigned int		m_Size;
};

#endif
