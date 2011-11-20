/**********************************************************/
// VCLの基本
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef OBJECT_LIFE
#include <list>
#include <algorithm>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>

/**********************************************************/
// イベントの初期化をコンストラクタでやる
/**********************************************************/
TObject::TObject()
{
	OwnerUse        = 0;
    OwerKill        = false;
}

/**********************************************************/
//自分が所有しているコンポーネントが増えた
/**********************************************************/
void TObject::OwnerAdd(TObject* hOwner,TObject* hMy)
{
    TObject* hhOwner;

    //オーナーが NULL だったらすべてアプリケーションがひきうける
    if (!hOwner) hhOwner = Application;
    else         hhOwner = hOwner;

    //リストに追加
	hhOwner->ObjectList.push_back(hMy);

    if (!hOwner)    hMy->hMyOwner = NULL;
    else            hMy->hMyOwner = hhOwner;
}

/**********************************************************/
// 自分が所有しているすべてのコンポーネントの破棄
/**********************************************************/
void TObject::ChildDestroy()
{
    std::list<TObject*>::iterator it;

    //すべてのリストにアクセスして解放する.
    for(it = ObjectList.begin() ; it != ObjectList.end() ; it ++)
    {
		ObjectTrancerationAndBreak((*it));
    }
    //リストのすべてのデータの破棄
    ObjectList.clear();
}

/**********************************************************/
// 自分が破棄されたことによる処理
/**********************************************************/
void TObject::IamDestroy()
{
    if (hMyOwner == NULL)   return;
    //自分のオーナーが自分を破壊してきた場合、
    //自分の破棄を殺人鬼であるオーナーにつたえる必要はないだろう.(:-
    if (hMyOwner->OwerKill)
    {
        return;
    }
    
    std::list<TObject*>::iterator it =
            std::find(hMyOwner->ObjectList.begin(),
                      hMyOwner->ObjectList.end(),this);
    if(it != hMyOwner->ObjectList.end())
    {
            ObjectList.erase(it);
    }
}

/**********************************************************/
/* そのオブジェクトの破棄するときのあるべき姿にキャストして殺す*/
/**********************************************************/
void TObject::ObjectTrancerationAndBreak(TObject* hObject)
{
    hObject->UserDestroy();
}



#endif  //LIFE END
