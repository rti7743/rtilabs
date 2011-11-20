// YSubject.h: YSubject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_)
#define AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YDAT.h"
#include "RSpinLock.h"
#include "RStreamUtil.h"
#include "RHtmlTemplate.h"
#include "YTextFile.h"
#include "YHttpdConf.h"
/*
readme.html:Step4
<B>subject.txtに何件まで登録できますか?</B>
	300件まで登録できます。
	それ以降のものは、過去ログ倉庫に送られます。

	これを書き換えるには、 YSubject.h を書き換えてください。
*/

const int MAX_SUBJECT	=	300;
const int MAX_SUBJECT_LINE	=	1024;

class YNiseDAT
{
public:
	YNiseDAT(DATNUMBER inNumber , string inSubject )
	{
		ASSERT(inNumber != 0);
		this->Number = inNumber;
		this->Subject = inSubject;
	}
	YNiseDAT()
	{
	}

	void SubjectChange(string inSubject)
	{
		this->Subject = inSubject;
	}

	string CompateLine() const
	{
		ASSERT(this->Number != 0);
		ASSERT(num2str(this->Number) != "");

		return num2str(this->Number) + ".dat<>" + this->Subject + "\n";
	}

	string getSubject() const 
	{
		return this->Subject + "\n";
	}

	DATNUMBER getDatNumber() const
	{
		return this->Number;
	}
private:
	DATNUMBER	Number;
	string		Subject;
};


class YSubject  : public YTextFile
{
public:

	YSubject();
	virtual ~YSubject();

	void Load(const string inBoardName);
	void Save(const string inBoardName);
	void Age(const YDAT* inDAT);
	void Sage(const YDAT* inDAT);

	/**
	 * Show:				表示			 YTextFileより継承
	 *
	 * @param outRPack		これに格納
	 * @return void 
	 */
	virtual void Show(RPack * outRPack) const;

	int getNumbers(DATNUMBER * outNumbers , int inSize) const;

	int WhatPosition(DATNUMBER inDATNumber) const;
	/**
	 * DrawThreadIndex:			スレッド一覧の描画
	 *
	 * @param inTemplate		描画に使うテンプレート
	 * @param inReadCGIPath		read.cgiのパス
	 * @param inCount			何個描画するか?
	 * @return string			描画した結果	
	 */
	const string DrawThreadIndex(const RHtmlTemplate* inTemplate , const string inReadCGIPath ,  int inCount) const;

	//テスト
	static void test();

private:
	//DAT
	YNiseDAT*	DATs[MAX_SUBJECT];

	//現在保持している DAT 数.
	int			No;

	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_)
