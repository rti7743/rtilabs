// RTableEasy.cpp: RTableEasy クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTableEasy.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
#include "RfilesLocal.h"

//テスト.
void RTableEasy::test()
{
	RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");
	RTableEasy a(__FILECOLUMN_TYPE_EXT,true);
	RfileLocalReport* rlr;

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ggg.dat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag4.dat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag3.pat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag2.aat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.eat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.gat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.mat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.nat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.pat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.fat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.bat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	a.Sort();


	a.createAllList(  );

	DisplayDebugPrintf d;
	d.setRfileItems( a.getAllSize() , a.getAllList( ) );

	string DebugString = "ag.bat ★ ファイル ag.eat ★ ファイル ag.fat ★ ファイル ag.gat ★ ファイル ag.mat ★ ファイル ag.nat ★ ファイル ag.pat ★ ファイル ag2.aat ★ ファイル ag3.pat ★ ファイル ag4.dat ★ ファイル ggg.dat ★ ファイル ";
	if ( d.DebugBuffer != DebugString )
	{
		TRACE("\n%s\n",d.DebugBuffer.c_str());
		ASSERT(0);
	}
}
