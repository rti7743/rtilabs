// AddressHistory.cpp: AddressHistory クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "AddressHistory.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AddressHistory::AddressHistory()
{
	HistoryPointer = -1;
}

AddressHistory::~AddressHistory()
{
}

//進めるの?
bool AddressHistory::IsNext() const
{
	if (History.size() <= 0) return false;
	return ( (HistoryPointer+1) < History.size());
}

//戻れる?
bool AddressHistory::IsBack() const
{
	if (History.size() <= 0) return false;
	return ((HistoryPointer) > 0 );
}

//戻れ!
//! 戻れないときには RNotFoundException  を投げる.
string AddressHistory::Back() throw(RNotFoundException)
{
	if ( !IsBack() )
	{
		throw RNotFoundException("これ以上戻ることはできません");
	}
	return History[--HistoryPointer];
}

//進め!
//! 進めないときには RNotFoundException  を投げる.
string AddressHistory::Next() throw(RNotFoundException)
{
	if ( !IsNext() ) 
	{
		throw RNotFoundException("これ以上進むことはできません");
	}
	return History[++HistoryPointer];
}

//一つ前のパスを取得します。
//! 取得できないときは RNotFoundException  を投げる.
string AddressHistory::getLastPath() const throw(RNotFoundException)
{
	if ( !IsBack() )
	{
		throw RNotFoundException("これ以上後ろはありません");
	}
	return History[HistoryPointer - 1];
}


//パスの追加
void AddressHistory::Append(string inAddress)
{
	//前回追加した要素は追加しない
	if (HistoryPointer >= 0 && History[HistoryPointer] == inAddress) return;


	//追加
	if ( HistoryPointer+1 < History.size() )
	{
		History.resize(HistoryPointer + 1);
	}
	History.push_back( inAddress );
	HistoryPointer++;
/*
	puts("==============dump================");
	for (HISTORY::iterator it = History.begin() ; it != History.end() ; ++it)
	{
		puts((*it).c_str());
	}
	puts("==============================");
*/
}


void AddressHistory::test()
{
	AddressHistory p;

	ASSERT( ! p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == -1 );

	//--------------------------------
	p.Append("c:\\");
	p.Append("c:\\");	//同じなので追加されない

	ASSERT( p.History.size() == 1);

	ASSERT( ! p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 0 );

	//--------------------------------
	p.Append("d:\\");

	ASSERT( p.History.size() == 2);

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//現在 c:\\ d:\\ と詰まれているはず.
	p.Append("e:\\");

	ASSERT( p.History.size() == 3);

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//               ^-------HistoryPointer はここを指してます。

	//今 e:\\ の一つ前がほしいので d:\\ を返します。
	//通常スタックと少し動作が違うので注意.
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//          ^-------HistoryPointer はここを指してます。

	//進んでみます。
	ASSERT( p.Next() == "e:\\" );

	ASSERT( p.IsBack() );
	ASSERT(! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//                ^-------HistoryPointer はここを指してます。

	//やっぱり戻ります
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//さらに戻ります
	ASSERT( p.Back() == "c:\\" );

	ASSERT( !p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 0 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//     ^-------HistoryPointer はここを指してます。

	//一つ進みます。
	ASSERT( p.Next() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//          ^-------HistoryPointer はここを指してます。

	//F5リロードしました。
	p.Append("d:\\");	//追加されない

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//現在 c:\\ d:\\ e:\\ と詰まれているはず.
	//          ^-------HistoryPointer はここを指してます。

	//別のところに逝きます。
	p.Append("x:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//現在 c:\\ d:\\ x:\\ と詰まれているはず.
	//               ^-------HistoryPointer はここを指してます。

	//別のところに逝きます。
	p.Append("z:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 3 );

	//--------------------------------
	//現在 c:\\ d:\\ x:\\ z:\\ と詰まれているはず.
	//                    ^-------HistoryPointer はここを指してます。

	//戻ります
	ASSERT( p.Back() == "x:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//もう一つ戻ります
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//現在 c:\\ d:\\ x:\\ z:\\ と詰まれているはず.
	//          ^-------HistoryPointer はここを指してます。

	p.Append("p:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//現在 c:\\ d:\\ p:\\ と詰まれているはず.
	//               ^-------HistoryPointer はここを指してます。
}

