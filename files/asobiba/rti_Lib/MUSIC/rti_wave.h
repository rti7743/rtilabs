#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

#ifndef ___RTI_WAVE
#define ___RTI_WAVE
#include <windows.h>
#include <list>
#pragma warning(disable: 4786)

struct WAVE_DATA_TABLE
{
    char *wv;       //実際のデータ
    DWORD FSize;    //それのサイズ
};

typedef WAVE_DATA_TABLE* WVD;

class TWaveLib
{
private:
    std::list<WVD> WvdList;
public:
    TWaveLib();
    ~TWaveLib();
    WVD Make(char *filename);
    void All_Breaker();
    void Breaker(WVD puf);
};

extern TWaveLib *WaveChar;


#endif


#endif //LIFE END
