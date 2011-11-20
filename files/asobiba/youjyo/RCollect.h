// RCollect.h: RCollect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_)
#define AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

template <class _K,class _T>
class RCollect  
{
	typedef map<_K,_T*>	RCOLLECTDEF;
public:
	RCollect()
	{
	}
	virtual ~RCollect()
	{
		//すべて解放.
		for(RCOLLECTDEF::iterator i = Def.begin() ; i != Def.end() ; ++i)
		{
			delete (*i).second;
		}
	}


	class CharProxy
	{
	public:
		//作成
		CharProxy(RCollect<_K,_T>& str,_K index) : m_String(str) , m_CharIndex(index){};
	private:
		//リスクが大きいので非サポート
		//lvalue my[10] = sting[20];
		CharProxy& operator=(const CharProxy& rhs)
		{
			m_String.Def[this->m_CharIndex] = rhs.Def[rhs.m_CharIndex];
			return *this;
		}
	public:
		//lvalue my[10] = 'c';
		CharProxy& operator=(_T* c)
		{
			RCOLLECTDEF::iterator i = m_String.Def.find(this->m_CharIndex);
			if (i == m_String.Def.end() )
			{
				m_String.Def.insert( pair<_K,_T*>(this->m_CharIndex,c) );
			}
			else
			{
				delete (*i).second;
				(*i).second = c;
			}
			return *this;
		}
		//rvalue char a = my[10];
		operator _T*() const
		{
			return *(this->m_String.Def[this->m_CharIndex] );
		}
		RCollect<_K,_T>& m_String;	//プロキシが属する文字列
		_K m_CharIndex;	//↑の中でこのプロキシが扱うもの

	};
	//Stringクラスの延長
	const CharProxy operator[](_K index) const	//const String のため
	{
		return CharProxy( (RCollect<_K,_T>&)*this,index);
	}
	//非 const String のため
	CharProxy operator[](_K index)
	{
		return CharProxy(*this,index);
	}
	friend class CharProxy;


	_T* find(_K index) const
	{
		RCOLLECTDEF::const_iterator i = Def.find(index);
		if (i == Def.end() ) return NULL;
		return (*i).second;
	}
private:
	RCOLLECTDEF	Def;	

};

#endif // !defined(AFX_RCOLLECT_H__DFE2F6CF_50F9_43A4_A80B_78F41AB15802__INCLUDED_)
