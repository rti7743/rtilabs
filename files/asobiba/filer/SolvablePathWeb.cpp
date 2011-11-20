// SolvablePathWeb.cpp: SolvablePathWeb クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathWeb.h"
#include "SolvablePathSpecialFolder.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////



//そのパスを解決してみる
bool SolvablePathWeb::DoSolvable(string inPath) 
{
	//先頭が http:// であること
	if ( inPath.substr(0,7) != "http://" && inPath.substr(0,7) != "https://" )
	{
		//ドメインぽいっ名前であれば補完する....

		if (inPath.size() <= 3) return false; //こんな短いのはない. a.to より短いのある?

		if (inPath[1] == ':') return false;	//ドライブレター?

		if (inPath.find("://") != -1) return false;	// :// があるってことはなんか別のプロトコル?

		if (inPath.substr(0,2) == "\\\\")	return false;	//これだったら共有.

		if (inPath.find(".") == -1) return false;	// . がないドメインなんてないでしょう.

		//ここまできたらこいつをドメインとして認定.

		inPath = string("http://") + inPath;
	}

	this->setPath(inPath);

	//OK
	return true;
}

//一つディレクトリを上がる
string SolvablePathWeb::UpDirectory() const
{
	const string path = getPath() ;

	int protcolSkip = path.find("://");	//http:// をスキップ.
	if (protcolSkip == -1 ) protcolSkip = 0;	//バグ?
	else					protcolSkip += sizeof("://");

	const int lastpos = MultiString::getLastDirectoryPos( path.substr( protcolSkip ), '/');
	if (lastpos == 0)
	{	//トップらしいので、デスクトップに戻る.
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_DESKTOP);
	}

	return path.substr( 0 , protcolSkip + lastpos + 1) ;
}

void SolvablePathWeb::Test()
{
	SolvablePathWeb sp;

	//パス解決が成功するか.
	ASSERT( sp.DoSolvable("http://www.yahoo.co.jp") );
	ASSERT( sp.getPath() == "http://www.yahoo.co.jp" );

}


