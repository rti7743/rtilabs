// YThreadWithTls.cpp: YThreadWithTls クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YThreadWithTls.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YThreadWithTls::YThreadWithTls(RThreadPool<YThreadWithTls>* inPool) throw(RWin32Exception)
{
	OwnerPool = inPool;
	Request = NULL;
}

YThreadWithTls::~YThreadWithTls()
{
}

/**
 * Run:		実行
 *
 * @return virtual unsigned int			常に 0
 */
unsigned int YThreadWithTls::Run()
{
	while( this->IsHoldUp() )
	{
		if (Request)
		{	//実行
			Request->Execute(this);
			//実行し終わったので破棄.
			delete Request;
			//一応終わったけど、新しい仕事ある?
			Request = OwnerPool->complateRequest(this);
		}
		else
		{	//実行するものないので待ち
			this->Wait();
		}
	}
	return 0;
}

