// RDataLoad.cpp: RDataLoad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"

#include "regex.h"

#include "RDataLoad.h"
#include "RConv.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDataLoad::RDataLoad()
{

}

RDataLoad::~RDataLoad()
{

}

string RDataLoad::Load(string inSetupFilename,string inSection,string inKey) 
				throw(RNotFoundException,RIOCannotOpenException,RIOCannotReadException)
{
	RStdioFile	StdFile;
	StdFile.Open(inSetupFilename ,"rb");

	string inNowSection = "名無しさん@おなかいっぱい";
	string buf;

	//セクションシーク.
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':' && buf.substr(1) == inSection) break;
	}

	int keysize = inKey.size();
	boost::reg_expression<char> dataRegex  = "\\s(.*)$";

	//キー
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':') break;		//別のセクションにはいったので、そのキーはないということで.

		if (buf.size() > keysize)
		{
			if (inKey == buf.substr(0,keysize) )
			{
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, dataRegex) )	return results[1];
			}
		}
	}

	throw RNotFoundException("検索対象の %s:%s は、ありません" ,inSection.c_str() ,inKey.c_str() );
}

void RDataLoad::Save(string inSetupFilename,string inSection,string inKey, string inData) 
			throw(RNotFoundException,RIOCannotOpenException,RIOCannotWriteException)
{
}

