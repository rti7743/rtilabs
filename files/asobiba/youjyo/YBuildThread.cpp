// YBuildThread.cpp: YBuildThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YBuildThread.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YBuildThread::YBuildThread()
{

}

YBuildThread::~YBuildThread()
{

}


void YBuildThread::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Template.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/buildthread.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)buildthread.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}
	this->BoardName = inBoardName;
	this->SettingTXT = inSettingTXT;
}

const string YBuildThread::make(POST& ioPOST, YLocalRule* inLocalRule) const
{
	RAutoReadSpinLock al(&Lock);

	RStringMap smap;
	smap["YItaLocalRule"] = "";
	smap["YItaName"] = BoardName;
	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YRequestThread"] = ioPOST["key"];
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	return Template.Replace(&smap);
}

