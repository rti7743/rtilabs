// RHTTPHeaderAttribute.h: RHTTPHeaderAttribute クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPHEADERATTRIBUTE_H__16223EA9_BAA3_4EAA_B366_533F71B1EDAB__INCLUDED_)
#define AFX_RHTTPHEADERATTRIBUTE_H__16223EA9_BAA3_4EAA_B366_533F71B1EDAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "TypeManip.h"

template <typename _V>
class RHTTPHeaderAttribute  
{
public:
	RHTTPHeaderAttribute()
	{
		this->Enable = false;
	}
	virtual ~RHTTPHeaderAttribute()
	{
	}

	//解析.
	bool Decode(const char* inStr)
	{
		for(const char*p = inStr ; *p != '\0' && !isgraph(*p)  ; p++)	
			;

		this->Enable = true;

		setValue( p , Loki::Int2Type< Loki::Conversion<_V , int>::exists >() );

		return true;
	}

	const _V getValue() const
	{
		return this->Val;
	}

	bool IsEnable()
	{
		return this->Enable;
	}
private:
	void setValue(const char * p , Loki::Int2Type< true >)
	{
		this->Val = atoi(p);
	}
	void setValue(const char * p , Loki::Int2Type< false >)
	{
		this->Val = p;
	}
public:
	static void test()
	{
		{
			RHTTPHeaderAttribute<string> rha;
			ASSERT( rha.Decode(" sakuratan haahaa") );
			ASSERT( rha.getValue() == "sakuratan haahaa" );
		}
		{
			RHTTPHeaderAttribute<int> rha;
			ASSERT( rha.Decode("123456") );
			ASSERT( rha.getValue() == 123456 );
		}
	}

private:
	bool	Enable;
	_V		Val;
};

#endif // !defined(AFX_RHTTPHEADERATTRIBUTE_H__16223EA9_BAA3_4EAA_B366_533F71B1EDAB__INCLUDED_)
