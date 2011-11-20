#ifndef __TSUPER_LIST_CLIENT_SRC
#define __TSUPER_LIST_CLIENT_SRC

//クライアントデータの元の低レベル層.

class __TSuperListClientSrc
{
public:
	virtual ~__TSuperListClientSrc(){};

	__TSuperListClientSrc* m_Next;
};

#include "rti_SuperListSrc.h"

#endif
