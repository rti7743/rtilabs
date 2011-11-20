// XLExceptionUtil.h: XLExceptionUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
#define AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * class XLExceptionUtil:	例外を作成するヘルパー関数集
 *
 * @author 
 */
class XLExceptionUtil  
{
public:
	XLExceptionUtil();
	virtual ~XLExceptionUtil();

	/**
	 * WindowsCodeToString:	指定されたWindowsのエラーコード(::GetLastError()) を日本語エラーに変換する.
	 *
	 * @param inErrorCode	エラーコード(::GetLastError())
	 * @return static string エラーメッセージ
	 */
	static string WindowsCodeToString(DWORD inErrorCode);
};

#endif // !defined(AFX_XLEXCEPTIONUTIL_H__B13BED1D_08D1_4762_88E0_4672F31B4D7C__INCLUDED_)
