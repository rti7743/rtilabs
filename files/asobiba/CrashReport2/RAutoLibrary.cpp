// RAutoLibrary.cpp: RAutoLibrary クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RAutoLibrary.h"
#include "RException.h"
#include "RExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//構築
RAutoLibrary::RAutoLibrary()
{
	LabraryHandle = NULL;
}

//指定した名前で構築
//失敗は RDoNotInitaliseException です。
RAutoLibrary::RAutoLibrary(const char* inDLLName)
{
	//コンストラクタで例外はあんまりよくないんだけど、知ったことかボケ。←開き直り
	Load(inDLLName);
}

//破棄
RAutoLibrary::~RAutoLibrary()
{
	Destory();
}

//DLL設定
//@inDLLName	DLLの名前
//失敗は RDoNotInitaliseException です。
void RAutoLibrary::Load(const char* inDLLName) 
{
	Destory();

#ifndef _WIN32_WCE
	LabraryHandle = ::LoadLibrary(inDLLName);
#else
	MY_USES_CONVERSION;
	LabraryHandle = ::LoadLibrary(_A2W(inDLLName) );
#endif
	if (!IsReading())
	{
		DWORD error = ::GetLastError();
		throw RException(PrintFormat("RAutoLibrary:%s の読み込みに失敗しますた。",inDLLName) + RExceptionUtil::WindowsCodeToString(error) );
	}
}
//DLL破棄
void RAutoLibrary::Destory()
{
	if (!IsReading())
	{
		return;
	}
	::FreeLibrary((HINSTANCE)LabraryHandle);
}

//DLLから 関数を取得
//@inFunctionName		関数名
FARPROC RAutoLibrary::GetProcAddress(const char* inFunctionName) 
{
	//初期化しないで読み込もうとか何なの？ばかなのアホなの死ぬの？
	ASSERT(IsReading());

#ifndef _WIN32_WCE
	FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, inFunctionName);
#else
	MY_USES_CONVERSION;
	FARPROC ret = ::GetProcAddress((HINSTANCE)LabraryHandle, _A2W(inFunctionName) );
#endif
	if (ret == NULL)
	{
		DWORD error = ::GetLastError();
		throw RException(PrintFormat("%s 関数はサポートされていません。",inFunctionName) + RExceptionUtil::WindowsCodeToString(error) );
	}
	return ret;
}
//DLLは読み込まれているの?
bool RAutoLibrary::IsReading()
{
	return LabraryHandle != NULL;
}


