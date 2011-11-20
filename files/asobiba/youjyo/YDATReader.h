// YDATReader.h: YDATReader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_)
#define AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"

class YDATReader  
{
public:
	YDATReader();
	virtual ~YDATReader();

	static const char* YDATReader::makeHTML(RStringMap* outSmap ,int inNo ,const char* inData );
	static string Subject(const char* inDat,int len);

	static void test();

	//次の <> へ
	static const char* YDATReader::next(const char* str);
	//次の \n へ
	static const char* YDATReader::nextnr(const char* str,const char* end);
	//次の \n へ
	static const char* YDATReader::nextnr(const char* str);


	/**
	 * MaskKakikomi:			書き込みのマスク
	 *
	 * @param inData			一書き込みの dat
	 * @param inMaskSize		マスクの数
	 * @param inMask			マスクデータ
	 * @return string			マスク処理された dat に書き戻せるデータ
	 */
	static string YDATReader::MaskKakikomi(const char* inData , int inMaskSize   ,const string *inMask);

private:
	/**
	 * MaskOneRecord:			一項目ごとのマスク
	 *
	 * @param inData			一書き込み項目
	 * @param inMaskSize		マスクの数
	 * @param inMask			マスクデータ
	 * @return string			マスク処理されたデータ
	 */
	static string YDATReader::MaskOneRecord(const string& inData , int inMaskSize   ,const string *inMask);

	// htmlらしいものがあった場合リンクとします
	static string YDATReader::makeLink(const string &inStr);

};

#endif // !defined(AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_)
