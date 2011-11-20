// RIPFilter.h: RIPFilter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_)
#define AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"

class RIPFilterData
{
public:
	RIPFilterData(in_addr inFromIP,in_addr inToIP)
	{
		FromIP.s_addr = htonl(inFromIP.s_addr);	//ビッグエンディアン表記に変換
		ToIP.s_addr = htonl(inToIP.s_addr);		//ビッグエンディアン表記に変換
	}
	RIPFilterData(in_addr inIP,int inMask)
	{
		unsigned long mask = (2 ^ inMask);
		FromIP.s_addr = (htonl(inIP.s_addr) & (~mask));	//ビッグエンディアン表記に変換
		ToIP.s_addr = (htonl(inIP.s_addr) | mask);		//ビッグエンディアン表記に変換
	}
	
	/**
	 * IsBan:		はじいていいの?
	 *
	 * @param inIP	評価しなくてはならないIP
	 * @return bool	ダメな場合 true
	 */
	bool IsBan(in_addr inIP) const
	{
		return (FromIP.s_addr <= inIP.s_addr && inIP.s_addr <= ToIP.s_addr);
	}

	static void RIPFilterData::test();
private:
	in_addr	FromIP;	//ビッグエンディアン表記
	in_addr	ToIP;	//ビッグエンディアン表記
};

class RIPFilter  
{
public:
	RIPFilter();
	virtual ~RIPFilter();

	void Clear();
	void Load(const string inFilename);

	inline bool IsBan(in_addr inIP) const
	{
		in_addr p;
		p.s_addr = htonl(inIP.s_addr);	//ビッグエンディアンに変換.
		for(IPLIST::const_iterator i = IPList.begin() ; i != IPList.end() ; ++i)
		{
			if ( (*i)->IsBan(p) ) return true;	//アクセス禁止
		}
		return false;	//アクセス許可.
	}

	static void RIPFilter::test();

private:
	void AppendIP(const string) throw(RParseException);

	//フィルターするIP一覧.
	typedef vector<RIPFilterData*> IPLIST;
	IPLIST	IPList;
};

#endif // !defined(AFX_RIPFILTER_H__2AB426CA_BA76_401E_A007_E39C4D0BA2CD__INCLUDED_)
