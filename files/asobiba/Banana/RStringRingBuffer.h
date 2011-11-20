// RStringRingBuffer.h: RStringRingBuffer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTRINGRINGBUFFER_H__4E2463A9_6A09_4FA1_80C3_C36527B6E990__INCLUDED_)
#define AFX_RSTRINGRINGBUFFER_H__4E2463A9_6A09_4FA1_80C3_C36527B6E990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

class RStringRingBuffer
{
public:
	RStringRingBuffer()
	{
		this->Buffer = NULL;
	}
	~RStringRingBuffer()
	{
		delete [] this->Buffer;
	}
	void Create(int inBufferSize)
	{
		this->OneSize = inBufferSize;
		this->BufferSize = inBufferSize * 2;
		this->NowPos = 0;

		this->Buffer = new char[this->BufferSize + 1];
	}

	void RStringRingBuffer::Push(const char* inStr , int inSize) throw(RException);
	const string RStringRingBuffer::getBuffer() const;
	static void RStringRingBuffer::test();

private:
	int		BufferSize;
	int		OneSize;
	char*	Buffer;
	int		NowPos;
};

#endif // !defined(AFX_RSTRINGRINGBUFFER_H__4E2463A9_6A09_4FA1_80C3_C36527B6E990__INCLUDED_)
