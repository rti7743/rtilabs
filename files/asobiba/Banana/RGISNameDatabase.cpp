// RGISNameDatabase.cpp: RGISNameDatabase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISNameDatabase.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISNameDatabase::RGISNameDatabase()
{
	Strings.push_back("");
}

RGISNameDatabase::~RGISNameDatabase()
{

}

unsigned short RGISNameDatabase::setName(const char* inName) throw(RException)
{
	//空文字
	if (inName[0] == '\0')	return 0;

	//その他は検索.
	int i;
	int max = Strings.size();
	for(i = 1 ; i < max ; i ++)
	{
		if ( Strings[i] == inName )
		{
			return i;
		}
	}

	if (i > 0xffff)
	{
		throw RException(EXCEPTIONTRACE + "RGISNameDatabase::setName 65535 以上の文字列を登録しようとしました");
	}

	//ないので追加.
	Strings.push_back(inName);

//	TRACE("RGISNameDatabase::alloc %d to %s\n" , i , inName);

	return i;
}

const string RGISNameDatabase::getName(unsigned short inIndex) const
{
	ASSERT(inIndex <= Strings.size() );

	return Strings[inIndex];
}


JoinTempFormatScript RGISNameDatabase::getPackInfo(const string & inDirectory , unsigned char inSimbol) 
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = inDirectory;
	ret.BlockStartSimbol = inSimbol;
	ret.DataSize = 0;
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeString;
	ret.WithNode = false;

	return ret;
}
