// Window.cpp: Window クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Window.h"
#include "RTaskManager.h"
#include "SolvablePathManager.h"
#include "SolvableFileManager.h"
#include "DisplayFactory.h"
#include "MultiString.h"
#include "RTaskLS.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Window::Window(FilerSub* inFilerSub,string inPath)
{
	Files = NULL;
	ParentFSub = inFilerSub;

	//Blankページ.
	Disp = DisplayFactory::New(__DISPLAYTYPE_BLANK, this ,__RFILETYPE_NONE_DUMMY);
	if ( ! this->Preview(inPath , true) )
	{
		this->PreviewOrLs(inPath , true);
	}
}

Window::~Window()
{
	if (this->Files) 
	{
		ASSERT( this->Files->Dec() );
	}

	if (Disp) Disp->Dec();
}


//アクティヴになりました.
void Window::OnActive()
{
	if (Disp)		
	{
		//アクティヴ
		Disp->OnActive();
	}
}

//非アクティヴになりました.
void Window::OnNoActive() const
{
	if (Disp)		Disp->OnNoActive();
}

//ディスプレイウィンドウの幅と高さを取得
void Window::GetWindowRect(RECT * outRC) const
{
	Disp->GetWindowRect(outRC);
}


//アップデートします。
void Window::UpdateWindow(Rfiles* inFiles , Display* inDisp)
{
	//もし、ディスプレイが更新される場合...
	if (inDisp)
	{
		//現在保持しているディスプレイと違うことを確認して
		if (inDisp != this->Disp)
		{
			//現在ディスプレイ破棄.
			if (Disp) Disp->Dec();
			//変更.
			this->Disp = inDisp;
		}
	}

	//もし、更新するファイルシステムが有効ならば、更新する.
	if (inFiles)
	{
		//現在保持しているファイルシステムと違うことを確認して
		if (inFiles != this->Files)
		{
			//現在保持しているファイルシステムへの参照カウントを減らす。
			if (this->Files) Files->Dec();
			//変更.
			this->Files = inFiles;
		}
	}

	if (this->Files && this->Disp)
	{
		//ディスプレイのデータの更新.
		const RTable* rt = this->Files->getTable();
		this->Disp->setRfileItems( rt->getAllSize() , rt->getAllList() );

		//現在アクティヴだったら、念のため Displayを更新しておく。
		if ( ParentFSub->IsActiveWindow(this) )			ParentFSub->setActiveWindow(this);
	}
}

//ディレクトリを一つ上がる.
void Window::UpDirectory()
{
	if (Files)
	{
		//パスを解決できるオブジェクトの作成.
		SolvablePathManager* spm = SolvablePathManager::getInstance();
		SolvablePath* tws = spm->DoSolvable( getNowPath() );
		if (!tws)	return ;

		this->PreviewOrLs( tws->UpDirectory() );
	}
}


//左にあんの?
bool Window::getIsLeft() const
{
	return ParentFSub->getIsLeft();
}

//戻れ!
void Window::HistoryBack()
{
	try
	{	//Scan を使うと履歴の残ってしまうので、
		PreviewOrLs( History.Back()  );
	}
	catch(RNotFoundException e)
	{
		//戻れませんでした
		printf("error : %s\n" , e.getMessage() );
	}
}

//進め!
void Window::HistoryNext()
{
	try
	{	//Scan を使うと履歴の残ってしまうので、
		PreviewOrLs( History.Next() );
	}
	catch(RNotFoundException e)
	{
		//進めませんでした
		printf("error : %s\n" , e.getMessage() );
	}
}



//ディレクトリスキャン.
bool Window::ls(string inPath,bool isNOTRecodeHistory) 
{
	Rfiles* files = NULL;
	Display* disp = Disp;
	try
	{
		//パスを解決できるオブジェクトの作成.
		SolvablePathManager* spm = SolvablePathManager::getInstance();
		SolvablePath* tws = spm->DoSolvable(inPath);
		if (tws == NULL) 
			throw RNotSupportException("パス %s は解決できませんでした" ,inPath.c_str() );
		//現在表示しているパターンと違う場合はほぼディスプレイの作り直しが必要です。
		if ( Disp->getNowNavigateType() != tws->getType())
		{
			if ( !tws->IsDisplaySupported( Disp->getType() ))
			{//サポートされていないので作り直しです。
				//ディスプレイ作成.
				disp = DisplayFactory::New( tws->WhatIsDisplayeRecommendedMost() , this , tws->getType() );
				//カラムの登録.
				disp->setFixedCloumn( tws->LetKnowColumnesInDisplayRecommendedMost() );
			}
		}

		//実際にファイルシステムを操作する Rfiles を作成.
		files = tws->RfilesFactory( disp->getType() );
		try
		{
			//ソート機能の作成.
			files->MakeTable(__FILECOLUMN_TYPE_EXT , true );
		}
		catch(RNotSupportException e)
		{	//失敗したら、名前でソート.
			files->MakeTable(__FILECOLUMN_TYPE_NAME , true );
		}

		//タスクマネージャーに登録.
		RTaskManager * rtm = RTaskManager::getInstance();
		//	void ls(Rfiles* inFiles ,Display* inDisp,  Window* inCallback);
		rtm->putTask( new RTaskLS(files ,Mask , disp , this) );
	}
	catch(RException e)
	{
		//新規につくったんなら消す.
		if (Disp != disp )
		{
			if (disp)	disp->Dec();
		}
		//新しいファイル一覧の消去
		if (files)	files->Dec();

		//そのパスは解決できません。
		printf("error : %s\n" , e.getMessage() );
		return false;
	}
	//ヒストリーに追加.
	if (!isNOTRecodeHistory) History.Append(inPath);

	return true;
}

//プレビュー
bool Window::Preview(string inPath,bool isNOTRecodeHistory)
{
	Display* disp = Disp;
	Rfiles* files = NULL;
	try
	{
		//パスを解決できるオブジェクトの作成.
		SolvableFileManager* spm = SolvableFileManager::getInstance();
		SolvableFile* tws = spm->DoSolvable(inPath);
		if (tws == NULL) 
			throw RNotSupportException("パス %s は解決できませんでした" ,inPath.c_str() );
		//現在表示しているパターンと違う場合はほぼディスプレイの作り直しが必要です。
		if ( Disp->getNowNavigateType() != tws->getType())
		{
			if ( !tws->IsDisplaySupported( Disp->getType() ))
			{//サポートされていないので作り直しです。
				//ディスプレイ作成.
				disp = DisplayFactory::New( tws->WhatIsDisplayeRecommendedMost() , this , tws->getType() );
				//カラムの登録.
				disp->setFixedCloumn( tws->LetKnowColumnesInDisplayRecommendedMost() );
			}
		}
		//実際にファイルシステムを操作する Rfiles を作成.
		files = tws->RfilesFactory( disp->getType() );
		try
		{
			//ソート機能の作成.
			files->MakeTable(__FILECOLUMN_TYPE_EXT , true );
		}
		catch(RNotSupportException e)
		{	//失敗したら、名前でソート.
			files->MakeTable(__FILECOLUMN_TYPE_NAME , true );
		}

		//タスクマネージャーに登録.
		RTaskManager * rtm = RTaskManager::getInstance();
		//	void ls(Rfiles* inFiles ,Display* inDisp,  Window* inCallback);
		rtm->putTask( new RTaskLS(files ,Mask , disp , this) );


	}
	catch(RException e)
	{
		//新規につくったんなら消す.
		if (Disp != disp ) 	
		{
			if (disp) disp->Dec();
		}
		//新しいファイル一覧の消去
		if (files) files->Dec();

		//そのパスは解決できません。
		printf("error : %s\n" , e.getMessage() );
		return false;
	}
	//ヒストリーに追加.
	if (!isNOTRecodeHistory) History.Append(inPath);
	return true;
}


//現在のディスプレイ上でのパスが変更になりました。
//たとえばブラウザでリンクをクリックされたとき、
//現在のディスプレイを使ってパスとヒストリーだけ記録します。
void Window::OnQuickChange(string inPath,bool isNOTRecodeHistory )
{
	ASSERT(this->Files);

	this->Files->setPath(inPath);

	//親に OnQuickChange がおきたことを通知し、
	//アドレスバーを更新する.
	ParentFSub->OnQuickChange();

	//ヒストリーに追加.
	if (!isNOTRecodeHistory) History.Append(inPath);
}


//タイトルの取得.
string Window::getTitle() const
{
	string title;
	if (Disp)	title = Disp->getTitle();
	
	//取得できないばあいは、最後のディレクトリにする.
	if (title.empty() )
	{
		title = getNowPath();
		int pos = MultiString::getLastDirectoryPos( title.c_str() );

		if (title[pos] == '\\') pos ++;
		title = title.substr( pos , title.size() - pos);
		if ( !MultiString::IsPathToComplement(title.c_str() ) )
		{	//最後の \\ を削除します。
			if ( title.size() <= 1) return "";	// \\\\ と連続したらやばいので回避.
			return title.substr(0 , title.size() - 1);
		}
	}
	return title;
}

//タイトルが変更された場合
void Window::OnTitleChange()
{
	//親に OnTitleChange がおきたことを通知する
	ParentFSub->OnTitleChange();
}


//自分が所有しているタスクをすべてとめます。
void Window::StopTasks() const
{
	//タスクマネージャーに登録.
	RTaskManager * rtm = RTaskManager::getInstance();
	rtm->stopTasks(this);
}

