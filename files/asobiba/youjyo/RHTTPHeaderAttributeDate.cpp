// RHTTPHeaderAttributeDate.cpp: RHTTPHeaderAttributeDate クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RHTTPHeaderAttributeDate.h"
#include "RDateConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RHTTPHeaderAttributeDate::RHTTPHeaderAttributeDate()
{
	this->Date = -1;
	this->Enabled = false;
}

RHTTPHeaderAttributeDate::~RHTTPHeaderAttributeDate()
{

}

RHTTPHeaderAttributeDate& RHTTPHeaderAttributeDate::operator=(const char* inHeader)
{
	this->Date = RDateConv::HttpTimeToUnixTime( inHeader );
	if (this->Date != -1)
	{
		this->Enabled = true;
	}
	return (*this);
}

