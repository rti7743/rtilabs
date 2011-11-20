// RGISSmallFindObject.h: RGISSmallFindObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFINDOBJECT_H__0BF46CAD_B211_4808_B14C_46BDF08098F9__INCLUDED_)
#define AFX_RGISSMALLFINDOBJECT_H__0BF46CAD_B211_4808_B14C_46BDF08098F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallType.h"

class RGISSmallFindObject
{
private:
	LPOINT XY;		//場所 絶対座標
	string Name;	//名前
public:
	RGISSmallFindObject(const LPOINT * inXY ,const string & inName )
	{
		XY = *inXY;
		Name = inName;
	}
	const char * getName() const
	{
		return Name.c_str();
	}
	LPOINT getPos() const
	{
		return XY;
	}
};
typedef vector<RGISSmallFindObject*> FindPool;

#endif // !defined(AFX_RGISSMALLFINDOBJECT_H__0BF46CAD_B211_4808_B14C_46BDF08098F9__INCLUDED_)
