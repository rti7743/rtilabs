// YDeleteWait.cpp: YDeleteWait クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YDeleteWait.h"
#include "YHttpdConf.h"
#include "YDATReader.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YDeleteWait::YDeleteWait()
{

}

YDeleteWait::~YDeleteWait()
{
	Clear();
}

//すべて削除
void YDeleteWait::Clear()
{
	for(YDELETE_WAIT_DATA::const_iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		delete (*i);
	}
	Pool.clear();

}

void YDeleteWait::Load(const string inBoard) 
{
	Clear();

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoard + "/delete.txt";

	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		return ;
	}

	char buf[DELETE_TXT_LINE];

	while( !feof(fp) )
	{
		if ( fgets(buf , DELETE_TXT_LINE , fp) == NULL ) break;

		//削除依頼すれ
		const char* deleteRequest = buf;
		//削除対処すれ
		const char* deleteTarget = YDATReader::next(deleteRequest);
		if (deleteTarget == NULL) break;
		//削除が執行される日付.
		const char* deleteTime = YDATReader::next(deleteTarget);
		if (deleteTime == NULL) break;
		//削除対象 最初の一個目.
		const char* del = YDATReader::next(deleteTime);
		if (del == NULL) break;

		int targetNo[DELETE_REQUEST_MAX];
		//削除対象を読み込み細大12個.   12-1 読み込み.
		int i;
		for(i = 0 ; i < DELETE_REQUEST_MAX ; i ++)
		{
			const char* n = YDATReader::next( del );
			if (n == NULL)
			{
				break;
			}
			targetNo[i] = atoi( string(del , 0 , n - del).c_str() );

			del = n;
		}

		Pool.push_back( new YDeleteWaitData(	
					atol( string(deleteRequest , 0 , deleteTarget - buf).c_str() )	,
					atol( string(deleteTarget , 0 , deleteTime - deleteRequest).c_str() )	,
					atol( string(deleteTime , 0 , del - deleteTarget).c_str() )	,
					i ,
					targetNo
			) );
	}
	fclose(fp);

	std::sort(Pool.begin() , Pool.end() ,YDeleteWaitData::Compre);
}

void YDeleteWait::Save(const string inBoard) 
{
	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoard + "/delete.txt";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		return ;
	}


	for(YDELETE_WAIT_DATA::const_iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		string l = (*i)->getCompateLine() + "\n";

		fputs(l.c_str() , fp);
	}
	fclose(fp);
}


/**
 * Check:				時間がすぎていたらあぼーんする.
 *
 * @param outShareBuffer
 * @param inRequestIta 
 * @param inTargetIta 
 * @return void 
 */
void YDeleteWait::Check(char* outShareBuffer,const string inRequestIta , const string inTargetIta)
{
	time_t timeLimit = time(NULL);

	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; )
	{
		//時間がきていたらあぼーんする.
		YDeleteWaitData * deleteData = (*i);
		if ( deleteData->isTimeLimit(timeLimit) )
		{
			deleteData->DoAborn(outShareBuffer , inRequestIta , inTargetIta);
			i = Pool.erase(i);
			delete deleteData;
		}
		else		break;	//常に昇順にソートされているので、これ以上調べても意味がない.
	}
}



//あぼーん実行.
void YDeleteWaitData::DoAborn(char* outShareBuffer,const string inRequestIta , const string inTargetIta)
{
	try
	{
		//削除実行
		{
			YBBSManager::getInstance()->Aborn(inTargetIta , this->DeleteTarget , this->DeleteSize , this->Delete);
		}
		/*ChangeLog.txt:2003/02/22	意義 -> 異議
		*/
		//削除完了の書き込み.
		{
			POST	post;
			post["bbs"]  = inRequestIta;
			post["FROM"]  = "YoujyoKiller★";
			post["mail"]  = "age";
			post["MESSAGE"]  = "時間まで、異議の申し出がなかったために、リクエストされていた発言は削除されました。";
			post["key"]  = num2str( this->DeleteRequest );
			post["submit"] = "書き込む";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
		//削除が執行されたことをターゲットスレに書き込む
		{
			POST	post;
			post["bbs"]  = inTargetIta;
			post["FROM"]  = "YoujyoKiller★";
			post["mail"]  = "age";
			post["MESSAGE"]  = "時間まで、異議の申し出がなかったために、削除依頼は執行されました。";
			post["key"]  = num2str( this->DeleteTarget );
			post["submit"] = "書き込む";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
	}
	catch(RException e)
	{
		printf("削除エラーが発生しました!! %s.\n削除依頼 %s / ターゲット %s / 削除依頼DAT %ld\n" ,e.getMessage() ,inRequestIta.c_str() , inTargetIta.c_str() ,this->DeleteRequest);
		try
		{
			//削除完了の書き込み.
			{
				POST	post;
				post["bbs"]  = inRequestIta;
				post["FROM"]  = "YoujyoKiller★";
				post["mail"]  = "age";
				post["MESSAGE"]  = string("削除時にエラーが発生しました。<br>削除は失敗しました。<br>管理人にご連絡ください<br>") + e.getMessage();
				post["key"]  = num2str( this->DeleteRequest );
				post["submit"] = "書き込む";

				YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
			}
		}
		catch(RException e)
		{
			//エラー報告がエラーったら、もうだめぽ.
			printf("削除エラーの報告に失敗しました、もうだめぽ!! %s.\n削除依頼 %s / ターゲット %s / 削除依頼DAT %ld\n" ,e.getMessage() ,inRequestIta.c_str() , inTargetIta.c_str() ,this->DeleteRequest);
		}
	}
}

//削除依頼の提出.
void YDeleteWait::Presentation(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , 
									time_t inDeleteTime , int inSize ,const int*	inDelete)
{
	Pool.push_back( new YDeleteWaitData(	
				inDeleteRequest	,
				inDeleteTarget	,
				inDeleteTime	,
				inSize ,
				inDelete
		) );
	std::sort(Pool.begin() , Pool.end() ,YDeleteWaitData::Compre);
}

//削除依頼のキャンセル.
bool YDeleteWait::Cancel(DATNUMBER inDeleteRequest)
{
	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		YDeleteWaitData * deleteData = (*i);
		if (deleteData->IsYourRequest(inDeleteRequest) )
		{
			Pool.erase(i);
			delete deleteData;
			return true;
		}
	}
	//そんなリクエストありません.
	return false;
}

//あなたが保持している削除依頼ですか?
DATNUMBER YDeleteWait::IsYourRequest(DATNUMBER inDeleteRequest)
{
	for(YDELETE_WAIT_DATA::iterator i = Pool.begin() ; i != Pool.end() ; ++i)
	{
		YDeleteWaitData * deleteData = (*i);
		if (deleteData->IsYourRequest(inDeleteRequest) )
		{
			return deleteData->getDeleteTarget();
		}
	}
	//そんなリクエストありません.
	return 0;
}


void YDeleteWait::test()
{
	int deleted[12];
	string r;
	{
		YDeleteWait d;

		deleted[0] = 1;
		deleted[1] = 2;
		deleted[2] = 3;
		deleted[3] = 4;
		d.Presentation(10, 100 , 1000 , 4 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}
	}
	{
		YDeleteWait d;

		deleted[0] = 1;
		deleted[1] = 2;
		deleted[2] = 3;
		deleted[3] = 4;
		d.Presentation(10, 100 , 1000 , 4 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}

		deleted[0] = 5;
		deleted[1] = 4;
		deleted[2] = 3;
		deleted[3] = 2;
		deleted[4] = 1;
		d.Presentation(510, 5100 , 100 , 5 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");
		}

		deleted[0] = 9;
		deleted[1] = 8;
		deleted[2] = 7;
		deleted[3] = 6;
		deleted[4] = 5;
		d.Presentation(810, 8100 , 10000 , 5 , deleted );
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "810<>8100<>10000<>9<>8<>7<>6<>5<>");
		}

		d.Save("test");
		d.Load("test");
		{
			YDELETE_WAIT_DATA::iterator i = d.Pool.begin() ;
			YDeleteWaitData* p = *i;
			ASSERT( (r = p->getCompateLine() ) == "510<>5100<>100<>5<>4<>3<>2<>1<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "10<>100<>1000<>1<>2<>3<>4<>");

			++i;
			p = *i;
			ASSERT( (r = p->getCompateLine() ) == "810<>8100<>10000<>9<>8<>7<>6<>5<>");
		}
	}
	puts("YDeleteWait::test() OK");
}
