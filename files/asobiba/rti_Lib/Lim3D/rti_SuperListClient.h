#ifndef __TSUPER_LIST_CLIENT
#define __TSUPER_LIST_CLIENT

//クライアントデータの元.



#include "rti_SuperListClientSrc.h"

//#ifndef __TSUPER_LIST_SERVER
//	class __TSuperListServer;
//#endif
#include "rti_SuperList.h"

class __TSuperListClient;
class __TSuperListClient : public __TSuperListClientSrc 
{
public:
	//コンストラクタ
	__TSuperListClient()
	{
		m_Parence = NULL;
	}
	//デストラクタ
	virtual ~__TSuperListClient(){}
public:
	//破棄.(virtual はオブジェクト思考的には ○ , 速度的には ×)
	virtual void Release()
	{
		if (m_Parence)
			((__TSuperListServer<__TSuperListClient>*)m_Parence)->Del(this);
		delete this;
	}
	//自分を登録.
	void Mount(void* inP)
	{
		m_Parence = inP;
	}
protected:
	//自分の親.
	void* m_Parence;
};


#include "rti_SuperList.h"

#endif
