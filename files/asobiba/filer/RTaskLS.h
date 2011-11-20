// RTaskLS.h: RTaskLS クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_)
#define AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RTask  ;


#include "comm.h"


#include "RTask.h"
#include "Rfiles.h"

class RTaskLS : public RTask  
{
public:
	RTaskLS(Rfiles* inNewFiles,string inMask, Display* inDisp , Window* inCallback);
	virtual ~RTaskLS();

	//進捗報告 開始0 ----> 完全100 
	virtual int Parsent();
	//タスクビューに表示するメッセージ
	virtual string getMessage();

private:
	//ls実行開始.
	virtual void Execute();

private:
	//検索結果をマスク.
	string Mask;
};

#endif // !defined(AFX_RTASKLS_H__F9EBFBBD_9A33_48DD_A4E3_B6E6B275BF00__INCLUDED_)
