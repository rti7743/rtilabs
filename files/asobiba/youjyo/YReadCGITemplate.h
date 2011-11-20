// YReadCGITemplate.h: YReadCGITemplate クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_)
#define AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHtmlTemplate.h"
#include "YDAT.h"
#include "YSettingTXT.h"

class YReadCGITemplate  
{
public:
	YReadCGITemplate();
	virtual ~YReadCGITemplate();

	void Load(const string inBoardName ,const YSettingTXT*	inSettingTXT);

	/**
	 * make:					指定された範囲内でスレッドの中身を html で表示する.
	 *
	 * @param inDAT				使用する DAT.
	 * @param inStart			開始位置
	 * @param inEnd 
	 * @param is1san 
	 * @return const string 
	 */
	const string make(YDAT* inDAT ,unsigned int inStart ,unsigned int inEnd , bool is1san) const;

	static void test();

private:

	//ページ割ループ
	const RHtmlTemplate* PageJump;
	//前100件
	const RHtmlTemplate* IfBefor100;
	//次100件
	const RHtmlTemplate* IfNext100;
	//書き込み表示部分. メインですな。
	const RHtmlTemplate* Kakiko;
	//レス数が<!--?V:YResAlert-->を超えています。<!--?V:YResOver-->を超えると表示できなくなるよ。
	const RHtmlTemplate* IfResWarning;
	//サイズが<!--?V:YSizeAlert-->KBを超えています。<!--?V:YSizeOver-->KBを超えると表示できなくなるよ。
	const RHtmlTemplate* IfSizeWarning;
	//レス数が<!--?V:YResOver-->を超えています。残念ながら全部は表示しません。
	const RHtmlTemplate* IfResOver;

	string	BoardName;
	
	RHtmlTemplate		Template;
	const YSettingTXT*	SettingTXT;

};

#endif // !defined(AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_)
