// YWriteHTML.cpp: YWriteHTML クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YWriteHTML.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YWriteHTML::YWriteHTML()
{

}

YWriteHTML::~YWriteHTML()
{

}

void YWriteHTML::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Error.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\error.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)error.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	try
	{
		KakikoSimasuta.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\kakikomi.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)kakikomi.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	try
	{
		KakikoKakunin.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\kakunin.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)kakunin.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	this->BoardName = inBoardName;
	this->SettingTXT = inSettingTXT;
}



//エラー作成.
const string YWriteHTML::makeError(const string & inErrorMessage , const string & inIP , 
								   const string & inFROM , const string & inMail , const string & inMESSAGE) const
{
	RStringMap smap;
	smap["YErrorString"] = inErrorMessage;
	smap["YIPAddress"] = inIP;
	smap["YKakikoName"] = inFROM;
	smap["YKakikoMail"] = inMail;
	smap["YkakikoText"] = inMESSAGE;
	smap["YItaName"] = BoardName;

	return Error.Replace(&smap);
}

//書き込みました作成.
const string YWriteHTML::makeKakikoSimasuta() const
{
	RStringMap smap;
	smap["YItaName"] = BoardName;

	return KakikoSimasuta.Replace(&smap);
}

//書き込み確認作成.
const string YWriteHTML::makeKakikoKakunin(const string & inIP , const string & inFROM , const string & inMail , const string & inMESSAGE) const
{
	RStringMap smap;
	smap["YIPAddress"] = inIP;
	smap["YKakikoName"] = inFROM;
	smap["YKakikoMail"] = inMail;
	smap["YkakikoText"] = inMESSAGE;
	smap["YItaName"] = BoardName;

	return KakikoKakunin.Replace(&smap);
}

