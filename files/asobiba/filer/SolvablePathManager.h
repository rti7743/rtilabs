// SolvablePathManager.h: SolvablePathManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHMANAGER_H__92D5CB9D_1593_4B19_BE25_2A75849C8261__INCLUDED_)
#define AFX_SOLVABLEPATHMANAGER_H__92D5CB9D_1593_4B19_BE25_2A75849C8261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvablePathManager;

#include "comm.h"


#include "SolvablePath.h"
#include "SolvablePathLocal.h"
#include "SolvablePathDebug.h"
#include "SolvablePathWeb.h"
#include "SolvablePathSMB.h"
#include "SolvablePathSMBLogon.h"

class SolvablePathManager  
{
public:
	SolvablePathManager()
	{
		ThoseWhoSolve = new SolvablePathDebug( new SolvablePathLocal( new SolvablePathSMB( new SolvablePathWeb( new SolvablePathSMBLogon ) ) ) );
	}
	virtual ~SolvablePathManager()
	{
		delete ThoseWhoSolve;
	}

public:
	//作成 singleton
	static SolvablePathManager* getInstance()
	{
		static SolvablePathManager spm;
		return &spm;
	}

	//解決してほすぃ.
	//解決できる人が見つかったら、そいつのポインタ
	//誰も解決できなかったら NULL
	SolvablePath*	DoSolvable(string inPath)
	{
		return ThoseWhoSolve->PleaseSolve(inPath);
	}


private:
	SolvablePath*	ThoseWhoSolve;
};

#endif // !defined(AFX_SOLVABLEPATHMANAGER_H__92D5CB9D_1593_4B19_BE25_2A75849C8261__INCLUDED_)
