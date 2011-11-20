// SolvablePathSpecialFolder.cpp: SolvablePathSpecialFolder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSpecialFolder.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvablePathSpecialFolder::DoSolvable(string inPath) 
{

	//OK
	return true;
}

//一つディレクトリを上がる
string SolvablePathSpecialFolder::UpDirectory() const
{
	return "";
}

//スペシャルフォルダの名前の取得
//参考 http://www.wac-jp.com/programmers/win32/GetSpecialFolder.shtml
//さらに参考 http://www.kab-studio.com/Programing/Codian/ShellExtension/06.html
string SolvablePathSpecialFolder::getSpecialFolder( int inFolder )
{
	//親フォルダ
	LPSHELLFOLDER	pCurFolder = NULL;
	//フォルダの中のデータ
	LPITEMIDLIST pItemIDList = NULL;
	//ディスプレイに表示される名前 これが戻り値
    string dispName;
	do
	{
		HRESULT	res;
		//ルートフォルダである ディスクトップの取得.
		res = SHGetDesktopFolder( &pCurFolder );
		if (res != NOERROR) break;

		//inFolder でもらった特別なフォルダを　フォルダをあらわすアイテムに変換
		res = ::SHGetSpecialFolderLocation(NULL, inFolder, &pItemIDList);
		if (res != NOERROR) break;
			
		ASSERT(pItemIDList);

		//それらから名前を求める.
		STRRET stFileName;
		res = pCurFolder->GetDisplayNameOf(pItemIDList, SHGDN_NORMAL , &stFileName);
		if (res != NOERROR) break;

		dispName = TranslationFileName(pItemIDList , &stFileName);
	}
	while(0);

	//解放するべー
	LPMALLOC pMalloc = NULL;
	::SHGetMalloc(&pMalloc);

	//フォルダの中のデータの解放
	if (pItemIDList)
	{
		pMalloc->Free( pItemIDList );
	}
	//親フォルダの解放.
	pCurFolder->Release();

	//メモリ操作関係の解放.
	pMalloc->Release();

    return dispName;
}

string SolvablePathSpecialFolder::
		TranslationFileName(LPITEMIDLIST p_pIDlist, LPSTRRET p_pStrret)
{
	LPMALLOC pMalloc = NULL;
	::SHGetMalloc(&pMalloc);
	string cRetStr;

	switch (p_pStrret->uType) 
	{
	case STRRET_WSTR:
		{
			char dest[MAX_PATH];
			if (::WideCharToMultiByte(CP_OEMCP, 0, p_pStrret->pOleStr , -1, dest, MAX_PATH, NULL, NULL) != 0)
			{
				cRetStr = dest;
			}
			pMalloc->Free(p_pStrret->pOleStr);
		}
		break;

	case STRRET_OFFSET:
		break;

	case STRRET_CSTR:
		break;
	}
	//メモリ操作関係の解放.
	pMalloc->Release();

	return cRetStr;
}



void SolvablePathSpecialFolder::Test()
{
}


