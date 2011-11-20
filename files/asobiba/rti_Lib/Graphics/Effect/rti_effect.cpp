#include "StartWithExeFile.h"
#ifdef EFFECT_LIFE
/*****************************************************************/
/*****************************************************************/
//   グラフィックエフェクト管理
/*****************************************************************/
/*****************************************************************/

#pragma warning(disable: 4786)
#include <list>
#include <algorithm>
#include <.\Graphics\Effect\rti_effect.h>
#include <.\Base\rti_debug.h>

TEffect Effect;

/********************************************************/
//コンストラクタ
/********************************************************/
TEffect::TEffect()
{
    all_breaker = false;
}

/********************************************************/
//デストラクタ
/********************************************************/
TEffect::~TEffect()
{
    AllDestroy();
}


/********************************************************/
//エフェクトの作成
/********************************************************/
void TEffect::Make(TEffectObject *My)
{
    EffectList.push_back(My); 
}

/********************************************************/
//エフェクトの破棄
/********************************************************/
void TEffect::Destroy(TEffectObject *My) 
{
    if(all_breaker) return ;

	//リストのつなぎ直し
    std::list<TEffectObject*>::iterator it =
        std::find(EffectList.begin(),EffectList.end(),My);
    if(it != EffectList.end())
    {
            //データの破棄
            EffectList.erase(it);
    }
}

/********************************************************/
//すべてのエフェクトの破棄
/********************************************************/
void TEffect::AllDestroy()
{
    all_breaker = true;
    std::list<TEffectObject*>::iterator it;

    //すべてのリストにアクセスして解放する.
    for(it = EffectList.begin() ; it != EffectList.end() ; it ++)
    {
        (*it)->VEffectDestroy();
    }
    //リストのすべてのデータの破棄
    EffectList.clear();
}

#endif //LIFE END
