// SolvablePathLocal.cpp: SolvablePathLocal クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathLocal.h"
#include <ctype.h>
#include "RConv.h"
#include "MultiString.h"
#include "SolvablePathSpecialFolder.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvablePathLocal::DoSolvable(string inPath) 
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

	//ドライブレターは最低1文字必要.
	if (inPath.size() < 1)	return false;

	//ドライブレターは a-z A-Z
	if ( ! isalpha (inPath[0]) ) return false;

	//c: の : の部分ある?
	if ( inPath[1] != ':' ) 
	{
		if (inPath.size() != 1) return false;	//ぼつ.

		// c 単体だったら c: と補完
		inPath += ":";
	}
	//最後が \\ でない場合は最後に \\ を補完します。
	if ( MultiString::IsPathToComplement(inPath) )
	{
		inPath += "\\";
	}
	this->setPath(inPath);

	//OK
	return true;
}

//一つディレクトリを上がる
string SolvablePathLocal::UpDirectory() const
{
	//パスは c:\ が最低で 最後はかならず \ がはいるということ
	//これを利用します。

	string path = getPath();
	if (path.length() <= 3)
	{//ドライブ一覧へ
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_DRIVES);
	}
	const int lastpos = MultiString::getLastDirectoryPos(path);
	string ret = path.substr( 0 , lastpos + 1) ;
	return ret;
}

void SolvablePathLocal::Test()
{
	SolvablePathLocal sp;

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:\\") );
	ASSERT( sp.getPath() == "c:\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:") );
	ASSERT( sp.getPath() == "c:\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c") );
	ASSERT( sp.getPath() == "c:\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:\\GAME\\いつものところ") );
	ASSERT( sp.getPath() == "c:\\GAME\\いつものところ\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:\\GAME\\いつものところ\\") );
	ASSERT( sp.getPath() == "c:\\GAME\\いつものところ\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:\\GAME\\いつものところ\\kanoso") );
	ASSERT( sp.getPath() == "c:\\GAME\\いつものところ\\kanoso\\" );

	//パスが解決できるか?
	ASSERT( sp.DoSolvable("c:\\ソ\\表\\ルソ\\ルンルン") );
	ASSERT( sp.getPath() == "c:\\ソ\\表\\ルソ\\ルンルン\\" );

	ASSERT( sp.DoSolvable("file:///c:/") );
	ASSERT( sp.getPath() == "c:\\" );

	ASSERT( sp.DoSolvable("file:///C:/Documents%20and%20Settings/Administrator/デスクトップ/") );
	ASSERT( sp.getPath() == "C:\\Documents and Settings\\Administrator\\デスクトップ\\" );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("http://www.yahoo.co.jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("ftp://www.yahoo.co.jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("://www.yahoo.co.jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("//www.yahoo.co.jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("www.yahoo.co.jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable(".jp") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("デスクトップ") );

	//パスが解決に失敗するか?
	ASSERT( ! sp.DoSolvable("ネットワークコンピュータ") );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\Windows") );
	ASSERT( sp.getPath() == "c:\\Windows\\" );
	ASSERT( sp.UpDirectory() == "c:\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\GAME\\いつものところ\\kanoso") );
	ASSERT( sp.getPath() == "c:\\GAME\\いつものところ\\kanoso\\" );
	ASSERT( sp.UpDirectory() == "c:\\GAME\\いつものところ\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\GAME\\いつものところ") );
	ASSERT( sp.getPath() == "c:\\GAME\\いつものところ\\" );
	ASSERT( sp.UpDirectory() == "c:\\GAME\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\ソ\\表\\ルソ\\ルンルン") );
	ASSERT( sp.getPath() == "c:\\ソ\\表\\ルソ\\ルンルン\\" );
	ASSERT( sp.UpDirectory() == "c:\\ソ\\表\\ルソ\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\ソ\\表\\ルソ") );
	ASSERT( sp.getPath() == "c:\\ソ\\表\\ルソ\\" );
	ASSERT( sp.UpDirectory() == "c:\\ソ\\表\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\ソ\\表\\") );
	ASSERT( sp.getPath() == "c:\\ソ\\表\\" );
	ASSERT( sp.UpDirectory() == "c:\\ソ\\" );

	//上に上がる
	ASSERT( sp.DoSolvable("c:\\ソ") );
	ASSERT( sp.getPath() == "c:\\ソ\\" );
	ASSERT( sp.UpDirectory() == "c:\\" );
}


