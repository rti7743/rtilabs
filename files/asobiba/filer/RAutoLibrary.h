// RAutoLibrary.h: RAutoLibrary クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_)
#define AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RAutoLibrary  
{
public:
	//構築
	RAutoLibrary()
	{
		LabraryHandle = NULL;
	}
	//指定した名前で構築
	RAutoLibrary(const char* inDLLName)
	{
		Create(inDLLName);
	}

	//破棄
	virtual ~RAutoLibrary()
	{
		Destory();
	}

	//DLL設定
	//@inDLLName	DLLの名前
	bool Create(const char* inDLLName)
	{
		Destory();

		LabraryHandle = ::LoadLibrary(inDLLName);
		return IsReading();
	}
	//DLL破棄
	void Destory()
	{
		if (!IsReading()) return;
		::FreeLibrary((HINSTANCE)LabraryHandle);
	}	

	//DLLから 関数を取得
	//@inFunctionName		関数名
	FARPROC GetProcAddress(const char* inFunctionName)
	{
		if (!IsReading()) return NULL;
		return ::GetProcAddress((HINSTANCE)LabraryHandle, inFunctionName);
	}

	//DLLは読み込まれているの?
	bool IsReading()
	{
		return LabraryHandle != NULL;
	}

private:
	HANDLE	LabraryHandle;
};

#endif // !defined(AFX_RAUTOLIBRARY_H__7B847D20_EF6F_4B92_BC7F_C31437A54806__INCLUDED_)
