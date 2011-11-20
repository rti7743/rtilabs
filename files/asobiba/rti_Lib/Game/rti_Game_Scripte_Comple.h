#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_COMPLE
#define ___SCRIPTE_COMPLE

#include <.\Game\rti_Game_Scripte_Run.h>
#include <.\Game\rti_Game_Scripte_RevPorland.h>
#include <.\Game\rti_Game_Scripte_String.h>
#include <stack>

class TScripteComple
{
protected:
    //コンパイル命令発効
    void NewCommand(int Nim,int O1 = 0,int O2 = 0 ,int O3 = 0)
    {
    	struct Action A;
	    A.Nim = Nim;
    	A.O1 = O1;
	    A.O2 = O2;
	    A.O3 = O3;
        MAction->push_back(A);
    };
    ACTION_VECTOR *MAction;   //コンパイラが吐き出す命令コード
    USE_STRING    *UseString; //文字列

    TScripteString      ScripteString;
    TScripteRevPorland  ScripteRevPorland;
    typedef std::stack<int> COMPLE_ANA_STACK;     //解析用のスタック
    COMPLE_ANA_STACK        Comple_Ana_Stack;        //解析用のスタック
    char **FunktionStr;                             //ユーザ定義のファンクションの名前(文字列)
    char *CompleFilename;                           //現在コンパイルしているファイル名
    int  FileLen;                                   //ファイルの行数

    //コンパイルのためのルーチン.
    bool CompleDefine(char *next_str,ACTION_VECTOR *Ac);//#DEFINE文の解釈.
    bool CompleElse(char *next_str);                    //#ELSE文の解釈.
    bool CompleEndIf(char *next_str);                   //#ENDIF文の解釈.
    bool CompleEndWhile(char *next_str);                //#ENDIF文の解釈.
    bool CompleBreak(char *next_str);                   //#BREAK文の解釈.
    bool CompleUserFunktion(char *buf);                 //ユーザ定義文の解釈.
    virtual bool UserOperation(char *str){return false;};      //ユーザー定義
    bool FunktionSearch(char *buf);                            //ユーザ指定のファンクションサーチ
    int  FunctionParaCals(char *str);
    bool AnalysisString(char* str);                            //次の文字列を計算式　及び、数式のどちらかで展開.
	void Saitekika(ACTION_VECTOR *Ac);						   //最適化.

public:
    TScripteComple(){FunktionStr = NULL;};
    //そのファイルをコンパイルしてデータを格納
    bool Comple(char *filename,ACTION_VECTOR *Ac,USE_STRING *Us);
    //ファンクション登録.
    void SetFunktion(char **Funk){ FunktionStr = Funk;};
};

#endif

#endif //LIFE END
