// RGISAtr.cpp: RGISAtr クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISAtr.h"
#include "RConv.h"
#include "RStdioFile.h"
#include "RGIS2500Head.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISAtr::RGISAtr()
{
	Count = 0;
	Points= NULL;
}

RGISAtr::~RGISAtr()
{
	Delete();
}

void RGISAtr::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISAtr::Create(const string & inFilename, int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	RStdioFile file;
	file.Open(inFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	//ヘッダーの取得.
	RGIS2500Head	head;
	head.Create( buffer.c_str() ,inScale);

	//要素の総数の取得.
	this->Count = head.getCount();
	this->Points = new AtrData[this->Count];

	char nameCVSBuffer[65535];


	int line = 0;
	for(int i = 0  ; !file.IsEof(); i++ , line ++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		//読み込み.		
		AtrData* p = &Points[i];
		char mark;
		int rsize = sscanf(buffer.c_str() , "%c%d,%d,%65535s" ,&mark, &p->Type , &p->No,nameCVSBuffer);
		if (rsize != 4)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %d ソース[%s]" ,rsize , buffer.c_str()  ) );
		}

		RStringList attribList = RConv::Explode(nameCVSBuffer);
		if ( attribList.size() > 1 ) p->attrib[0] = attribList[1];
		if ( attribList.size() > 2 ) p->attrib[1] = attribList[2];
		if ( attribList.size() > 3 ) p->attrib[2] = attribList[3];
		if ( attribList.size() > 4 ) p->attrib[3] = attribList[4];
	}
}
