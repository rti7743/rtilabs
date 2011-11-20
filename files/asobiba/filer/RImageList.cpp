// RImageList.cpp: RImageList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImageList.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RImageList::RImageList()
{
	this->ImageList = NULL;
}

RImageList::~RImageList()
{
	Destroy();
}

//イメージリストの作成
void RImageList::Create(int inWidth,int inHegith,int inInitCount,int inMax)  throw()
{
	if (this->ImageList) throw RDoNotInitaliseException("すでにイメージリストが作成されています");

	this->ImageList = ImageList_Create(inWidth , inHegith ,ILC_COLORDDB ,  inInitCount , inMax);

	if (this->ImageList == NULL) throw RDoNotInitaliseException("イメージリストが作成できません");

//	TRACE("イメージリストを作成しました %p \n" , this->ImageList);
}

//イメージリストの破棄
void RImageList::Destroy()
{
//	TRACE("イメージリストを破棄しました \n" );
	ImageList_Destroy(this->ImageList);
	this->ImageList = NULL;
}


//追加
int RImageList::Add(HICON inIcon)
{
	if ( RemoveList.empty())
	{//空き領域がないので普通に追加.
//		TRACE("ImageList_ReplaceIcon 新規 %d \n" , ImageList_GetImageCount(this->ImageList ) - 1 );
		return ImageList_AddIcon( this->ImageList , inIcon);
	}
	else
	{//空き領域を消化.
		const int index = this->RemoveList.back();
		this->RemoveList.pop_back();

//		TRACE("ImageList_ReplaceIcon 再利用 %d / %d %p\n" , index , ImageList_GetImageCount(this->ImageList) ,this->ImageList);

		return ImageList_ReplaceIcon( this->ImageList ,index , inIcon);
	}
}

//指定した index を削除
//勝手につめたりしないので、ママも安心です。
void RImageList::Remove(int inIndex)
{
	int count = ImageList_GetImageCount(this->ImageList ) - 1;
	if ( count == inIndex)
	{//最後を消す場合はつめる.
		ImageList_Remove(this->ImageList , inIndex);
		TRACE("ImageList_Remove %d つめました\n",inIndex);
		count--;
		//その前があいていたらさらに消す.
		for( ; count >= 0 ; count--)
		{
			REMOVELIST::iterator it = 
				find(this->RemoveList.begin() , this->RemoveList.end() , count);
			if (it == this->RemoveList.end())
			{//見つからない
				break;
			}
			//みつかっので削除.
			ImageList_Remove(this->ImageList , count);
			this->RemoveList.erase(it);
//			TRACE("ImageList_Remove %d さらに、つめました\n",count);
		}
	}
	else
	{//最後でない場合は、削除マークを立てる.
		this->RemoveList.push_back(inIndex);
	}
}

//全部消す.
void RImageList::RemoveAll()
{
	this->RemoveList.clear();
	ImageList_RemoveAll(this->ImageList);
}

