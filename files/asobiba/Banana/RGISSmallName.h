// RGISSmallName.h: RGISSmallName クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLNAME_H__BADD9638_C657_4CF6_A903_A2C4E3F526D0__INCLUDED_)
#define AFX_RGISSMALLNAME_H__BADD9638_C657_4CF6_A903_A2C4E3F526D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RConv.h"

struct RGISSmallNamePair
{
	string			Name;
	unsigned char	Type;
};

class RGISSmallName  
{
private:
	class RGISSmallNameRuleChain
	{
	private:
		unsigned char		Type;
		RStringList			MatchList;
	public:
		void Create(unsigned char inType , const string & inMatchList)
		{
			this->Type = inType;
			this->MatchList = RConv::Explode(inMatchList);
		}

		unsigned char getType() const
		{
			return this->Type;
		}
		const RStringList* getMatchList() const
		{
			return &this->MatchList;
		}
	};

public:
	RGISSmallName();
	virtual ~RGISSmallName();

	void Creae(const string & inFilename) throw(RException);

	bool Match(const char* inName , RGISSmallNamePair * outPair) const;
private:
	newVector<RGISSmallNameRuleChain>	Chain;
};

#endif // !defined(AFX_RGISSMALLNAME_H__BADD9638_C657_4CF6_A903_A2C4E3F526D0__INCLUDED_)
