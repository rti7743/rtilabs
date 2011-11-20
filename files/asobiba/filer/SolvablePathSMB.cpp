// SolvablePathSMB.cpp: SolvablePathSMB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSMB.h"
#include "RConv.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvablePathSMB::DoSolvable(string inPath) 
{
	//先頭が file:/// だった場合、その部分をカット.
	if ( inPath.substr(0,8) == "file:///" )
	{
		inPath = inPath.substr(8);
		// / を \\ にする.
		replace( inPath.begin() , inPath.end() , '/','\\');
		//UrlEncode を取り除く.
		inPath = RConv::UrlEncode(inPath);
	}

	//先頭が \\ でなければダメ
	if (inPath.substr(0,2) != "\\\\")
	{
		return false;
	}

	//最後が \\ でない場合は最後に \\ を補完します。
	if ( MultiString::IsPathToComplement(inPath) )
	{
		inPath += "\\";
	}

	//パスの深さが 2以上あることが必須   substr は 先頭の \\ を除くため.
	if ( MultiString::setPathDepth(inPath.substr(2) ) < 2)
	{
		return false;
	}

	this->setPath(inPath);

	//OK
	return true;
}

//一つディレクトリを上がる
string SolvablePathSMB::UpDirectory() const
{
	const string path = getPath() ;
	const int lastpos = MultiString::getLastDirectoryPos( path );
	return path.substr( 0 , lastpos + 1) ;
}

void SolvablePathSMB::Test()
{
	SolvablePathSMB sp;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


