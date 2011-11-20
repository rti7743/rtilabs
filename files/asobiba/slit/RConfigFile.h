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
	void Load(const string & inFilename);

	/**
	 * Save:				書き込み
	 *
	 * @param inFilename	ファイル名
	 * @return void 
	 */
	void Save(const string & inFilename) ;

public:	//サービス
	//基本形.
	template<typename _T>
	_T LoadOrDefault(const string & inKeyName ,_T inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() );
	}

	//string の場合.
	template<>
	string LoadOrDefault<string>(const string & inKeyName ,string inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() )
		{
			return inDefualt;
		}
		return (*i).second;
	}
	//bool
	template<>
	bool LoadOrDefault<string>(const string & inKeyName ,bool inDefualt  ) const
	{
		RStringMap::const_iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return inDefualt;
		return atoi( (*i).second.c_str() ) == 0 ? false : true;
	}

	//基本形.
	template <typename _T>
	void Upsert(const string & inKeyName ,_T inValue  ) 
	{
		Setting[inKeyName] = num2str(inValue);
	}
	template<>
	void Upsert<string>(const string & inKeyName ,string  inValue)
	{
		Setting[inKeyName] = inValue;
	}
	template<>
	void Upsert<bool>(const string & inKeyName ,bool  inValue)
	{
		Setting[inKeyName] = num2str(inValue == true ? 1 : 0);
	}

	void Remove(const string & inKeyName ) 
	{
		RStringMap::iterator i = Setting.find(inKeyName);
		if (i == Setting.end() ) return ;

		Setting.erase(i);
	}

	static void test();
public:
	RStringMap		Setting;
};

#endif // !defined(AFX_RCONFIGFILE_H__C605DDA7_2339_4567_9FDE_925ECE9F1567__INCLUDED_)
