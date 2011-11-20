// VirtualPath.h: VirtualPath クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_)
#define AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


//仮想パスをネストできる数.
const int MAX_NEXT_VIRTUAL_PATH	(128);

class VirtualPath  
{
private:
	VirtualPath()	{	}					//呼べませんよ ニヤリ
public:
	VirtualPath(string inPath);
	virtual ~VirtualPath();

	//フルパスの取得.
	string getFullPath()			const	{		return Path;	}
	//パスの取得.
	string getSubPath(int inIndex)	const
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <= DilimiteresMax );

		if (inIndex == 0)
		{	//先頭
			return Path.substr( 0 , PathDilimiteres[0] );
		}
		else
		{	//それ以外の何か
			const int startPos = PathDilimiteres[inIndex-1]+2;
			return Path.substr( startPos , PathDilimiteres[inIndex] - startPos );
		}
	}
	//パスの深さの取得.
	int getPathDilimiterCount()	const	{	return DilimiteresMax; }

//サービス(新太郎のおじさん)
	//仮想ではないパスの取得.
	string getNativePath()	const	{		return getSubPath(0);	}
	//最後のパスの取得.
	string getLastPath()	const	{		return getSubPath(DilimiteresMax);	}
	//最後から一つ前のパス
	string getAdjacentPath()	const	
	{
		if (DilimiteresMax == 0)
		{
			return getNativePath();
		}
		else
		{
			return getSubPath(DilimiteresMax-1);
		}
	}

//テスト
	static void test();

private:
	//仮想パスのフルパス.
	string	Path;	
	//パスの区切り +1は番兵.
	int		PathDilimiteres[MAX_NEXT_VIRTUAL_PATH+1];
	//↑現在の最大値
	int		DilimiteresMax;	//
};

#endif // !defined(AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_)
