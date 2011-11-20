#include "comm.h"
#include "RTableMuchDuplication.h"
#include "Owners.h"
#include "RfilesLocal.h"
#include "RfilesPreviewOne.h"
#include "SolvablePathLocal.h"
#include "RSyncFileCopy.h"
#include "RTableEasy.h"
#include "AddressHistory.h"
#include "RConv.h"
#include "RfilesLzh.h"
#include "RfilesWithCache.h"
#include "VirtualPath.h"
#include "WindowsMain.h"
#include "RExpr.h"




int main()
{


	puts("Welcome to the SuperFiler!! yah!");

	int ret = -1;
//	try
	{
/*
		//仮想パス.
		VirtualPath::test();
		//重複の多いソート
		RTableMuchDuplication<string>::test();
		//単純なソート
		RTableEasy::test();
		//ファイルの所有者リスト
		Owners::test();
		//ローカルからのファイルの取得
		RfilesLocal::test();
		//単一ファイル
		RfilesPreviewOne::test();
		//LZHからゲット.
		RfilesLzh::test();
		//キャッシュテスト
		RfilesWithCache::test();
		//ヒストリー関係
		AddressHistory::test();
		//パス解決.
		SolvablePathLocal::Test();
		//コピーテスト.
		RSyncFileCopy::Test();
		//文字列変換ルーチン
		RConv::test();
		//数式分解
		RExprToken::test();
		//計算.
		RExpr::test();
*/
		//Windowsの実行環境
		ret = WindowsMain::getMainSystem()->Run();
	}
	/*
	catch(RException e)
	{
		TRACE("原因不明のエラーが発生しました\n「%s」\n", e.getMessage() );	
		return -100;
	}
	catch(...)
	{
		TRACE("システムまたは C++ の例外が投げられました");
		return -200;
	}
	*/
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return ret;
}

