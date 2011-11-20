// RTaskLS.cpp: RTaskLS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"
#include "RTaskLS.h"
#include "RTaskManager.h"
#include "Filer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTaskLS::RTaskLS(Rfiles* inNewFiles ,string inMask , Display* inDisp , Window* inCallback) : RTask(inNewFiles,inDisp,inCallback)
{
	Mask = inMask;
}

RTaskLS::~RTaskLS()
{

}


//ls実行開始.
void RTaskLS::Execute( )
{
	Filer::getFiler()->getTaskView()->InsertTask( (unsigned long)this , getMessage());

	Files->Inc();
	bool ErrorFlg = false;
	try
	{
		if ( Mask.empty() )		Files->ls(  );
		else					Files->ls( Mask );
	}
	catch(RException e)
	{//ls 中にエラー
		ErrorFlg = true;
	}

	Files->Dec();
	Filer::getFiler()->getTaskView()->DeleteTask( (unsigned long)this );

	//コールバック.
	if (ErrorFlg)
		this->ErrorCallBack();
	else
		this->CallBack();
	

}

//進捗報告 開始0 ----> 完全100 
int RTaskLS::Parsent()
{
	return 0;
}

//タスクビューに表示するメッセージ
string RTaskLS::getMessage()
{
	ASSERT(Files != NULL);

	return "一覧取得 "+ Files->getPath();
}

