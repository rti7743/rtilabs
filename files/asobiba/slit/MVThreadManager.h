// MVThreadManager.h: MVThreadManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_)
#define AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "MVReaderThread.h"
#include "MVFileList.h"



class MVThreadManager  
{
	struct MVThreadManagerImagCache
	{
		bool			Run;		//仕事を依頼している?
		MVReaderThread	Thread;		//先読みスレッド

		MVPageData*		Image;
		int				Page;		//担当しているページ
	};

public:
	MVThreadManager();
	virtual ~MVThreadManager();

	/**
	 * Update:				新しい本を開く
	 *
	 * @param inFilename	ファイル名
	 * @return void
	 */
	void Update(const string & inFilename) throw(RException);

	/**
	 * GotoPage:			指定されたページに移動
	 *
	 * @param inPage				ページ番号
	 * @return const MVPageData*	ページのデータ.
	 *
	 */
	const MVPageData* GotoPage(int inPage) throw(RException);

	/**
	 * ChangeOption:		設定の変更
	 *
	 * @param inImageConvertParam	変換に使用するパラメータ
	 * @return void 
	 */
	void ChangeOption(const MVImagePreConvert* inImageConvertParam );


	int getPageCount() const
	{
		return FileList.getCount();
	}
private:

	//すべてのスレッドを停止させる.
	void MVThreadManager::AllThreadDestory();
	//スレッドの開始する.
	void MVThreadManager::ThreadRun(int inPage , MVThreadManagerImagCache* ioThread);
	//データの読み込みが終わっていない場合中断させる.
	void MVThreadManager::ThreadAbort(MVThreadManagerImagCache* ioThread);
	//データの読み込みが実は終わっていない場合は終わるまで待つ.
	void MVThreadManager::ThreadJoin(MVThreadManagerImagCache* ioThread);

private:
	MVThreadManagerImagCache	Before;
	MVThreadManagerImagCache	Now;
	MVThreadManagerImagCache	After;


	MVFileList			FileList;

	MVImagePreConvert	ImageConvertParam;

};

#endif // !defined(AFX_MVTHREADMANAGER_H__6EC6EC28_1482_4A64_8D96_ECDC4F12A433__INCLUDED_)
