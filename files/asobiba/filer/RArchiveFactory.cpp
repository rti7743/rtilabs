// RArchiveFactory.cpp: RArchiveFactory クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveFactory.h"
#include "RArchiveLha.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RArchiveFactory::RArchiveFactory()
{

}

RArchiveFactory::~RArchiveFactory()
{

}

//サポートしていないやつは RNotSupportException
//!初期化できなかったら RDoNotInitaliseException
//!関数が読み込めなかったら RDoNotInitaliseException
RArchive* RArchiveFactory::New(__ARCHIVETYPE inArchiveType) throw()
{
	RArchive* p = NULL;
	try
	{
		switch(inArchiveType)
		{
		case __ARCHIVETYPE_LZH:
			p = new RArchiveLha();
			p->Create();
			return p;
		}
	}
	catch(RNotSupportException e)
	{
		delete p;
		throw ;
	}
	catch(RDoNotInitaliseException e)
	{
		delete p;
		throw ;
	}
	throw RNotSupportException("RArchiveFactory::指定された圧縮ファイル %d はサポートが何か?",inArchiveType);
	return NULL;
}

