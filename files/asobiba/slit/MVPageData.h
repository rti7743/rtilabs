// MVPageData.h: MVPageData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVPAGEDATA_H__1ADF090A_5B63_4CB3_8CCA_12DB55869A82__INCLUDED_)
#define AFX_MVPAGEDATA_H__1ADF090A_5B63_4CB3_8CCA_12DB55869A82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RImage.h"

class MVPageData  
{
public:
	MVPageData();
	virtual ~MVPageData();


	HDC getDC() const
	{
		return this->Image.getDC();
	}
	int getWidth() const
	{
		return this->Image.getWidth();
	}
	int getHeight() const
	{
		return this->Image.getHeight();
	}

	void ChangeScale(int inWidth, int inHegiht)
	{
		this->Image.ChangeScale(inWidth , inHegiht);
	}

	void Load(const char * inBuffer , int inSize) throw(RException)
	{
		this->Image.Load(inBuffer , inSize);
	}

private:
	RImage		Image;
};

#endif // !defined(AFX_MVPAGEDATA_H__1ADF090A_5B63_4CB3_8CCA_12DB55869A82__INCLUDED_)
