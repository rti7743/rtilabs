#include <StartWithExeFile.h>
#ifdef TIMEACTIONH

#include <windows.h>
#include <.\Base\rti_time_action.h>
/*******************************************************************/
//時限爆弾
/*******************************************************************/

TTimerAction TimeAction;

/*******************************************************************/
//イベント追加(構造体バージョン)
/*******************************************************************/
void TTimerAction::Add(TimerActionStruct *TAS)
{
    TimerActionStruct *LpTAS = SearchEmpty();
    if (LpTAS)
    {
        CopyMemory(LpTAS,TAS,sizeof(TimerActionStruct) );
        LpTAS->alive = true;
    }
}

/*******************************************************************/
//イベント追加(引数バージョン)
/*******************************************************************/
void TTimerAction::Add(int timer,TimerActionStructEvent BomeEvent,
        TimerActionStructEvent EveEvent,
        unsigned int BomeEventTag,unsigned int EveEventTag)
{
    TimerActionStruct *LpTAS = SearchEmpty();
    if (LpTAS)
    {
        LpTAS->alive = true;
        LpTAS->Timer = timer;
        LpTAS->BomeEvent = BomeEvent;
        LpTAS->EveEvent = EveEvent;
        LpTAS->BomeEventTag = BomeEventTag;
        LpTAS->EveEventTag = EveEventTag;
    }
}

/*******************************************************************/
//内部関数　空きを探す
/*******************************************************************/
TimerActionStruct* TTimerAction::SearchEmpty()
{
    TimerActionStruct *LpTAS = &TASTable[0];
    for(int i = 0; i < TIMER_ACTION_MESSAGE_MAX ; i++)
    {
        if (!LpTAS->alive) return LpTAS;
        LpTAS ++;
    }
    return NULL;
}

/*******************************************************************/
//イベント削除
/*******************************************************************/
void TTimerAction::Del(TimerActionStruct *TAS)
{
    TAS->alive = false;
}

/*******************************************************************/
//イベントを実行させる上で必要なヤツ
/*******************************************************************/
void TTimerAction::Action()
{
    TimerActionStruct *LpTAS = &TASTable[0];
    for(int i = 0; i < TIMER_ACTION_MESSAGE_MAX ; i++)
    {
        if (LpTAS->alive)
        {
            LpTAS->Timer --;
            //毎回実行
            if (LpTAS->EveEvent)  LpTAS->EveEvent(LpTAS->EveEventTag);
            //時間は大丈夫 ?
            if (LpTAS->Timer <= 0)
            {   //ボンバー
                if (LpTAS->BomeEvent)
                    LpTAS->BomeEvent(LpTAS->BomeEventTag);
                LpTAS->alive = false;
            }
        }
        LpTAS ++;
    }
}


#endif  //LIFE END
