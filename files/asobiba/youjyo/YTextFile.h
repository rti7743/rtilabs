// YTextFile.h: YTextFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_)
#define AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"

class YTextFile  
{
public:
	YTextFile()
	{
		Update();
	}
	virtual ~YTextFile()
	{
	}

	/**
	 * Show:				出力します.
	 *
	 * @param outPack		出力先.
	 * @return virtual void 
	 */
	virtual void Show(RPack * outPack) const = 0;


	/**
	 * getLastModified:		最終更新日を返します。
	 *
	 * @return time_t		最終更新日.
	 */
	time_t getLastModified() const
	{
		return this->LastModified;
	}

protected:
	/**
	 * Update:			内容を更新したとき読んでください.
	 *
	 * @return void 
	 */
	void Update()
	{
		this->LastModified = time(NULL);
	}

private:
	//最終更新時間
	time_t		LastModified;

};

#endif // !defined(AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_)
