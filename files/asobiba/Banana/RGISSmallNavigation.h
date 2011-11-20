// RGISSmallNavigation.h: RGISSmallNavigation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_)
#define AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#include "comm.h"
//ナビゲーションで使用する情報を収集するための構造体.
//クラスにしないのは、 なんか CE ででかいクラスを大量に作成するとクラッシュしてしまうため.(なぜ?
//VC++では動作するんだけどなぁ。 EVC なんかおかしいのかなぁ.
struct SmallPathfinderData
{
	struct SmallPathfinderDataOne
	{
		unsigned char			SB;
		unsigned char			FI;

		unsigned char			NodeCount;
		struct _Node
		{
			unsigned char	Pos;				//ノードは道路データの何番目にあるか?
			unsigned char	ConnectCount;		//接続する道路の数 (4字の場合は 3 あることになる.)
			struct _Connect
			{
				unsigned char	AtherMap;		//別のマップに接続?	0->なし  以下テンキー方向に接続.
				DWORD			Number;			//接続される道路番号
				unsigned char	Pos;			//接続される道路のノードの場所
			};
			_Connect *Connect;
		};
		_Node	*Node;

		unsigned char			CVCount;
		SPOINT				*	CV;
	};
	DWORD						DataCount;
	SmallPathfinderDataOne	*	Data;
};

struct SmallPathfinderOneData
{
	DWORD	MapNumber;
	DWORD	DouroNumber;
	unsigned char	DouroPos;
};

struct SmallPathfinderSave
{
	DWORD	Distance;
	newVector<SmallPathfinderOneData>	Data;
};

class RGISSmallNavigation  
{
public:
	RGISSmallNavigation();
	virtual ~RGISSmallNavigation();

	void Create(const RGISSmallIntelligentCache* inCache,int inDetourMax);
	void Find();

private:
	newVector<SmallPathfinderSave>	Save;
	int	DetourMax;
};
*/
#endif // !defined(AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_)
