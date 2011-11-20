#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <stdio.h>
#include <.\Game\rti_Game_Scripte_Comple.h>
#include <.\Base\rti_data_list.h>
#include <.\Base\rti_error_message.h>

#define COMPLE_MAX_BUFFER_SIZE 256
#define COMPLE_ERROR_OUT(INPORMATION)    (MsgError3("CompleError File %s (%dLines)\n %s",CompleFilename,FileLen,INPORMATION))

/***********************************************************/
//そのファイルをコンパイルしてデータを格納
/***********************************************************/
bool TScripteComple::Comple(char *filename,ACTION_VECTOR *Ac,USE_STRING *Us)
{
    FILE *fp;
    char buf[COMPLE_MAX_BUFFER_SIZE];
    //予約語.
    char *CL[] =
    {
        "#*",           //コメント  工事現場で使うのは、セメント ... (ﾂﾏﾗｰ)
        "#DEFINE",      //定義
        "#CALS",        //計算
        "#IF",          //IF
        "#ELSE",        //ELSE
        "#ENDIF",       //ENDIF
        "#WHILE",       //WHILE
        "#ENDWHILE",    //ENDWHILE
        "#SWITCH",      //SWITCH
        "#ENDSWITCH",   //ENDSWITCH
        "#CASE",        //CASE
        "#BREAK",       //BREAK
        "#END",         //END
        NULL,
    };
    TCommandList TCL;   //検索エンジン (^^;

    char *next_str;     //予約語の次の文字列.
    //予約語を検索対象に承認する.
    TCL.set(CL);
    //文字列をマッピングするところ.
    UseString = Us;
    //ファイル名をいただいておく.
    CompleFilename = filename;
    //ファイルの行数を初期化
    FileLen = 0;

    fp = fopen(filename,"rb");
    if (!fp)
    {
        FileNotOpen("TScripteComple::Comple",filename);
        return false;
    }
    try
    {
        while (! feof(fp) )
        {
            FileLen++;  //ファイル行数をカウントします.
            fgets(buf,COMPLE_MAX_BUFFER_SIZE,fp);
            next_str = buf;
            switch (TCL.search(buf,&next_str) )
            {
            case 0: //コメント
                break;
            case 1: //定義.
                if ( !CompleDefine(next_str,Ac) ) throw 0;
                break;
            case 2: //計算
                if ( !AnalysisString(next_str) )  throw 0;//その文字列を解析する
                break;
            case 3: //IF
                if ( !AnalysisString(next_str) )  throw 0;//その文字列を解析する
                Comple_Ana_Stack.push( MAction->size() );   //その IF のあった場所をスタックに登録.
                NewCommand(E_IF);   //分岐命令発効.
                break;
            case 4: //ELSE
                if ( !CompleElse(next_str) ) throw 0;
                break;
            case 5://ENDIF
                //スタックより、先の if , else の位置を取得.
                if ( !CompleEndIf(next_str) ) throw 0;
                break;
            case 6://WHILE
                if (!AnalysisString(next_str) )  throw 0;//その文字列を解析する
                Comple_Ana_Stack.push( MAction->size() );   //その WHILE のあった場所をスタックに登録.
                NewCommand(E_IF);   //分岐命令発効.
                break;
            case 7://ENDWHILE
                if ( !CompleEndWhile(next_str) ) throw 0;
                break;
            case 8://SWITCH
                break;
            case 9://ENDSWITCH
                break;
            case 10://CASE
                break;
            case 11://BREAK
                if ( !CompleBreak(next_str) ) throw 0;
                break;
            case 12://END
                NewCommand(E_HAL);   //END 終了命令
                break;
            default:    //ユーザ定義文 ?
                if ( !CompleUserFunktion(buf)) throw 0;
                break;
            }
        }
    }
    catch(...)
    {   //途中で異常発生!!
        fclose(fp);
        return false;
    }

    fclose(fp);

	//最適化.
	Saitekika(Ac);

    return true;
}

/***********************************************************/
//#DEFINE文の解釈.
/***********************************************************/
bool TScripteComple::CompleDefine(char *next_str,ACTION_VECTOR *Ac)
{
    int Def;
    //#DEFINE の値の取得.
    sscanf(next_str,"%d",&Def);
    if (Def >= ACTION_MAX) 
    {
        COMPLE_ERROR_OUT("#DEFINE の限界を超えた定義でげす.");
        return false;
    }
    //今回コンパイルしていく場所を示す. MAction は、private メンバ
    MAction = & Ac[Def];
    return true;
}

/***********************************************************/
//#ELSE文の解釈.
/***********************************************************/
bool TScripteComple::CompleElse(char *next_str)
{
    //スタックより、先の if の位置を取得.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //念のため確認.
    if ( (*MAction)[w].Nim != E_IF)
    {
        COMPLE_ERROR_OUT("#ELSE と #IF が対応してまへん.");
        return false;
    }

    //不成立の時は、自分の次の命令に.
    (*MAction)[w].O1 = MAction->size() + 1;
    Comple_Ana_Stack.push( MAction->size() );   //その ELSE があった位置を記録
    NewCommand(E_JMP);   //ELSE の時の分岐命令.

    return true;
}

/***********************************************************/
//#ENDIF文の解釈.
/***********************************************************/
bool TScripteComple::CompleEndIf(char *next_str)
{
    //スタックより、先の if , else の位置を取得.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //念のため確認.
    if (! ((*MAction)[w].Nim == E_IF || (*MAction)[w].Nim == E_JMP) )
    {
        COMPLE_ERROR_OUT("#ENDIF が いきなり呼ばれて出てきてズドドドーンです.");
        return false;
    }
    //不成立の時は、自分の次の命令に.
    (*MAction)[w].O1 = MAction->size() ;
    return true;
}

/***********************************************************/
//#ENDIF文の解釈.
/***********************************************************/
bool TScripteComple::CompleEndWhile(char *next_str)
{
    //スタックより、先の while の位置を取得.
    int w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    //BREAK の 念のため確認.
    if ((*MAction)[w].Nim == E_JMP)
    {   //BREAK!
        (*MAction)[w].O1 = MAction->size() ;
        //これは、 WHILE
        w = Comple_Ana_Stack.top(); Comple_Ana_Stack.pop();
    }
    //念のため確認.
    if ((*MAction)[w].Nim == E_IF)
    {   //不成立の時は、自分の次の命令に.
        (*MAction)[w].O1 = MAction->size() + 1;
        NewCommand(E_JMP,w);   //ENDWHILE の時に WHILE までジャンプ
    }
    else
    {
        COMPLE_ERROR_OUT("#ENDWHILE が 不正に出現しました.");
        return false;
    }
    return true;
}

/***********************************************************/
//#BREAK文の解釈.
/***********************************************************/
bool TScripteComple::CompleBreak(char *next_str)
{
    //スタックより、先の while の位置を取得.
    int w = Comple_Ana_Stack.top(); //取得するだけってことに注意.
    //念のため確認.
    if ((*MAction)[w].Nim == E_IF)
    {
        Comple_Ana_Stack.push( MAction->size() );   //その BREAK のあった場所をスタックに登録.
        NewCommand(E_JMP);   //BREAK の時の分岐命令.
    }
    else
    {
        COMPLE_ERROR_OUT("#BREAK が 不正に出現したにょー.");
        return false;
    }
    return true;
}
/***********************************************************/
//ユーザ定義文の解釈.
/***********************************************************/
bool TScripteComple::CompleUserFunktion(char *buf)
{
    if ( !UserOperation(buf) )   //ユーザが定義したニモニック ?
    {
        if (! FunktionSearch(buf) )    //ユーザが定義した関数 ? 
        {
            COMPLE_ERROR_OUT("解析不能だにょー.");
            return false;
        }
    }
    return true;
}

/***********************************************************/
//ユーザ定義のファンクションを検索、展開します.
/***********************************************************/
bool TScripteComple::FunktionSearch(char *buf)
{
    TCommandList TCL;
    int ope,w;
    char *next_str;

    if ( ! FunktionStr) return false;

    TCL.set(FunktionStr);
    next_str = buf;
    //ユーザ命令を選択.
    ope =  TCL.search(buf,&next_str) ;
    if (ope < 0)    return false; //解読不能

    //解読できた!
    w = FunctionParaCals(next_str); //パラメタを展開.
    NewCommand(E_FUNK,ope,w);       //ユーザ定義ファンクション呼び出し.
    return true;    //ok.
}

/***********************************************************/
//ユーザ定義のファンクションのパラメータの展開.
/***********************************************************/
int TScripteComple::FunctionParaCals(char *str)
{
    char buf[COMPLE_MAX_BUFFER_SIZE];
    int ParaNum = 0;
    int buf_counter = 0;
    for( ; *str ; str ++)
    {
        switch(*str)
        {
        case ',':
            buf[buf_counter] = 0;  //その文の修理用.
            AnalysisString(buf);   //その文字列を解析する
            NewCommand(E_PUSH,REG_S);       //S レジスタをスタックに積む.
            buf_counter = 0;
            ParaNum ++;
            break;
        default:
            buf[buf_counter++] = *str;
            break;
        }
    }
    if (buf_counter > 0)
    {
        buf[buf_counter] = 0;  //その文の修理用.
        AnalysisString(buf);   //その文字列を解析する
        NewCommand(E_PUSH,REG_S);       //S レジスタをスタックに積む.
    }
    return ParaNum;
}

/***********************************************************/
//次の文字列を計算式　及び、数式のどちらかで展開.
/***********************************************************/
bool TScripteComple::AnalysisString(char* str)
{
    //文字列の証拠 '"' を探す.
    int Cnt = 0;
    char *StrChk = str;
    for( ; *StrChk ; StrChk++) if (*StrChk == '"') Cnt ++;

    if (Cnt > 0)
    {   //文字列.
        NewCommand(E_MOVI,REG_S,
            ScripteString.StringMapping(str,UseString) );       //S レジスタにマッピングした場所を返す.
    }
    else
    {   //数式
        ScripteRevPorland.RevPorland( str , *MAction);
    }
    return true;
}


/***********************************************************/
//最適化.
/***********************************************************/
void TScripteComple::Saitekika(ACTION_VECTOR *Ac)
{
	int ActionNo ;
	int OpeNo ;
	int NextOpNo;
	int MaxActiones;
	Action* lpTempAction;
	Action* lpNextTempAction;
	ACTION_VECTOR *NowAction;
	//基本的な最適化.
	//	MOV  REG_S,1		PUSH REG_S		=> PUSH  1
	//	MOVI REG_S,1		PUSH REG_S		=> PUSHI 1

	for(ActionNo = 0 ; ActionNo < ACTION_MAX ; ActionNo++)
	{
		NowAction = & Ac[ActionNo];
		MaxActiones = (*NowAction).size() - 1;	//ループ回数
		//各命令の内容を吟味します.
		for(OpeNo = 0 ; OpeNo < MaxActiones ; OpeNo ++)
		{
			lpTempAction = &(*NowAction)[OpeNo];
			if ( (lpTempAction->Nim == E_MOV || lpTempAction->Nim == E_MOVI) && (lpTempAction->O1 == REG_S) )
			{
				NextOpNo = OpeNo+1;
				if (NextOpNo >= MaxActiones) break; //次の命令はないのですねぇ ふへっ
				//次の命令を取得.
				lpNextTempAction = &(*NowAction)[NextOpNo];
				//次が、 PUSH REG_S だったら.最適化する.
				if (lpNextTempAction->Nim == E_PUSH && lpNextTempAction->O1 == REG_S)
				{
					//lpNextTempActionのところを 最適化したコードに改造します.
					if (lpTempAction->Nim == E_MOV)		lpNextTempAction->Nim = E_PUSH;
					else								lpNextTempAction->Nim = E_PUSHI;
					lpNextTempAction->O1 = lpTempAction->O2;

					(*NowAction).erase(lpTempAction);
					MaxActiones--;
				}
			}	// E_MOV E_MOVI の判定の IF
		}		// 
	}
}

#endif //LIFE END
