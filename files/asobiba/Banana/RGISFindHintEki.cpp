// RGISFindHintEki.cpp: RGISFindHintEki クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISFindHintEki.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISFindHintEki::RGISFindHintEki()
{

}

RGISFindHintEki::~RGISFindHintEki()
{
	Delete();
}


void RGISFindHintEki::Create(unsigned long inBlock,const string & inName , unsigned short inNameIndex)
{
	FindHintEkiData data;
	data.Block = inBlock;
	data.NameIndex = inNameIndex;

	if (inName.size() >= NAME_SHORT_CUT_SIZE) 
	{
		memcpy(data.NameShortCut , inName.c_str() , NAME_SHORT_CUT_SIZE);
	}
	else
	{
		memset(data.NameShortCut , 0 , NAME_SHORT_CUT_SIZE );
		memcpy(data.NameShortCut , inName.c_str() , inName.size() );
	}

	UniqEki::iterator i = Eki.begin();
	UniqEki::iterator e = Eki.end();
	for( ; i != e ; i++)
	{
		if ( (*i)->NameIndex == data.NameIndex && (*i)->Block == data.Block && 
			memcmp( (*i)->NameShortCut , data.NameShortCut , NAME_SHORT_CUT_SIZE) == 0 ) return ;
	}
	//見つからなかったので追加.
	FindHintEkiData * p = new FindHintEkiData;
	*p = data;
	Eki.push_back( p );
}

void RGISFindHintEki::Write(const string & inDir) throw (RException)
{
	RStdioFile file;

	file.Open(inDir + "ekiHint", "a+b");
	UniqEki::iterator i = Eki.begin();
	UniqEki::iterator e = Eki.end();
	for( ; i != e ; i++)
	{
		file.fwrite( &((*i)->NameShortCut) , sizeof(unsigned char) * NAME_SHORT_CUT_SIZE);
		file.fwrite( &((*i)->Block) , sizeof(unsigned long) );
		file.fwrite( &((*i)->NameIndex) , sizeof(unsigned short) );
	}
}

void RGISFindHintEki::Delete()
{
	UniqEki::iterator i = Eki.begin();
	UniqEki::iterator e = Eki.end();
	for( ; i != e ; i++)
	{
		delete (*i);
	}
	Eki.clear();
}

JoinHeadFormatScript RGISFindHintEki::getPackInfo() 
{
	JoinHeadFormatScript ret;
	ret.TempFilename = "ekiHint";
	ret.BlockStartSimbol = 'E';
	ret.DataSize = sizeof(unsigned short) + sizeof(unsigned long) + (sizeof(unsigned char) * NAME_SHORT_CUT_SIZE);	//2 + 4 + 8 = 14
	ret.HeadType = HeadTypeFixed;

	return ret;
}




