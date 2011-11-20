// MVThreadManager.cpp: MVThreadManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVThreadManager.h"
#include "RWaitCursor.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVThreadManager::MVThreadManager()
{
	Now.Run = false;
	Now.Image = NULL;
	Now.Page = -1;

	After.Run = false;
	After.Image = NULL;
	After.Page = -1;

	Before.Run = false;
	Before.Image = NULL;
	Before.Page = -1;

}

MVThreadManager::~MVThreadManager()
{
	AllThreadDestory();
}

/**
 * Update:				新しい本を開く
 *
 * @param inFilename	ファイル名
 * @return void 
 */
void MVThreadManager::Update(const string & inFilename) throw(RException)
{
	RWaitCursor wait;

	//すべてのスレッドを停止させる.
	AllThreadDestory();

	FileList.Update(inFilename);

}

//データの読み込みが実は終わっていない場合は終わるまで待つ.
void MVThreadManager::ThreadJoin(MVThreadManagerImagCache* ioThread)
{
	try
	{
		if (ioThread->Run)
		{
			ioThread->Thread.Join();
			ioThread->Image = ioThread->Thread.TakeoverPageData();

			ioThread->Run = false;
		}
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		delete ioThread->Image;
		ioThread->Image = NULL;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

//データの読み込みが終わっていない場合中断させる.
void MVThreadManager::ThreadAbort(MVThreadManagerImagCache* ioThread)
{
	try
	{
		if (ioThread->Run)
		{
			ioThread->Thread.HoldUp();
			ioThread->Thread.Join();
			ioThread->Image = ioThread->Thread.TakeoverPageData();

			ioThread->Run = false;
		}
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		delete ioThread->Image;
		ioThread->Image = NULL;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

//スレッドの開始する.
void MVThreadManager::ThreadRun(int inPage , MVThreadManagerImagCache* ioThread)
{
	//これから仕事を依頼するのに忙しくてどうするの?
	ASSERT(ioThread->Run == false);

	ASSERT(ioThread->Image == NULL);

	//範囲外のページの読み込みはダメ.
	if (inPage < 0 || inPage >= this->FileList.getCount() )
	{
		ioThread->Page = -1;
		return ;
	}

	try
	{
		//スレッドに必要な情報を記録してもらいます.
		ZipReadInfo zipInfo;
		FileList.getInfo(inPage , &zipInfo );
		
		//仕事を依頼する.
		ioThread->Run = true;
		ioThread->Page = inPage;

		ioThread->Thread.Setting(	zipInfo.buffer , zipInfo.compressedSize , 
										zipInfo.uncompressedSize , zipInfo.compressionMethod , 
										&this->ImageConvertParam );

		//スレッド動作
		ioThread->Thread.Start();

		TRACE("スレッド %p に %dページの仕事を依頼しました\n" , ioThread , inPage);
	}
	catch(RException & e)
	{
		ioThread->Run = false;
		ioThread->Page = -1;
		ioThread->Thread.HoldUp();
		ioThread->Thread.Join();

		RMessageBox::Message(e.getMessage() );
	}
}

/**
 * GotoPage:			指定されたページに移動
 *
 * @param inPage				ページ番号
 * @return const MVPageData*	ページのデータ.
 *
 */
const MVPageData* MVThreadManager::GotoPage(int inPage) throw(RException)
{
	//今の画像を読み込み中はありえない.
	ASSERT(Now.Run == false);

	//同じポインタを指してはいけない. double free してしまう.
	ASSERT((Now.Image == NULL) || (Now.Image != After.Image) );
	ASSERT((Now.Image == NULL) || (Now.Image != Before.Image) );
	//でも、 NULL なら、ぶつかってもしかたないか.
	ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
	//同じページを指しはずがない.
	ASSERT( (Now.Page == -1) || (Now.Page != After.Page) );
	ASSERT( (Now.Page == -1) || (Now.Page != Before.Page) );
	ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );

	//今 のデータ
	if (Now.Page == inPage)
	{
		//何もせずに、今のデータを送信.

		//同じポインタを指してはいけない. double free してしまう.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//でも、 NULL なら、ぶつかってもしかたないか.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//同じページを指しはずがない.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//要求されたページが 次 のページ
	else if (After.Page == inPage)
	{
		//次のデータの読み込みが実は終わっていない場合は終わるまで待つ.
		ThreadJoin(&After);
		//前のデータの読み込みが終わっていない場合中断させる.
		ThreadAbort(&Before);
		ASSERT(After.Run == false);
		ASSERT(Before.Run == false);

		//	前 のデータを破棄.
		delete Before.Image;
		Before.Image = NULL;

		//今 のデータを 前 のデータとして記録.
		Before.Image = Now.Image;
		Before.Page = Now.Page;
		ASSERT(Now.Image == Before.Image);
		ASSERT(Now.Page == Before.Page);
		ASSERT(Before.Run == false);

		//次 のデータを 今 のデータとして記録
		Now.Image = After.Image;
		Now.Page = After.Page;
		ASSERT(Now.Image == After.Image);
		ASSERT(Now.Page == After.Page);
		ASSERT(Now.Run == false);

		//次 のデータの先読みを開始する.
		After.Image = NULL;
		ThreadRun(inPage + 1 , &After);

		//同じポインタを指してはいけない. double free してしまう.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//でも、 NULL なら、ぶつかってもしかたないか.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//同じページを指しはずがない.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//要求されたページが 前 のページ
	else if (Before.Page == inPage)
	{
		//前のデータの読み込みが実は終わっていない場合は終わるまで待つ.
		ThreadJoin(&Before);
		//次のデータの読み込みが終わっていない場合中断させる.
		ThreadAbort(&After);

		//次 のデータを破棄.
		delete After.Image;
		After.Image = NULL;

		//今 のデータを 次 のデータとして記録.
		After.Image = Now.Image;
		After.Page = Now.Page;
		ASSERT(Now.Image == After.Image);
		ASSERT(Now.Page == After.Page);
		ASSERT(After.Run == false);

		//前 のデータを 今 のデータとして記録
		Now.Image = Before.Image;
		Now.Page = Before.Page;
		ASSERT(Now.Image == Before.Image);
		ASSERT(Now.Page == Before.Page);
		ASSERT(Now.Run == false);

		//前 のデータの先読みを開始する.
		Before.Image = NULL;
		ThreadRun(inPage - 1 , &Before);

		//同じポインタを指してはいけない. double free してしまう.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//でも、 NULL なら、ぶつかってもしかたないか.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//同じページを指しはずがない.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}
	//前 にも 次 にも入っていない画像
	else 
	{
		//次のデータの読み込みが終わっていない場合中断させる.
		ThreadAbort(&After);
		//前のデータの読み込みが終わっていない場合中断させる.
		ThreadAbort(&Before);
		ASSERT(After.Run == false);
		ASSERT(Before.Run == false);

		//今のデータを破棄.
		delete Now.Image;
		Now.Image = NULL;
		//今のデータをロード
		ThreadRun(inPage  , &Now );

		//前のデータを破棄、後ろのデータも破棄. 
		delete Before.Image;
		Before.Image = NULL;
		delete After.Image;
		After.Image = NULL;

		//前と次のデータの先読みを開始する.
		After.Image = NULL;
		ThreadRun(inPage + 1 , &After  );
		Before.Image = NULL;
		ThreadRun(inPage - 1 , &Before );

		//今のデータの読み込みが終わるまで待つ.
		ThreadJoin(&Now);

		//同じポインタを指してはいけない. double free してしまう.
		ASSERT(Now.Image != After.Image);
		ASSERT(Now.Image != Before.Image);
		//でも、 NULL なら、ぶつかってもしかたないか.
		ASSERT((After.Image == NULL && Before.Image == NULL) || (After.Image != Before.Image));
		//同じページを指しはずがない.
		ASSERT(Now.Page != After.Page);
		ASSERT(Now.Page != Before.Page);
		ASSERT((After.Page == -1 && Before.Page == -1) || (After.Page != Before.Page) );
	}

	//今の画像を読み込み中はありえない.
	ASSERT(Now.Run == false);
	ASSERT(Now.Image != NULL);
	ASSERT(Now.Thread.IsAlive() == false);


	return Now.Image;
}


//すべてのスレッドを停止させる.
void MVThreadManager::AllThreadDestory()
{
	//今の画像を読み込み中はありえない.
	ASSERT(Now.Run == false);

	//次のデータの読み込みが終わっていない場合中断させる.
	ThreadAbort(&After);
	//前のデータの読み込みが終わっていない場合中断させる.
	ThreadAbort(&Before);
	ASSERT(After.Run == false);
	ASSERT(Before.Run == false);

	delete Now.Image;
	Now.Image = NULL;
	Now.Page = -1;

	delete After.Image;
	After.Image = NULL;
	After.Page = -1;

	delete Before.Image;
	Before.Image = NULL;
	Before.Page = -1;
}


/**
 * ChangeOption:		設定の変更
 *
 * @param inImageConvertParam	変換に使用するパラメータ
 * @return void 
 */
void MVThreadManager::ChangeOption(const MVImagePreConvert* inImageConvertParam )
{
	bool threadReload = true ;
	if ( this->ImageConvertParam.getScrollType() != inImageConvertParam->getScrollType() )
	{
		//読み直し必要.
	}
	else if (	this->ImageConvertParam.getJustFit() == false && 
				this->ImageConvertParam.getJustFit() == inImageConvertParam->getJustFit() )
	{
		//もともとジャストフィットしないで、状態が変わらない
		threadReload = false;
	}
	else if (this->ImageConvertParam.getJustFit() == true && 
			 this->ImageConvertParam.getJustFit() == inImageConvertParam->getJustFit()  )
	{
		switch( this->ImageConvertParam.getScrollType() )
		{
		case MVConfig::YOKO:
			if ( this->ImageConvertParam.getScreenHeight() == inImageConvertParam->getScreenHeight() )
			{
				//もともとジャストフィットする だけど、高さが変わらない
				threadReload = false;
			}
			break;
		case MVConfig::TATE:
			if ( this->ImageConvertParam.getScreenWidth() == inImageConvertParam->getScreenWidth() )
			{
				//もともとジャストフィットする だけど、高さが変わらない
				threadReload = false;
			}
			break;
		}
	}

	//スレッドを読み直し必要があれば読み直す..
	if (threadReload)
	{
		//すべてのスレッドを停止させる.
		AllThreadDestory();
	}

	this->ImageConvertParam = *inImageConvertParam;
}


