// RConfigFile.h: RConfigFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
#define AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RConfigFile  
{
public:
	RConfigFile();
	virtual ~RConfigFile();

public:
	/**
	 * Load:				設定ファイルを読み込む.
	 *
	 * @param inFilename	ファイル名
	 * @return void 
	 */
	void Load(const string inFilename);

public:	//サービス
	//基本形.
	template <class _T>
	_T LoadOrDefault(const string inKeyName ,_T inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		string a = ((*i).second);
		return _T( a );
	}

	//string の場合.
	template<>
	string LoadOrDefault<const string>(const string inKeyName ,string inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return (*i).second;
	}
	//int の場合.
	template<>
	int LoadOrDefault<int>(const string inKeyName ,int inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() );
	}

	static void test();
public:
	RStringMap		Setting;
};

#endif // !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
