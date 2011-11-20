// SolvableFile.h: SolvableFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_)
#define AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvableFile  ;

#include "comm.h"
#include "PathInterface.h"

//ファイル名解決.
class SolvableFile  : public PathInterface
{
public:
	SolvableFile()
	{
	}
	virtual ~SolvableFile()
	{
	}

	//解決してください
	SolvableFile* PleaseSolve(string inPath)
	{
		//自分でできる? できたら俺凄いぢゃん　ってことで自分を帰す。
		if ( this->DoSolvable(inPath) ) return this;
		//なんでぇい、解決できないやん。 もうだめぽ。
		return NULL;
	}

	//一つディレクトリを上がる
	virtual string UpDirectory() const ;

protected:
	//そのパスを解決してみる
	//解決できた場合は解決した結果のパスは メンバに setPath でセットすること.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ などとなる
	//解決できた場合は true 解決できなかった場合は false
	virtual bool DoSolvable(string inPath)  = 0;

};

#endif // !defined(AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_)
