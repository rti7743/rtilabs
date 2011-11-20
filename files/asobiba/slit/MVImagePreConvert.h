// MVImagePreConvert.h: MVImagePreConvert クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVIMAGEPRECONVERT_H__1B4EFD74_E26F_4149_8776_6B7772A54BB4__INCLUDED_)
#define AFX_MVIMAGEPRECONVERT_H__1B4EFD74_E26F_4149_8776_6B7772A54BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MVConfig.h"

class MVImagePreConvert  
{
public:

	MVImagePreConvert();
	virtual ~MVImagePreConvert();

	void Create(int inScreenWidth ,int inScreenHeight ,bool inJustFit ,MVConfig::SCROLL_TYPE inScrollType)
	{
		this->ScreenWidth		=	inScreenWidth;
		this->ScreenHeight		=	inScreenHeight;
		this->JustFit			=	inJustFit;
		this->ScrollType		=	inScrollType;

	}

	int getScreenWidth() const
	{
		return this->ScreenWidth;
	}
	int getScreenHeight() const
	{
		return this->ScreenHeight;
	}
	bool getJustFit() const
	{
		return this->JustFit;
	}
	MVConfig::SCROLL_TYPE getScrollType() const
	{
		return this->ScrollType;
	}

private:


private:
	int			ScreenWidth;
	int			ScreenHeight;

	bool		JustFit;
	MVConfig::SCROLL_TYPE	ScrollType;

};

#endif // !defined(AFX_MVIMAGEPRECONVERT_H__1B4EFD74_E26F_4149_8776_6B7772A54BB4__INCLUDED_)
