// RNeoIFStreamUtil.h: RNeoIFStreamUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNEOIFSTREAMUTIL_H__99BC3921_D3F0_4FA2_BEDD_93AA770FD785__INCLUDED_)
#define AFX_RNEOIFSTREAMUTIL_H__99BC3921_D3F0_4FA2_BEDD_93AA770FD785__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNeoIStreamble.h"

class RNeoIFStreamUtilChecker
{
public:
	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead) = 0;
	virtual int getSeparater() const = 0;
	virtual int getNext() const = 0;
};


//連続する改行コードまで
class RNeoIFStreamUtilForSecutiveReturnCode : public RNeoIFStreamUtilChecker
{
public:
	RNeoIFStreamUtilForSecutiveReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~RNeoIFStreamUtilForSecutiveReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		if (inAllReadyRead -4  < 0) inAllReadyRead = 0;

		for (int i = inAllReadyRead; i < (inNowRead-3) ; i ++) 
		{
			if ( (inBuffer[i] == '\r' && inBuffer[i+1] == '\n' && inBuffer[i+2] == '\r' && inBuffer[i+3] == '\n') )
			{
				ConectStart = i + 4;
				Separater = i;
				return false;
			}
			else if (	(inBuffer[i] == '\r' && inBuffer[i+1] == '\r') ||
						(inBuffer[i] == '\r' && inBuffer[i+1] == '\r')  )
			{
				ConectStart = i + 2;
				Separater = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};
//改行コードまで
class RNeoIFStreamUtilForReturnCode : public RNeoIFStreamUtilChecker
{
public:
	RNeoIFStreamUtilForReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~RNeoIFStreamUtilForReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		for (int i = inAllReadyRead; i < inNowRead ; i ++) 
		{
			if ( inBuffer[i] == '\n' )
			{
				Separater = i - 1;
				ConectStart = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};


class RNeoIFStreamUtil
{
public:
	RNeoIFStreamUtil()
	{
		Buffer = NULL;
		Stream = NULL;
	}
	~RNeoIFStreamUtil()
	{
		delete [] Buffer;
	}

	void Create(RNeoIStreamble* ioStream , int inTotalBufer = 65535)
	{
		ASSERT(Buffer == NULL);

		Buffer =	new char[inTotalBufer];
		BufferLength = inTotalBufer;
		BufferUse = 0;

		Stream = ioStream;
	}

	int Read(char *outBuffer ,int inBufferSize,RNeoIFStreamUtilChecker* inStreamChecker) throw(RException);


	


	static void test();
private:
	RNeoIStreamble*	Stream;
	char	*		Buffer;
	int				BufferLength;
	int				BufferUse;
};

#endif // !defined(AFX_RNEOIFSTREAMUTIL_H__99BC3921_D3F0_4FA2_BEDD_93AA770FD785__INCLUDED_)
