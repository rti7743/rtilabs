// RDateConv.h: RDateConv クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_)
#define AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDateConv  
{
public:
	RDateConv()
	{
	}
	virtual ~RDateConv()
	{
	}
	//Unix時間を Http 時間に変換.RFC 1123 (RFC 822) に従う
	static string UnixTimeToHttpTime(time_t inTime);
	static time_t HttpTimeToUnixTime(const char* date);

	/**
	 * UnixTime2chTime:			Unix時間を 2002/11/14 00:55 形式に文字列として変換します
	 *
	 * @param inTime			Unix時間
	 * @return string			2002/11/14 00:55 形式に文字列として変換します
	 */
	static string UnixTime2chTime(time_t inTime);


	static void test();

private:

	static int RDateConv::ap_checkmask(const char *data, const char *mask);
	static time_t RDateConv::ap_tm2sec(const struct tm * t);

	
	static time_t tm2time(struct tm* when);
};

#endif // !defined(AFX_RDATECONV_H__58F23618_282C_4CC8_BAE0_2AEC4C1DAEC8__INCLUDED_)
