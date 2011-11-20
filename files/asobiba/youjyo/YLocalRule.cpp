// YLocalRule.cpp: YLocalRule クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YLocalRule.h"
#include "YHttpdConf.h"
#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YLocalRule::YLocalRule()
{

}

YLocalRule::~YLocalRule()
{

}

//ローカルルールの読み込み.
void YLocalRule::Load(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/head.txt";

	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		//ローカルルールなし
		return ;
	}

	FILE * fp =  fopen(filename.c_str() , "rb" );
	if (!fp)
	{
		//ローカルルールなし.
		return ;
	}

	int size = statbuf.st_size;
	char * buf =  new char[size + 1];

	size = fread( buf ,sizeof(char), size ,fp);
	fclose(fp);

	//おまじない(w
	buf[size] = '\0';

	LocalRule = buf;

	delete buf;
}
