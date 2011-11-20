// YDATWritter.h: YDATWritter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDATWRITTER_H__5BC5A538_2BA8_477D_8C4B_6DFFB47CE494__INCLUDED_)
#define AFX_YDATWRITTER_H__5BC5A538_2BA8_477D_8C4B_6DFFB47CE494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RConv.h"

class YDATWritter  
{
public:
	YDATWritter();
	virtual ~YDATWritter();

	//通常書き込み
	static string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText,const string & inAppend);

	//スレたて
	static string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText , const string & inSubject,const string & inAppend);

	//あぼーん
	static string makeAborn(const string & inAbornName);

	//システム特権で書き込み
	static string YDATWritter::makeSystem(const string & inName ,const string & inMail ,const string & inText);

	static void test();

	static string MessageESC(const string & inBaee , const string & inString)
	{
		return RConv::nl2br(AppendTagHtml(inBaee , RConv::htmlspecialchars(inString)));
	}


private:
	static const string AppendTagHtml(const string & inBaee , const string & inTarget);

};

#endif // !defined(AFX_YDATWRITTER_H__5BC5A538_2BA8_477D_8C4B_6DFFB47CE494__INCLUDED_)
