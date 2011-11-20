// YThreadWithTls.h: YThreadWithTls クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_)
#define AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class YThreadWithTls;
#include "RThread.h"
#include "RThreadPool.h"
#include "RRequest.h"
#include "RWin32Exception.h"

const int TLS_POOL_MAX		=	10 * 1024;	//10kB

class YThreadWithTls : public RThread  
{
public:
	YThreadWithTls(RThreadPool<YThreadWithTls>* inPool) throw(RWin32Exception);
	virtual ~YThreadWithTls();

	
	/**
	 * Run:		実行
	 *
	 * @return virtual unsigned int			常に 0
	 */
	virtual unsigned int Run();
	/**
	 * Bind:		仕事を登録する.
	 *
	 * @param inReq		新しい仕事
	 * @return void 
	 */
	void	Bind(RRequest* inReq)
	{
		Request = inReq;
	}

	char* getTLS() const
	{
		return (char*) Tls;
	}

private:
	//スレッドプール本体.
	RThreadPool<YThreadWithTls>*	OwnerPool;
	//リクエスト.
	RRequest*		Request;
	//TLS領域
	char	Tls[TLS_POOL_MAX];

};

#endif // !defined(AFX_YTHREADWITHTLS_H__81DF2087_C62D_4D5C_B9A0_220E4936B537__INCLUDED_)
