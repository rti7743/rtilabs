#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#ifndef ___SCRIPTE_REVPORLAND
#define ___SCRIPTE_REVPORLAND

/***********************************************************/
//テキストで与えられた式の計算
//  及び　スクリプト中間言語　変換.
/***********************************************************/
#include <.\Game\rti_Game_Scripte_Run.h>
#include <stack>

#define KACO_NEST_LEVEL 10  //括弧がネストする度に増える優先度
    //計算式を変換するために使う.  コード (X - * / % など.)
    struct CalsCode
    {
        unsigned char Code;    //演算記号
        int O;                 //優先順位
        friend bool operator == (const CalsCode& A,const CalsCode& B){return A.O == B.O;};
        friend bool operator <  (const CalsCode& A,const CalsCode& B){return A.O < B.O;};
    };
    //計算式を変換するために使う.  数字　レジスタなど.
    struct CalsData
    {
        bool Suji;             //ここが、 true だったら数字だよん.
        int O;                 //データ内容 数字orレジスタナンバー
        friend bool operator == (const CalsData& A,const CalsData& B){return A.O == B.O;};
        friend bool operator <  (const CalsData& A,const CalsData& B){return A.O < B.O;};
    };
class TScripteRevPorland
{
    //計算式の命令.
    enum 
    {
        P_EQU,P_ADD,P_SUB,P_MUL,P_DIV,P_MOD,P_SHL,P_SHR,P_AND,P_OR,P_XOR,
        P_A,P_B,P_AE,P_BE,P_NE,P_E,
        P_NOT,
    };
    //コードレベル.
    unsigned char *CodeLevel; 
    //ネストした事によるレベル
    int Kaco_Next_Add_Level;

    typedef std::stack<CalsCode> CALS_CODE_STACK;     //記号専用スタック
    typedef std::stack<CalsData> CALS_DATA_STACK;     //計算データ専用スタック
    CALS_CODE_STACK        CalsCodeStack;       //計算記号
    CALS_DATA_STACK        CalsDataStack;       //数字もしくはレジスタが入るスタック
    ACTION_VECTOR _MAction;

    //コンパイル命令発効
    void NewCommand(int Nim,int O1 = 0,int O2 = 0,int O3 = 0)
    {
    	struct Action A;
	    A.Nim = Nim;
    	A.O1 = O1;
	    A.O2 = O2;
	    A.O3 = O3;
        _MAction.push_back(A);
    };
    //計算記号をスタックに登録
    void  NewClasCode(unsigned char N,int O);
    //計算データをスタックに登録
    void  NewClasData(bool Suji,int O);
    //計算記号をスタックから取得
    bool  GetCommandCode(struct CalsCode* CC );
    //計算データをスタックから取得
    bool  GetCommandData(struct CalsData *CD);
    //数字に変換.
    int SujiGet(char* &s);
    //数値計算
    void ClasProc(int i1,int Code,int i2);
    //その計算命令コードを実際の命令コードに変換
    unsigned char GetCode(int Op);
    //レジスターが絡む計算.
    void RegisterProc(struct CalsData *A,int Code,struct CalsData *B);
    //計算処理
    bool StackClasProc();
    //演算子などのチェック
    void CodeCheck(int Code);
    //レジスターを取得
    void Register(char* &s);
    //現在のアルゴリズムでは、最後によけいな push が入るのでそれを削除.
    //また、ついでに　無駄なスタック PUSh POP をも削除して最適化を行う (ぷっ
    void PushKiller(int StartMActionNumber);
    //逆ポーランド法のメイン
    void RevPorlandMain(char *Target);
public:
    TScripteRevPorland();
    //逆ポーランド法だよん
    bool RevPorland(char *Target,ACTION_VECTOR &MasterAction);
};

#endif
#endif //LIFE END
