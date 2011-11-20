#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <.\Game\rti_Game_Scripte_String.h>

//純粋な文字列 "    " のもじれの切り出し.
void TScripteString::GetPureString(char *pStr,char* &str)
{
    int Pure = false;
    while(*str)
    {
        if (*str == '"')
        {
            if(!Pure) Pure = true;
            else      
            {
                *pStr = 0;  //ヌル
                break;    //ループ脱出.
            }
        }
        else
        {
            if (Pure) //'"' の中だった収録していく.
            {
                *pStr = *str;
                pStr++;
            }
        }
        str++;
    }
}

//文字列を配列にマッピング
int TScripteString::StringMapping(char* &str,USE_STRING *Us)
{
    char pureString[256];
    int MappingNo;
    //純粋な文字列の切り出し.
    GetPureString(pureString,str);
    //マッピングする場所を取得.
    MappingNo = Us->size();
    Us->push_back( std::string(pureString) );
    //マッピングした場所を返す.
    return MappingNo;
}

/*
//文字列の取得
const char* GetMappingString(int No)
{
    return (UseString[No]).c_str();
}
*/

#endif //LIFE END
