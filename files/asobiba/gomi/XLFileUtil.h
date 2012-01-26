// XLFileUtil.h: XLFileUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_)
#define AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLFileUtil  
{
public:
	XLFileUtil();
	virtual ~XLFileUtil();

	//ファイルが存在するか?
	static bool XLFileUtil::Exist(const string & inFileName);
	//ファイルをすべて string に読み込む.
	static string XLFileUtil::FileToString(const string & inFileName);
	//inStr を ファイルに書き込む
	static void XLFileUtil::StringToFile(const string & inStr , const string & inFileName);
	//inBuffer を ファイルに書き込む
	static void XLFileUtil::BufferToFile(const XLBuffer & inBuffer , const string & inFileName);

};

#endif // !defined(AFX_XLFILEUTIL_H__A51E414C_8F9A_46E5_A5CF_364F04C9FA00__INCLUDED_)
