// RGISSmallUserData.h: RGISSmallUserData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLUSERDATA_H__4CF2CE3F_B023_46C6_86F1_3927F449039B__INCLUDED_)
#define AFX_RGISSMALLUSERDATA_H__4CF2CE3F_B023_46C6_86F1_3927F449039B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"
#include "RGISSmallNameDatabase.h"
#include "RGISSmallFindObject.h"
#include "RStdioMemoryWrite.h"

/*
	type	0	-	63			文字列
			64	-	127			int
			128	-	191			short
			192	-	255			char
*/

enum USERDATA_DEFINE
{
//			0	-	63			文字列
	USERDATA_NAME		=	0,
	USERDATA_COMMENT	=	1,
//			64	-	127			int
//			128	-	191			short
//			192	-	254			char
	USERDATA_ICON	=	192,
	USERDATA_CATEGO	=	193,
	USERDATA_SUB_CATEGO	=	194,
	USERDATA_PENSIZE	=	200,

	USERDATA_EOD	=	255,
};


class RGISSmallUserDataWrite
{
public:
	int Create(char * outData, const SPOINT * inPoint ) 
	{
		if (outData == NULL)
		{
			return getHeaderSize();
		}
		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUS(inPoint->x);
		mem.writeGetUS(inPoint->y);

		return mem.ftell();
	}

	//文字列
	int Attribute(char * outData, unsigned char inType , const string & inStr) 
	{
		if (outData == NULL)
		{
			return getAttributeHeaderSize() + inStr.size() + 1;
		}

		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUC(inType);
		mem.writeGetUC( (unsigned char)inStr.size() + 1);
		mem.fwrite(inStr.c_str() , inStr.size() + 1);	//+1 は NULL

		return mem.ftell();
	}
	//何か
	template<class _T> int Attribute(char * outData, unsigned char inType , _T inT)
	{
		if (outData == NULL)
		{
			return getAttributeHeaderSize() + sizeof(inT);
		}

		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUC(inType);
		mem.writeGetUC( sizeof(_T) );
		mem.fwrite( &inT , sizeof(_T) );

		return mem.ftell();
	}
	//終端
	int Null(char * outData)
	{
		if (outData == NULL)
		{
			return getAttributeHeaderSize() ;
		}

		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUC(0xff);
		mem.writeGetUC(0);

		return mem.ftell();
	}
private:
	int getHeaderSize() const
	{
		return
			sizeof(unsigned short)	+		//x
			sizeof(unsigned short)	;		//y
	}
	int getAttributeHeaderSize() const
	{
		return
			sizeof(unsigned char)	+		//type
			sizeof(unsigned char)	;		//size
	}
};


class RGISSmallUserDataRead
{
private:
	LPOINT					XY;
	int						Serial;

	//ビバ! アトリビュートユニオン!
	union AttributeUnion
	{
		const char *		str;
		unsigned long		lval;
		unsigned short		sval;
		unsigned char		cval;
	};
	AttributeUnion			Attribute[256];
public:
	//作成.
	const char* Create(const LPOINT * inBlockStart ,const char * inData, int inDataSize , int inSerial);

	//属性検索.
	const char* FindAttributeStr(int inType) const
	{
		ASSERT(inType >= 0 && inType <= 63);		//0	-	63			文字列

		return Attribute[inType].str;
	}
	//属性検索.
	unsigned long FindAttributeL(int inType) const
	{
		ASSERT(inType >= 64 && inType <= 127);		//64	-	127			int

		return Attribute[inType].lval;
	}
	//属性検索.
	unsigned short FindAttributeS(int inType) const
	{
		ASSERT(inType >= 128 && inType <= 191);		//128	-	191			short

		return Attribute[inType].sval;
	}
	//属性検索.
	unsigned char FindAttributeC(int inType) const
	{
		ASSERT(inType >= 192 && inType <= 255);		//192	-	255			char

		return Attribute[inType].cval;
	}


	const LPOINT* getXY() const
	{
		return &XY;
	}
	int getSerial() const
	{
		return Serial;
	}
};

class RGISSmallUserData  
{
public:
	RGISSmallUserData();
	virtual ~RGISSmallUserData();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);

	//ユーザーデータの検索.
	int FindUserData(const LPOINT *inBlockStart ,RGISSmallUserDataRead * outFindData ,int inDistance,  const LRECT * inRect  ) const;


private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLUSERDATA_H__4CF2CE3F_B023_46C6_86F1_3927F449039B__INCLUDED_)
