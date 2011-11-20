// RHTTPHeader.h: RHTTPHeader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_)
#define AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHTTPHeaderAttribute.h"
#include "RHTTPHeaderAttributeRange.h"
#include "RHTTPHeaderAttributeDate.h"
#include "RRemoteHost.h"

class RHTTPHeader  
{
public:
	RHTTPHeader();
	virtual ~RHTTPHeader();

	/**
	 * ReadBuffer:				バッファから読み込みヘッダとして解析します。
	 *
	 * @param inHeaderBuffer	解析するバッファ
	 * @param inHeaderSize		バッファサイズ
	 * @return void 
	 */
	void ReadBuffer(char * ioHeaderBuffer , int inHeaderSize);

	void setRemoteHost(in_addr inAddr)
	{
		RemoteHost.setIP(inAddr);
	}

	//メソッドのゲット
	const string getMethod()const	{		return Method;	}
	//パスのゲット.
	const string getPath()	const	{		return Path;	}

	//コンテントレングス
	RHTTPHeaderAttribute<int>	ContentLength;
	//Range
	RHTTPHeaderAttribute<RHTTPHeaderAttributeRange>	Range;
	//If-Modified-Since
	RHTTPHeaderAttribute<RHTTPHeaderAttributeDate>	IfModifiedSince;

	//リモートホスト	ヘッダと関係ないけどとりあえず(w
	RRemoteHost					RemoteHost;

	static void test();

private:
/**
 * FirstScan:				最初の一行をスキャン
 *
 * @param isFirst			一番最初のヘッダ?
 * @param inLine			一行
 * @return void 
 */
void RHTTPHeader::FirstScan(const char* inLine);
/**
 * CommandScan:				最初の一行以外をスキャン
 *
 * @param inLine			一行
 * @return void 
 */
void RHTTPHeader::CommandScan(const char* inLine);

private:
	//メソッド名
	string	Method;
	//リクエストパス
	string	Path;
};

#endif // !defined(AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_)
