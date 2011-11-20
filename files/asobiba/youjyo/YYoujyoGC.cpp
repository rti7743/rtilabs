// YYoujyoGC.cpp: YYoujyoGC クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoGC.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YYoujyoGC::YYoujyoGC()
{

}

YYoujyoGC::~YYoujyoGC()
{

}

/**
 * Run:	実行.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoGC::Run()
{
	while( this->IsHoldUp() )
	{
		YBBSManager::getInstance()->GCTask();
		/*ChangeLog.txt:2003/01/26	YYoujyoGCの終了処理に時間がかかっていたので修正
		*/
		//1.5分に一回動きます。
		this->Sleep(1000 * 90 * 1);
	}
	return 0;
}

/**
 * Shutdown:	停止命令送信.
 *
 */
void YYoujyoGC::Shutdown()
{
	this->HoldUp();
}

