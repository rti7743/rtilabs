// RExceptionUtil.h: RExceptionUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
#define AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>
#include <atlbase.h>


/**
 * class RExceptionUtil:	例外を作成するヘルパー関数集
 *
 * @author 
 */
class RExceptionUtil  
{
public:
	RExceptionUtil();
	virtual ~RExceptionUtil();

	/**
	 * WindowsCodeToString:	指定されたWindowsのエラーコード(::GetLastError()) を日本語エラーに変換する.
	 *
	 * @param inErrorCode	エラーコード(::GetLastError())
	 * @return static string エラーメッセージ
	 */
	static string WindowsCodeToString(DWORD inErrorCode);

	/**
	 * RExceptionUtil:	COM のエラーメッセージに変換する
	 *
	 * @param inComError	エラー(::GetLastError())
	 * @return static string エラーメッセージ
	 */
	string RExceptionUtil::ComErrorToString(_com_error &inComError);
};

#endif // !defined(AFX_RExceptionUtil_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
