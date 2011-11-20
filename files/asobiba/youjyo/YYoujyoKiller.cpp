// YYoujyoKiller.cpp: YYoujyoKiller クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoKiller.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YYoujyoKiller::YYoujyoKiller()
{

}

YYoujyoKiller::~YYoujyoKiller()
{

}

/**
 * Run:	実行.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoKiller::Run()
{
	while( this->IsHoldUp() )
	{
		YBBSManager::getInstance()->KillerTask(Buffer);
		/*ChangeLog.txt:2003/01/26	YoujyoKillerの終了処理に時間がかかっていたので修正
		*/
		//一分に一回動きます。
		this->Sleep(1000 * 60 * 1);
	}
	return 0;
}

/**
 * Shutdown:	停止命令送信.
 *
 */
void YYoujyoKiller::Shutdown()
{
	this->HoldUp();
}
