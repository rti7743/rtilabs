// YWriteHTML.h: YWriteHTML クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_)
#define AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHtmlTemplate.h"
#include "YSettingTXT.h"

class YWriteHTML  
{
public:
	YWriteHTML();
	virtual ~YWriteHTML();

	void Load(const string inBoardName,const YSettingTXT*	inSettingTXT);

	//エラー作成.
	const string makeError(const string & inErrorMessage , const string & inIP , 
								   const string & inFROM , const string & inMail , const string & inMESSAGE) const;
	//書き込みました作成.
	const string makeKakikoSimasuta() const;
	//書き込み確認作成.
	const string makeKakikoKakunin(const string & inIP , const string & inFROM , const string & inMail , const string & inMESSAGE) const;
private:

	//エラー
	RHtmlTemplate	Error;
	//書き込みました
	RHtmlTemplate	KakikoSimasuta;
	//書き込み確認.
	RHtmlTemplate	KakikoKakunin;

	string BoardName;

	const YSettingTXT*	SettingTXT;

};

#endif // !defined(AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_)
