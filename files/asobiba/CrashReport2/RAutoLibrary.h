// RAutoLibrary.h: RAutoLibrary クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
#define AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

//DLLの読み込みとか
class RAutoLibrary
{
public:
	//構築
	RAutoLibrary();

	//指定した名前で構築
	//失敗は RDoNotInitaliseException です。
	RAutoLibrary(const char* inDLLName) ;

	//破棄
	virtual ~RAutoLibrary() ;

	//DLL読み込み
	//@inDLLName	DLLの名前
	void Load(const char* inDLLName) ;

	//DLL破棄
	void Destory();

	//DLLから 関数を取得
	//@inFunctionName		関数名
	//初期化かされていない場合は RDoNotInitaliseException です。
	//そんな関数ない場合は RNotSupportException です。
	FARPROC GetProcAddress(const char* inFunctionName);

	//DLLは読み込まれているの?
	bool IsReading();

private:
	HANDLE	LabraryHandle;
};

#endif // !defined(AFX_RAutoLibrary_H__BFCDEDC4_9433_4519_B572_424984DF4C5D__INCLUDED_)
