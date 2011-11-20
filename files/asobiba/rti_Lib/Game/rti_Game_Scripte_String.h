#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_STRING
#define ___SCRIPTE_STRING

#include <.\Game\rti_Game_Scripte_Run.h>

class TScripteString
{
    //純粋な文字列 "    " のもじれの切り出し.
    void GetPureString(char *pStr,char* &str);
public:
    TScripteString(){};
    //文字列を配列にマッピング
    int StringMapping(char* &str,USE_STRING *Us);
};

#endif

#endif //LIFE END
