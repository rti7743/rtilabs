// AddressHistory.h: AddressHistory クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_)
#define AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AddressHistory  ;

#include "comm.h"
#include "RNotFoundException.h"

class AddressHistory  
{
private:
	typedef deque<string>	HISTORY;
	HISTORY					History;
	int						HistoryPointer;
public:
	AddressHistory();
	virtual ~AddressHistory();

	//進めるの?
	bool IsNext() const;
	//戻れる?
	bool IsBack() const;
	//戻れ!
	//! 戻れないときには RNotFoundException  を投げる.
	string Back() throw(RNotFoundException);
	//進め!
	//! 進めないときには RNotFoundException  を投げる.
	string Next() throw(RNotFoundException);
	//一つ前のパスを取得します。
	//! 取得できないときは RNotFoundException  を投げる.
	string getLastPath() const throw(RNotFoundException);

	//パスの追加
	void Append(string inAddress);

	static void test();
};

#endif // !defined(AFX_ADDRESSHISTORY_H__ECBAB96B_64E1_4D32_A5CB_0B5E375BDA2B__INCLUDED_)
