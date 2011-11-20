#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_RUN
#define ___SCRIPTE_RUN

//STL 最高～
#include <stack>
#include <vector>
#include <string>
#include <memory.h>
//長い変数名などへの警告を黙らせる. STLを使うとでるアレです.
#pragma warning(disable: 4786)

//スクリプト言語　中間言語　の　命令書式. 8086 汗　に近いです (^^;
struct Action
{
    unsigned char Nim;
    int O1;
    int O2;
    int O3;

    friend bool operator == (const Action& A,const Action& B){return A.Nim == A.Nim;};
    friend bool operator <  (const Action& A,const Action& B){return A.Nim < A.Nim;};
};

//スクリプト中間言語の命令.
enum Operation
{
    E_MOV,E_MOVI,E_MOVII,E_ADD,E_ADDI,E_ADDII,E_SUB,E_SUBI,E_SUBII,
    E_MUL,E_MULI,E_MULII,E_DIV,E_DIVI,E_DIVII,E_MOD,E_MODI,E_MODII,
    E_SHL,E_SHLI,E_SHLII,E_SHR,E_SHRI,E_SHRII,E_AND,E_ANDI,E_ANDII,
    E_OR,E_ORI,E_ORII,   E_XOR,E_XORI,E_XORII,
    E_A,E_AI,E_AII,E_B,E_BI,E_BII,
    E_AE,E_AEI,E_AEII,E_BE,E_BEI,E_BEII,
    E_NE,E_NEI,E_NEII,E_E,E_EI,E_EII,
    E_NOT,E_NOTI,E_NOTII,
    E_PUSH,E_PUSHI,E_POP,E_POPI,
    E_JMP,E_IF,E_HAL,E_FUNK,
};
typedef std::vector<Action> ACTION_VECTOR;
typedef std::stack<int> ACTION_STACK;
typedef std::vector<std::string>    USE_STRING;

class TScripeRunArea;

typedef int (*USER_FUNKTION)(TScripeRunArea *SC,int o1,int o2,int o3,int o4,int o5);
#define USER_FUNKTION_MAX 5    //ユーザが定義できる関数の引数の最大値.
//特別レジスタ.
#define REG_S      (5)
#define REG_S_EX   (6)
#define ACTION_MAX  256

//Run の戻り値について.
#define RET_RESERVED_WORD_RUN   0   //予約語を実行した,
#define RET_END_WORD_RUN   -1       //終了コード、及び命令を最後まで実行した.
#define RET_DISHONEST_WORD_RUN   -10//不正な命令を実行しました. リセットしてください (ぉ

class TScripteComple;
class TScripteRun
{
    //スクリプトデータ.
    ACTION_VECTOR MAction[ACTION_MAX];
    //文字列データ
    USE_STRING UseString;

    bool FunktionSearch(char *buf);
    int  FunctionParaCals(char *str);
	//UserFunk の保存
	void UserFunKeep(USER_FUNKTION *Funk,char **StrCommand);
    //ユーザ定義の命令
    virtual int UserOperation(Action *A,TScripeRunArea *SRA){return RET_DISHONEST_WORD_RUN;};
    USER_FUNKTION *UserFunk;
public:
	TScripteRun() : UserFunk(NULL) {};
	~TScripteRun(){delete UserFunk;};
    //そのファイルをコンパイルしてデータを格納
    bool Comple(char *filename,USER_FUNKTION *Funk = NULL,char **Str = NULL);
    //そのファイルをコンパイルしてデータを格納
    bool Comple(char *filename,TScripteComple *SC,
                USER_FUNKTION *Funk = NULL,char **Str = NULL);
    //削除.
    void Breaker(int c){MAction[c].clear();};
    //すべてクリア
    void All_Breaker(){for(int i = 0 ; i < ACTION_MAX ;i ++ )Breaker(i);};
    //Run
    int __fastcall Run(int c,TScripeRunArea *SRA);
    //マッピングされた場所から、文字列を求める
    const char* GetString(int MappingNo){return (UseString[MappingNo]).c_str();};
    //マッピングされた場所から、文字列を求める
    std::string GetStlString(int MappingNo){return (UseString[MappingNo]);};
    //Debug
    void Debug(int c);
};

//スクリプト実行エリア.
class TScripeRunArea
{
    //TScripteRun クラス(命令デコーダだよん)から、このクラスのレジスタ、
    //スタックに自由にアクセスができるようにします. (^^;;;;
    friend class TScripteRun;
protected:
    int IP;                     //現在実行しているアドレス(?)
    int R[7];                   //レジスタ
	int RunNumber;			    //実行するスクリプト
    ACTION_STACK ActionStack;   //スタック
public:
    TScripeRunArea(){};
    void ClearIP(){IP = 0;};
    void ClearR(){memset(R,0,sizeof(int)*7);};
    void ClearStack(){/*ActionStack.clear();*/};
    void ClearAll(){ClearIP(); ClearR(); ClearStack(); };
	//新規命令実行開始.
	void NewScripteRun(int No){ ClearAll(); RunNumber = No; };
};

#endif
#endif //LIFE END

