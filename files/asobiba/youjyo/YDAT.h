// YDAT.h: YDAT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_)
#define AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"
#include "RSpinLock.h"
#include "RStreamUtil.h"
#include "YHttpdConf.h"
#include "RMmap.h"

typedef unsigned long DATNUMBER;
/*
readme.html:Step2
<B>レスストはいくつなんですか?</B>
	チャットch に敬意を表して 5000 となっています。
	これを変更する場合 YDAT.h の MAX_NUMBER を書き換えてください。

	ただし、MAXいったスレッドに対して削除依頼などがあるといけないので、
	システム側の隠しエリアとして + 100しています。
	つまり、 5100 が本当のレスストとなります。

	これ以上は、どうあがこうと書き込めません。

<B>スレ容量制限はいくつですか?</B>
	スレ容量制限は、800kb となっています。
	チャットch の DAT をみると 5000 書き込みで 540kb ぐらいなので、十分に絶えうると思われます。
	よって、 AA 職人やきり版ゲッターは遠慮なく AA を貼り付けることができます。
*/

//最大書き込み数.
const int MAX_NUMBER	= 5000 ;
//実際の限界値.
const int MAX_NUMBER_FULL = MAX_NUMBER + 100;
//警告を出す数.
const int WARNING_NUMBER	= ((int)(MAX_NUMBER * 0.85)) ;


//DATの最大サイズ.
const int MAX_DAT_SIZE	=	800 * 1024;	//800k
//警告を出すサイズ
const int WARNING_DAT_SIZE	=	((int)(MAX_DAT_SIZE * 0.85));



class YDAT  
{
public:
	/**
	 * YDAT:	コンストラクタ
	 *
	 * @param inDatNumber		DATの名前
	 * @return  
	 */
	YDAT::YDAT(DATNUMBER	inDatNumber);

	/**
	 * ~YDAT:		デストラクタ
	 *
	 * @return  
	 */
	YDAT::~YDAT();

	/**
	 * Load:		INDEX,DATファイルの読み込み.
	 *
	 * @return void 
	 */
	void YDAT::Load(const string inBoardName) throw(RIOCannotOpenException)
	{
		RAutoReadSpinLock al(&Lock) ;

		IndexLoad(inBoardName);
		DATLoad(inBoardName);
	}
	/**
	 * Save:		INDEX,DATファイルの書き込み.
	 *
	 * @return void 
	 */
	void YDAT::Save(const string inBoardName) const throw(RIOCannotWriteException)  
	{
		RAutoWriteSpinLock al(&Lock) ;

		IndexSave(inBoardName);
		DATSave(inBoardName);
	}
	/**
	 * ReadAt:			inStartNo ～ inEndNo まで読み込む
	 *
	 * @param outRPack	読み込んだ内容が保存されるバッファ.
	 * @param inStartNo 開始地点
	 * @param inEndNo	終了地点
	 * @return void	
	 */
	bool YDAT::ReadAt(RPack * outRPack,unsigned int inStartNo,unsigned int inEndNo) const  ;
	/**
	 * ReadAtGoEND:		inStartNo ～ ALL まで読み込む
	 *
	 * @param outRPack	読み込んだ内容が保存されるバッファ.
	 * @param inStartNo 開始地点
	 * @return void 
	 */
	bool YDAT::ReadAtGoEND(RPack * outRPack,unsigned int inStartNo) const  ;

	/**
	 * ReadByte:		inStartByte ～ inEndByte まで読み込む
	 *
	 * @param outRPack		読み込んだ内容が保存されるバッファ.
	 * @param inStartByte	開始バイト
	 * @param inEndByte		終了バイト
	 * @return bool 
	 */
	bool YDAT::ReadByte(RPack * outRPack,DWORD inStartByte,DWORD inEndByte) const  ;
	/**
	 * ReadByteGoEND:		inStartByte ～ ALL まで読み込む
	 *
	 * @param outRPack		読み込んだ内容が保存されるバッファ.
	 * @param inStartByte	開始バイト
	 * @return bool 
	 */
	bool YDAT::ReadByteGoEND(RPack * outRPack,int inStartByte) const  ;

	/**
	 * Append:				書き込み
	 *
	 * @param	inRPack		書き込むデータ
	 */
	void Append(const RPack * inRPack) throw(RIOCannotWriteException);
	/**
	 * Append:				書き込み(Admin 理論限界値位置まで)
	 *
	 * @param	inRPack		書き込むデータ
	 */
	void AppendAdmin(const RPack * inRPack) throw(RIOCannotWriteException);
	/**
	 * Overwrite:				オーバーライド
	 *
	 * @param inDeleteNo 
	 * @param inOverwrite 
	 * @return void 
	 */
	void Overwrite(int inDeleteNo , const RPack * inOverwrite) throw(RIOCannotWriteException);

	/**
	 * setSubject:			Subject のセット
	 *						新規書き込みのときに呼ぶべし.
	 *
	 * @param inSubject		追加するサブジェクト
	 * @return void 
	 */
	void setSubject(const string inSubject)
	{
		this->Subject = inSubject;
	}

	//最後の書き込みナンバーの取得.
	int getLastNo() const
	{
		return this->No;
	}

	time_t getLastModified() const
	{
		return this->LastModified;
	}
	string getSubject() const
	{
		return this->Subject;
	}
	DATNUMBER getDatNumber() const
	{
		return this->DatNumber;
	}
	int getDatSize() const
	{
		return this->Index[No];
	}

	// aaaaaaa (10) 形式で取得する
	string getSubjectAndLastNo() const
	{
		return this->getSubject() + " (" + num2str(this->getLastNo() ) + ")" ;
	}

	static void test();
private:
	/**
	 * IndexLoad:		INDEXファイルの読み込み.
	 *
	 * @return void 
	 */
	void YDAT::IndexLoad(const string inBoardName);
	/**
	 * DATLoad:		DATファイルの読み込み.
	 *
	 * @return void 
	 */
	void YDAT::DATLoad(const string inBoardName);
	/**
	 * IndexLoad:		INDEXファイルの書き込み
	 *
	 * @throw		RIOCannotOpenException	ファイルけません.
	 * @return void 
	 */
	void YDAT::IndexSave(const string inBoardName) const  throw(RIOCannotOpenException);
	/**
	 * DATLoad:		DATファイルの書き込み
	 *
	 * @throw		RIOCannotOpenException	ファイルけません.
	 * @return void 
	 */
	void YDAT::DATSave(const string inBoardName) const throw(RIOCannotOpenException);

private:
	//DATのNumber .
	DATNUMBER	DatNumber;
	//現在の書き込み数
	int					No;
	//この番号の書き込みが何バイト目にロードされたか求めるテーブル.
//	DWORD				Index[MAX_NUMBER_FULL];
	DWORD				*Index;
	RMmap				IndexMM;

	//実際の書き込み.
//	char				Dat[MAX_DAT_SIZE];
	char				*Dat;
	RMmap				DatMM;
	//最終書き込み時間.
	DATNUMBER			LastModified;
	//サブジェクト
	string				Subject;


	//ロック
	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YDAT_H__E3A11A8A_9DFB_435B_8B06_A8763A67D50A__INCLUDED_)
