// GISType.h: GISType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GISTYPE_H__BC381BA5_8D61_40CE_8D0A_6427043F320A__INCLUDED_)
#define AFX_GISTYPE_H__BC381BA5_8D61_40CE_8D0A_6427043F320A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

//1234,5678,12345 などといったカンマ区切りの数字の文字列を、
//整数の配列に変えます。
class GISINTArray
{
	unsigned int	Count;
	unsigned long*	Points;
public:
	GISINTArray();
	~GISINTArray();
	GISINTArray(const GISINTArray& inMySelf);
	GISINTArray& operator=(const GISINTArray& inMySelf);

	void Create(const char* inStr) throw (RException);
	void Create(unsigned long* inPoints ,unsigned int inCount);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	int getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	void Delete();



	static void test();
};

class GISType
{
	char Type[2];
public:
	GISType()
	{
	}

	GISType(const char* inStr)
	{
		ASSERT(inStr != NULL );
		ASSERT(inStr[0] != '\0');
		ASSERT(inStr[1] != '\0');

		this->Type[0] = inStr[0];
		this->Type[1] = inStr[1];

		ASSERT( (Type[0] >= '0' && Type[0] <= '9') || (Type[0] >= 'A' && Type[0] <= 'F') );
		ASSERT( (Type[1] >= '0' && Type[1] <= '9') || (Type[1] >= 'A' && Type[1] <= 'F') );
	}
	GISType(GISType& inT)
	{
		this->Type[0] = inT.Type[0];
		this->Type[1] = inT.Type[1];

		ASSERT( (Type[0] >= '0' && Type[0] <= '9') || (Type[0] >= 'A' && Type[0] <= 'F') );
		ASSERT( (Type[1] >= '0' && Type[1] <= '9') || (Type[1] >= 'A' && Type[1] <= 'F') );
	}
	bool operator==(const char* inStr) const
	{
		ASSERT(inStr != NULL );
		ASSERT(inStr[0] != '\0');
		ASSERT(inStr[1] != '\0');

		return((this->Type[0] == inStr[0]) && (this->Type[1] == inStr[1]));
	}
	bool operator!=(const char* inStr) const
	{
		ASSERT(inStr != NULL );
		ASSERT(inStr[0] != '\0');
		ASSERT(inStr[1] != '\0');

		return!((this->Type[0] == inStr[0]) && (this->Type[1] == inStr[1]));
	}
	bool operator==(GISType* inStr) const
	{
		return((this->Type[0] == inStr->Type[0]) && (this->Type[1] == inStr->Type[1]));
	}
	bool operator!=(GISType* inStr) const
	{
		return!((this->Type[0] == inStr->Type[0]) && (this->Type[1] == inStr->Type[1]));
	}
	operator string() const
	{
		ASSERT( (Type[0] >= '0' && Type[0] <= '9') || (Type[0] >= 'A' && Type[0] <= 'F') );
		ASSERT( (Type[1] >= '0' && Type[1] <= '9') || (Type[1] >= 'A' && Type[1] <= 'F') );

		string ret;
		ret.resize(2);
		ret[0] = Type[0];
		ret[1] = Type[1];
//		ret[2] = '\0';
		return ret;
	}
	unsigned char OnePack() const
	{
		ASSERT( (Type[0] >= '0' && Type[0] <= '9') || (Type[0] >= 'A' && Type[0] <= 'F') );
		ASSERT( (Type[1] >= '0' && Type[1] <= '9') || (Type[1] >= 'A' && Type[1] <= 'F') );

		unsigned char  ret;
		ret= ( (unsigned)( (Type[0] >= 'A') ? Type[0] - 'A' + 10 : Type[0] - '0' ) << 4) + 
				( (Type[1] >= 'A') ? Type[1] - 'A' + 10 : Type[1] - '0' );
		return ret;
	}

	//大変危険なので使い方に注意!!
	//scanfの中で使う場合は、絶対に %2s などを使うこと!!
	char* ptr()
	{
		return Type;
	}

	static void test();
};

#endif // !defined(AFX_GISTYPE_H__BC381BA5_8D61_40CE_8D0A_6427043F320A__INCLUDED_)
