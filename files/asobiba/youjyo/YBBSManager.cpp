// YBBSManager.cpp: YBBSManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YBBSManager.h"
#include "RStreamMemory.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YBBSManager::YBBSManager()
{
}



YBBSManager::~YBBSManager()
{
	ClearBBS();
}

void YBBSManager::Read(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer )
				throw(RNotFoundException,RIOCannotOpenException)
{
	this->getBBS( inRequestPath->getBoardName()  )->Read( outShareBuffer , inRHTTPHeader, inRequestPath , outBuffer );
}

void YBBSManager::Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer )
				throw(RNotFoundException,RIOCannotOpenException)
{
	YBBS* bbs = this->getBBS( ioPost["bbs"] );

	const string submit = ioPost["submit"];
	if (submit == "書き込む" )
	{
		bbs->Write( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
	else if (submit == "新規スレッド作成画面へ")
	{
		bbs->NewThread( outShareBuffer , ioPost , outBuffer );
	}
	else if (submit == "新規スレッド作成")
	{
		bbs->New( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
	else if (!ioPost["saku"].empty() )
	{	//削除依頼へ
		bbs->NewSakujyoIrai(outShareBuffer , inRHTTPHeader ,ioPost, outBuffer);
	}
	else
	{//とりあえず書き込み.
		bbs->Write( outShareBuffer , inRHTTPHeader, ioPost , outBuffer );
	}
}

/**
 * SystemWrite:				システムからのメッセージ
 *
 * @param outShareBuffer	TLSバッファ.
 * @param ioPost			書き込む内容
 * @return void 
 */
void YBBSManager::SystemWrite(char* outShareBuffer,POST& ioPost)
				throw(RNotFoundException,RIOCannotOpenException,RNotFoundException)
{
	this->getBBS( ioPost["bbs"] )->SystemWrite(outShareBuffer , ioPost);
}

//削除
void YBBSManager::Aborn(const string inTargetIta ,DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete)
																throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
	this->getBBS(inTargetIta)->Aborn(inDeleteTarget , inDeleteSize , inDelete);
}


//特定の板の特定のスレッドのタイトルの取得.
const string YBBSManager::getSubject(const string inBoardName , DATNUMBER inDatNumber) throw(RNotFoundException)
{
	return this->getBBS(inBoardName)->getSubject(inDatNumber);
}


YBBSManager* YBBSManager::getInstance()
{
	static	YBBSManager	ybbsmanager;
	return &ybbsmanager;
}

//削除人タスク.
void YBBSManager::KillerTask(char* outShareBuffer)
{
	//削除依頼の状態を見て、
	//削除するべきであれば削除する.
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->KillerTask(outShareBuffer);
	}
}

//DATガページコレクタ
void YBBSManager::GCTask()
{
	//削除依頼の状態を見て、
	//削除するべきであれば削除する.
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->GCTask();
	}
}

//情報を出力.
void YBBSManager::Dump()
{
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->Dump();
	}
}
///すべての情報をディスクにフラッシュする.
void YBBSManager::Flush()
{
	puts("★Flush開始");
	for(BBSS::const_iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		YBBS* bbs = (*i).second;
		bbs->Flush();
	}
	puts("★Flush終了");
}

//BBSを再読み込み
void YBBSManager::ReloadBBS() throw(RException)
{
	ClearBBS();

	const string htdocs = YHttpdConf::getInstance()->getDocumentRoot() + "/";
	const string findpath = htdocs + "*";

	WIN32_FIND_DATA data;
	HANDLE p = ::FindFirstFile( findpath.c_str() , &data);
	if (p == INVALID_HANDLE_VALUE)
	{
		throw RNotFoundException("指定されたディレクトリ(%s)には、掲示板らしきディレクトリがありません" , htdocs.c_str() );
	}

	do
	{
		//ディレクトリでなければ没.
		if ( !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) continue;
		// . または .. のファイルは無視.
		if (data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') )) 
			continue;
		//掲示板ディレクトリかどうかは、 SETTING.TXT で判断します(手抜き)
		const string fullname = htdocs + data.cFileName + "/SETTING.TXT";
		if (! FILE_EXIST(fullname.c_str() ) )	continue;

		YBBS* bbs = NULL;
		try
		{
			//登録.
			bbs = new YBBS( string(data.cFileName) );
			this->AllBBS[ string(data.cFileName) ] = bbs;
		}
		catch(RException e)
		{
			delete bbs;
			throw ;
		}

		//登録メッセージ.
		printf("掲示板登録... %s\r\n",data.cFileName);
	}
	while( ::FindNextFile( p , &data) );
	::FindClose(p);

}

//BBSをすべて破棄する.
void YBBSManager::ClearBBS()
{
	//すべて解放.
	for(BBSS::iterator i = AllBBS.begin() ; i != AllBBS.end() ; ++i)
	{
		delete (*i).second;
	}
	AllBBS.clear();
}

//削除依頼された書き込みのやばい単語をマスクする.
void YBBSManager::MaskReWrite(const string inTargetIta  ,DATNUMBER inDatNumber ,const string & password ,
	 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
	 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
	this->getBBS(inTargetIta)->MaskReWrite(inDatNumber , password , inDeleteSize , inDelete , inMaskSize , inMask);
}


/**
 * getBBS:					指定された板に対するBBSを取得.
 *
 * @param inTargetIta 
 * @return YBBS* 
 */
YBBS* YBBSManager::getBBS(const string &inTargetIta) throw(RNotFoundException)
{
	BBSS::const_iterator i = AllBBS.find( inTargetIta );
	if (i == AllBBS.end() )
	{
		throw RNotFoundException(EXCEPTIONTRACE + inTargetIta + "という名前の掲示板はありません");
	}
	return (*i).second;
}