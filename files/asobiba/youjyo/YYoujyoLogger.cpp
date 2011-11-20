// YYoujyoLogger.cpp: YYoujyoLogger クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoLogger.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


YLogNotify* YLogNotify::getInstance()
{
	static YLogNotify ycn;
	return &ycn;
}


YYoujyoLogger::YYoujyoLogger()
{
	LogFile = fopen( "c:/secret.log" , "ab");
}

YYoujyoLogger::~YYoujyoLogger()
{
	if (LogFile)
	{
		fclose(LogFile);
	}
}

/**
 * Run:	実行.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoLogger::Run()
{
	int i = 0;
	while( 1 )
	{
		string *log;
		YLogNotify::getInstance()->Wait(&log);
		if (log == NULL)
		{
			break;	//終了.
		}

		if (LogFile)
		{
			fwrite( log->c_str() , log->size() , sizeof(char) , LogFile );
			if (i == 100)	//100回に一度フラッシュ.
			{
				fflush(LogFile);
				i = 0;
			}
			i++;
		}
		delete log;
	}
	return 0;
}

/**
 * Shutdown:	停止命令送信.
 *
 */
void YYoujyoLogger::Shutdown()
{
	this->HoldUp();
	YLogNotify::getInstance()->Shutdown();
}
