#ifndef ___NON_DUPLICATIONH
#define ___NON_DUPLICATIONH

/*****************************************************************/
//多重起動の防止    アトム版(鉄腕ぢゃないよ(お約束) )
/*****************************************************************/
#include <windows.h>

class TNonDuplication
{
    ATOM Atom;
public:
    TNonDuplication(){ Atom = NULL;};
    ~TNonDuplication();
    //多重起動しているかどうか調べる. している = true
    bool IsDuplication(char *SeriStr);
};

#endif
