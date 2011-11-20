// RConv.h: RConv クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_)
#define AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"



#define TRIM_SPACE(x)	((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')

class RConv  
{
public:
	RConv()
	{
	}
	virtual ~RConv()
	{
	}

	//URLを文字列に変換 
	static string UrlEncode(string inUrl);
	//置換.
	static string Replace(const string inTarget ,const string inOld ,const string inNew);
	//左空白削除.
	static string ltrim(const string inStr);
	//右空白削除.
	static string rtrim(const string inStr);
	//数字を文字列に変換.
	static string itoa(int inInt);
	//タグをエスケープ 基本的に PHP の htmlspecialchars と同じ.
	static string htmlspecialchars(const string inStr);
	//\nを<br>に 基本的に PHP の nl2br と同じ.
	static string nl2br(const string inStr);
	//左右のホワイトスペースの除去
	static string chop(const string inStr)
	{
		return ltrim(rtrim(inStr));
	}

	//SJIS の1バイト目だけを残すような制御文字アタックを空白を追加
	static string SJISDangerESC(const string inStr);


	static void test();
};

#endif // !defined(AFX_RCONV_H__824C1683_95AF_41A9_8C93_E6AF692BDF10__INCLUDED_)
