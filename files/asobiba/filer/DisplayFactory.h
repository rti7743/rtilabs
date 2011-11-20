// DisplayFactory.h: DisplayFactory クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYFACTORY_H__B32C9EED_DFB3_4C10_98E9_2037BD2B35A3__INCLUDED_)
#define AFX_DISPLAYFACTORY_H__B32C9EED_DFB3_4C10_98E9_2037BD2B35A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayFactory;

#include "Window.h"
#include "Display.h"
#include "Rfile.h"

class DisplayFactory  
{
private:
	DisplayFactory();
public:
	virtual ~DisplayFactory();
public:
	//サポートしていないやつは RNotSupportException
	static Display* New(__DISPLAYTYPE inDisplayType, Window* inWindow , __RFILETYPE inRfileType) throw();
};

#endif // !defined(AFX_DISPLAYFACTORY_H__B32C9EED_DFB3_4C10_98E9_2037BD2B35A3__INCLUDED_)
