// SusiePluginManager.h: SusiePluginManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
#define AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "SusiePluginWapper.h"

typedef deque<string> SUSIE_EXT;

class SusiePluginManager  
{
private:
	//singleton
	SusiePluginManager();
public:
	virtual ~SusiePluginManager();

	//インスタンス取得
	static SusiePluginManager*	getInstance();

	//拡張子一覧の取得
	SUSIE_EXT getSupportExt() ;


	//サポートしているプラグインきぼんぬ
	//みつからない場合は RNotSupportException 
	const SusiePluginWapper* getSupportPlugin(const string inFilename,const string inExt) throw();
private:
	//プラグインロード
	string PluginLoad(const string inPluginPath);
	//対応するプラグインをマッピング.
	void AppendSupportBy(SusiePluginWapper* inSusiePlugin) throw();
private:
	typedef deque<SusiePluginWapper*>		PLUGINES;
	typedef multimap< string, SusiePluginWapper*>	EXTMAPPER;
	typedef EXTMAPPER::iterator MULTIMAP_RANGE_ITERATOR;

	EXTMAPPER	ExtMapper;
	PLUGINES	Plugines;
};

#endif // !defined(AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
