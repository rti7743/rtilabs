// RMimeType.h: RMimeType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_)
#define AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RAlphabetHash.h"


class RMimeType  
{
private:		//singleton
	RMimeType();
public:
	virtual ~RMimeType();

	void Load(const string inMimeFilname);

	/**
	 * getMimeType:			Mimeの取得
	 *
	 * @param inStr				取得したい拡張子(ピリオドを含めてはいけない。) 
	 * @return const string		Mime
	 */
	const string getMimeType(const string inStr) const
	{
//		map< unsigned long , string>::const_iterator i = MimeType.find( RAlphabetHash(inStr.c_str()) );
		RStringMap::const_iterator i = MimeType.find( inStr );
		if (i == MimeType.end() ) return "text/plain";	//不明なときは取り合えず
		return (*i).second;
	}

	static RMimeType* getInstance();

	static void test();

private:
	//次の区切りまで読み飛ばし.
	const char* nextSpace(const char* inStr) const;
	//次の区切りではない文字まで読み飛ばし.
	const char* nextNonSpace(const char* inStr) const;

//	map< unsigned long , string>	MimeType;
	RStringMap						MimeType;
};

#endif // !defined(AFX_RMIMETYPE_H__689723A0_9D86_4926_859B_23E81AAD9B9E__INCLUDED_)
