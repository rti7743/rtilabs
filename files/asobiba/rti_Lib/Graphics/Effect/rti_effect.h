#include "StartWithExeFile.h"
#ifdef EFFECT_LIFE

#ifndef ___EFFECTH
#define ___EFFECTH

#include <list>

class TEffectObject;

//エフェクト管理
class TEffect
{
    bool all_breaker; //すべて破棄するときにたつ
    std::list<TEffectObject*> EffectList;
public:
    void Make(TEffectObject *My);
    void Destroy(TEffectObject *My) ;
    void AllDestroy();
    TEffect();
    ~TEffect();
};
extern TEffect Effect;

////////////////////////////////
//エフェクト
// すべてのエフェクトはこれを継承しなくてはなりません.
////////////////////////////////
class TEffectObject
{
protected:
    void EffectMake(){Effect.Make(this); };
    void EffectDestroy(){Effect.Destroy(this); };
public:
    TEffectObject(){};
    ~TEffectObject(){};
    virtual void VEffectDestroy() = 0;
};

#endif

#endif //LIFE END
