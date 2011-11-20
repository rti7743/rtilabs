// YSubBackHTML.h: YSubBackHTML クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSUBBACKHTML_H__14E0B251_D160_49C6_91F0_B9351C0EB4D8__INCLUDED_)
#define AFX_YSUBBACKHTML_H__14E0B251_D160_49C6_91F0_B9351C0EB4D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YTextFile.h"
#include "RHtmlTemplate.h"
#include "YSubject.h"
#include "RSpinLock.h"
#include "YSettingTXT.h"


class YSubBackHTML  : public YTextFile
{
public:
	YSubBackHTML();
	virtual ~YSubBackHTML();

	void Load(const string inBoardName ,const YSettingTXT*	inSettingTXT);

	/**
	 * Write:					subback.html の作成.
	 *
	 * @param inSubject			subject.txt
	 * @return void 
	 */
	void YSubBackHTML::Write(YSubject* inSubject);

	/**
	 * Show:				表示			 YTextFileより継承
	 *
	 * @param outRPack		これに格納
	 * @return void 
	 */
	virtual void Show(RPack * outRPack) const;


	static void test();
private:
	string			BoardName;
	mutable RSpinLock		Lock;

	string			SubBackHTML;
	RHtmlTemplate	Template;
	RHtmlTemplate*	ThreadIndex;

	const YSettingTXT*	SettingTXT;

};

#endif // !defined(AFX_YSUBBACKHTML_H__14E0B251_D160_49C6_91F0_B9351C0EB4D8__INCLUDED_)
