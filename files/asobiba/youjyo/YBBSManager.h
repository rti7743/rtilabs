// YBBSManager.h: YBBSManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_)
#define AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YBBS.h"

#include "RNotFoundException.h"
#include "RIOCannotOpenException.h"
#include "YThreadWithTls.h"


class YBBSManager  
{
private:
	YBBSManager();
public:
	virtual ~YBBSManager();


	//読み込み
	void Read(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,
		const YRequestPath* inRequestPath, RStreamUtil * outBuffer ) throw(RNotFoundException,RIOCannotOpenException);
	
	//書き込み
	void Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,
		POST& ioPost, RStreamUtil * outBuffer ) throw(RNotFoundException,RIOCannotOpenException);

	/**
	 * SystemWrite:				システムからのメッセージ
	 *
	 * @param outShareBuffer	TLSバッファ.
	 * @param ioPost			書き込む内容
	 * @return void 
	 */
	void SystemWrite(char* outShareBuffer,POST& ioPost) throw(RNotFoundException,RIOCannotOpenException);
	//削除
	void Aborn(const string inTargetIta ,DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete)
		throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	//削除依頼された書き込みのやばい単語をマスクする.
	void MaskReWrite(const string inTargetIta  ,DATNUMBER inDatNumber ,const string & password ,
					int inDeleteSize ,const int * inDelete ,
					int inMaskSize   ,const string *inMask ) 
		throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	//特定の板の特定のスレッドのタイトルの取得.
	const string getSubject(const string inBoardName , DATNUMBER inDatNumber) throw(RNotFoundException);


	static YBBSManager*	getInstance();

	/////制御用スレッドによる実行
	//削除人タスク.
	void KillerTask(char* outShareBuffer);
	//DATガページコレクタ
	void GCTask();

	/////////////////管理者用命令
	//情報を出力.
	void Dump();
	///すべての情報をディスクにフラッシュする.
	void Flush();
	//BBSを再読み込み
	void ReloadBBS() throw(RException);

private:
	//BBSをすべて破棄する.
	void ClearBBS();
	/**
	 * getBBS:					指定された板に対するBBSを取得.
	 *
	 * @param inTargetIta 
	 * @return YBBS* 
	 */
	YBBS* getBBS(const string &inTargetIta) throw(RNotFoundException);

private:
	typedef map<string,YBBS*>	BBSS;
	BBSS	AllBBS;
};

#endif // !defined(AFX_YBBSMANAGER_H__BBFCC337_97F2_41F1_8741_91006A1E44E8__INCLUDED_)
