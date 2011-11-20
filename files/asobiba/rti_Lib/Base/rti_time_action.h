#include <StartWithExeFile.h>
#ifdef TIMEACTIONH

#ifndef ___TIMEACTIONH
#define ___TIMEACTIONH

/*******************************************************************/
//時限爆弾
/*******************************************************************/
#include <list>

typedef void (*TimerActionStructEvent)(unsigned int Tag);

struct TimerActionStruct
{
    bool alive;
    int Timer; //これが 0 になったら指定されたイベント実行.
    TimerActionStructEvent BomeEvent;    //時間で実行されるイベント
    TimerActionStructEvent EveEvent;     //毎回実行されるイベント
    unsigned int BomeEventTag;
    unsigned int EveEventTag;
};
#define TIMER_ACTION_MESSAGE_MAX 20

class TTimerAction
{
    TimerActionStruct TASTable[TIMER_ACTION_MESSAGE_MAX];
    //空きを探す
    TimerActionStruct* SearchEmpty();
public:
    void Add(TimerActionStruct *TAS);
    void Add(int timer,TimerActionStructEvent BomeEvent,
        TimerActionStructEvent EveEvent,
        unsigned int BomeEventTag,unsigned int EveEventTag);
    void Del(TimerActionStruct *TAS);
    void Action();
};

extern TTimerAction TimeAction;

#endif

#endif //LIFE END
