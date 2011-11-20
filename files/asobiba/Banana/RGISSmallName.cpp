// RGISSmallName.cpp: RGISSmallName クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallName.h"

#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallName::RGISSmallName()
{

}

RGISSmallName::~RGISSmallName()
{

}


void RGISSmallName::Creae(const string & inFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	char matchListBuffer[1024];
	
	Chain.clear();

	int type;
	char spaceBuffer[1024];

	for(int nowLine = 0 ;  !file.IsEof() ; nowLine ++ )
	{
		string line = file.ReadLine();
		//空行または、コメントは無視.
		if ( line.empty() || line[0] == '#' || line == "\r\n") continue;

		int r = sscanf(line.c_str() , "%d%1024[ ]%s1024", &type , spaceBuffer , matchListBuffer);
		if (r < 1) 
		{
			throw RException(EXCEPTIONTRACE + inFilename + " ファイルの" + num2str(nowLine) + "行目の読み込みに失敗しました");
		}

		if (type < 0 || type > 256)
		{
			throw RException(EXCEPTIONTRACE + inFilename + " ファイルの" + num2str(nowLine) + "行目、種類の範囲は 0 ～ 255 までです");
		}

		//説明がない場合も問題がおきないように。
		if (r != 3)
		{
			matchListBuffer[0] = '\0';
		}
		//格納.
		RGISSmallNameRuleChain* p = new RGISSmallNameRuleChain;
		p->Create( (unsigned char) type , matchListBuffer);
		Chain.push_back( p );
	}
}

bool RGISSmallName::Match(const char* inName , RGISSmallNamePair * outPair) const
{
	newVector<RGISSmallNameRuleChain>::const_iterator i = Chain.begin();
	newVector<RGISSmallNameRuleChain>::const_iterator e = Chain.end();

	int nameLen = strlen(inName);

	for( ; i != e ; i++)
	{
		const RStringList* list = (*i)->getMatchList();
		RStringList::const_iterator is = list->begin();
		RStringList::const_iterator es = list->begin();

		for( ; is != es ; is++ )
		{
			//最後が一致しているか検索.
			const char * p = inName + nameLen - is->size();
			if ( strcmp( is->c_str() , p ) == 0 )
			{
				//一致している.
				//種類を表す部分を切り捨てる.

				//ex.
				//ととろ病院	→	ととろ
				//
				outPair->Name = string(p , 0  , nameLen - is->size() );
				outPair->Type = (*i)->getType();

				return true;
			}
		}
	}
	return false;
}

