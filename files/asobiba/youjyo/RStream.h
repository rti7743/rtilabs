// RStream.h: RStream クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAM_H__A0B8A670_3BE7_4E33_9669_4CBF1617501E__INCLUDED_)
#define AFX_RSTREAM_H__A0B8A670_3BE7_4E33_9669_4CBF1617501E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RStream  
{
public:
	RStream();
	virtual ~RStream();

	virtual int Write(const char *inBuffer ,int inBufferSize) = 0;
	virtual int Read(char *outBuffer ,int inBufferSize) = 0;
};

#endif // !defined(AFX_RSTREAM_H__A0B8A670_3BE7_4E33_9669_4CBF1617501E__INCLUDED_)
