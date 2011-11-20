// RAlphabetHash.h: RAlphabetHash クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_)
#define AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//アルファベットを高速に判定します。
//ただし、小文字 only で 5文字までです。
//それ以外の結果は未定義です。
class RAlphabetHash  
{
public:
	RAlphabetHash()
	{
		Hash = 0;
	}
	RAlphabetHash(const char* p)
	{
		this->Hash = getHash(p);
	}
	~RAlphabetHash()	//継承禁止
	{
	}

	/**
	 * getHash:	ハッシュを求める.
	 *
	 * @param p					ハッシュ化する文字列(小文字 only で 5文字)
	 * @return unsigned long	ハッシュ値
	 */
	unsigned long getHash(const char* p) const
	{
#ifdef _DEBUG
		{
			//小文字 only で 5文字 であるかどうか
			for(int i = 0 ; i < 6 ; i ++)
			{
				if (p[i] == '\0') break;
				ASSERT( islower(p[i]) );
			}
			ASSERT(i <= 5);
		}
#endif
		if (p[0] == '\0') return 0;
		else if (p[1] == '\0') return (p[0] - 97);
		else if (p[2] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6);
		else if (p[3] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12);
		else if (p[4] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18);
		else if (p[5] == '\0') return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18) + ((p[4] - 97) << 24);
		else                   return (p[0] - 97) + ((p[1] - 97) << 6) + ((p[2] - 97) << 12) + ((p[3] - 97) << 18) + ((p[4] - 97) << 24) + ((p[5] - 97) << 30);
	}

	RAlphabetHash& operator=(const char* p)
	{
		this->Hash = getHash(p);
		return (*this);
	}
	bool operator==(const char* p)
	{
		return this->Hash == getHash(p);
	}
	bool operator!=(const char* p)
	{
		return this->Hash != getHash(p);
	}
	bool operator==(unsigned long p)
	{
		return this->Hash == p;
	}
	bool operator!=(unsigned long p)
	{
		return this->Hash != p;
	}


	operator unsigned long() const 
	{
		return this->Hash;
	}
	operator unsigned int() const 
	{
		return this->Hash;
	}

	static void test();

private:
	unsigned long		Hash;
};

#endif // !defined(AFX_RALPHABETHASH_H__83469041_6D53_425A_A10A_2B12260571C4__INCLUDED_)
