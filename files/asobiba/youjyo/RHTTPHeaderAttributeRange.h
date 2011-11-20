// RHTTPHeaderAttributeRange.h: RHTTPHeaderAttributeRange クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPHEADERATTRIBUTERANGE_H__AAC0D132_3944_4B28_84AF_682263FC81D9__INCLUDED_)
#define AFX_RHTTPHEADERATTRIBUTERANGE_H__AAC0D132_3944_4B28_84AF_682263FC81D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RHTTPHeaderAttributeRange  
{
public:
	RHTTPHeaderAttributeRange();
	virtual ~RHTTPHeaderAttributeRange();

	RHTTPHeaderAttributeRange& operator=(const char* inHeader)
	{
		const char* p = strchr( inHeader , '=');
		if (p != NULL)
		{
			const char* s = p + 1;
			const char * haifun = strchr(s , '-');
			if ( haifun != NULL)
			{//201-300
				if ( *(haifun + 1) == '\0')
				{//201-
					Start = atoi(s);
					End = INT_MAX;
				}
				else if (s == haifun )
				{//-300
					Start = 0;
					End = atoi(haifun + 1);
				}
				else
				{
					Start = atoi(s);
					End = atoi(haifun + 1);
				}
				Enable = true;
			}
		}
		//エラーチェック
		if (Start >= End)
		{
			Start = 0;
			End = INT_MAX;
			Enable = false;
		}
		
		return (*this);
	}

	int getStart() const	{		return Start;	}
	int getEnd() const	{		return End;	}
	bool IsEnabled() const {	return Enable;	}

private:
	int Start;
	int End;
	bool Enable;
};

#endif // !defined(AFX_RHTTPHEADERATTRIBUTERANGE_H__AAC0D132_3944_4B28_84AF_682263FC81D9__INCLUDED_)
