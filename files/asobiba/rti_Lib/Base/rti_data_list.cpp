
#include <stdio.h>
#include <string.h>
#include <.\Base\rti_data_list.h>

//サーチ
int TCommandList::search(char *str,char **NextChar)
{
    //コマンド解析
    for(int i = 0; CL[i] != NULL ; i++)
    {
        //コマンド検索
        char *p = strstr(str,CL[i]);
        if (p)
        {   //見つかった!!
            *NextChar += (strlen(CL[i])+1) ;  //コマンドの次の文字に移動
//            *NextChar += CL[i].Len ;  //コマンドの次の文字に移動
            return i;
        }
    }
    //見つからない.
    return -1;
}

