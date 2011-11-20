#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <.\Game\rti_Game_Scripte_RevPorland.h>
#include <stdio.h>

/*****************************************************/
//計算記号をスタックに登録
/*****************************************************/
void  TScripteRevPorland::NewClasCode(unsigned char N,int O)
{
	struct CalsCode A;
	A.Code = N;	A.O = O;
    CalsCodeStack.push(A);
}
/*****************************************************/
//計算データをスタックに登録
/*****************************************************/
void  TScripteRevPorland::NewClasData(bool Suji,int O)
{
	struct CalsData A;
	A.Suji = Suji;	A.O = O;
    CalsDataStack.push(A);
}

/*****************************************************/
//計算記号をスタックから取得
/*****************************************************/
bool TScripteRevPorland::GetCommandCode(struct CalsCode* CC )
{
    if (CalsCodeStack.size() <= 0) return false; //スタック空っぽの例外

    *CC = CalsCodeStack.top();    CalsCodeStack.pop();
    return true;
}

/*****************************************************/
//計算データをスタックから取得
/*****************************************************/
bool TScripteRevPorland::GetCommandData(struct CalsData* CD)
{
    if (CalsDataStack.size() <= 0) return false; //スタック空っぽの例外

    *CD = CalsDataStack.top();    CalsDataStack.pop();
    return true;
}
/*****************************************************/
//数字に変換.
/*****************************************************/
int TScripteRevPorland::SujiGet(char* &s)
{
    int ret = 0;            //返り血
    int zero = 1000000;     //とりあえず、百万 (^^; 数字は文字列だと、逆に格納されているのでねっ
    //文字列の長さを取得.
    for(; *s >= '0' && *s <= '9' && zero >=0; s++)
    {
        ret += (*s - '0' ) * zero;
        zero /= 10; //位を一つ下げる.
    }
    // s を一つ戻す(数字の最後の文字 1234 だったら 4 にポインタをあわせる.)
    s --;
   if (zero == 0) return ret; // zero が 0 になるような値だった.
   //多すぎる値のカットとして書えす.
    return ret / (zero*10);
}

/*****************************************************/
//数値計算
/*****************************************************/
void TScripteRevPorland::ClasProc(int i1,int Code,int i2)
{
        switch(Code)
        {
        case P_EQU:
            printf("%d",i2);
            break;
        case P_ADD:
            NewClasData(true,i1 + i2);   break;
        case P_SUB:
            NewClasData(true,i1 - i2);   break;
        case P_MUL:
            NewClasData(true,i1 * i2);   break;
        case P_DIV:
            NewClasData(true,i1 / i2);   break;
        case P_MOD:
            NewClasData(true,i1 % i2);   break;
        case P_SHL:
            NewClasData(true,i1 << i2);   break;
        case P_SHR:
            NewClasData(true,i1 >> i2);   break;
        case P_AND:
            NewClasData(true,i1 & i2);   break;
        case P_OR:
            NewClasData(true,i1 | i2);   break;
        case P_XOR:
            NewClasData(true,i1 ^ i2);   break;
        case P_A:
            NewClasData(true,i1 < i2);   break;
        case P_B:
            NewClasData(true,i1 > i2);   break;
        case P_AE:
            NewClasData(true,i1 <= i2);   break;
        case P_BE:
            NewClasData(true,i1 >= i2);   break;
        case P_NE:
            NewClasData(true,i1 != i2);   break;
        case P_E:
            NewClasData(true,i1 == i2);   break;
        case P_NOT:
            break;
        }
}

/*****************************************************/
//その計算命令コードを実際の命令コードに変換
/*****************************************************/
unsigned char TScripteRevPorland::GetCode(int Op)
{
        switch(Op)
        {
        case P_EQU:            return E_MOV;
        case P_ADD:            return E_ADD;
        case P_SUB:            return E_SUB;
        case P_MUL:            return E_MUL;
        case P_DIV:            return E_DIV;
        case P_MOD:            return E_MOD;
        case P_SHL:            return E_SHL;
        case P_SHR:            return E_SHR;
        case P_AND:            return E_AND;
        case P_OR:             return E_OR;
        case P_XOR:            return E_XOR;
        case P_A:              return E_A;
        case P_B:              return E_B;
        case P_AE:             return E_AE;
        case P_BE:             return E_BE;
        case P_NE:             return E_NE;
        case P_E:              return E_E;
        case P_NOT:
            break;
        }
        printf("コンパイル不能なレジスタの命令が発効されました.");
        throw 0;    //例外発生. RevPorlandMainまで吹き飛ばします (^^;;

        return 0xff;
}

/*****************************************************/
//レジスターが絡む計算.
/*****************************************************/
void TScripteRevPorland::RegisterProc(struct CalsData *A,int Code,struct CalsData *B)
{
    //演算子を決める.
    unsigned char Op  = GetCode(Code);

    //      Reg , Reg +0
    //      Reg , 1   +1
    //      1   , Reg +2
    // の規約により、　数字　レジスタの関係で演算命令の修正.
    //なお、  1 , 1 の場合は、そもそもこのルーチンにこない.
    //だって、　レジスタが含まれる演算のみをここでやっているからね. (^^;
    if (A->Suji == false && B->Suji == true  ) Op +=1;//Reg , 1   +1
    if (A->Suji == true  && B->Suji == false ) Op +=2;//1   , Reg +2

    // S レジスタを必要とする計算.
    if (B->Suji == false && B->O == REG_S)
    {
        //計算結果の S レジスタの保護.
        NewCommand(E_POP,REG_S_EX); //  第 6 レジスタへ.
    }
    // S レジスタを必要とする計算.
    if (A->Suji == false && A->O == REG_S)
    {
        //計算結果の S レジスタの保護.
        NewCommand(E_POP,REG_S); //  第 5 レジスタへ.
    }

    //新規命令発効.
    NewCommand(Op,A->O,B->O);
    //計算結果の S レジスタの保護.
    NewCommand(E_PUSH,REG_S);
    //逆ポーランド法をいじするために、スタックに計算コードを入れる.
    NewClasData(false,REG_S);
}


//計算処理
bool TScripteRevPorland::StackClasProc()
{
    struct CalsData A;
    struct CalsData B;
    struct CalsCode Code;

	if ( ! GetCommandData(&B) )	return false;	//スタックが空.
	if ( ! GetCommandData(&A) )
	{
		// A が無いが、 B はあった場合.
		//この時は、 Abs(10) の 10 のときのようなことであるといえる.
		//ゆえに、 B の値を REG_S レジスタに格納.
		if (B.Suji)	NewCommand(E_MOVI,REG_S,B.O);	//数字だよ
		else		NewCommand(E_MOV,REG_S,B.O);	//レジスタだよ.
 		return false;
	}
	if ( ! GetCommandCode(&Code))  return false;	//演算子スタックが空.
    // A 演算 B をする.
    //printf("\n%d  %d  %d\n",A.O,Code.Code,B.O);

    if (A.Suji && B.Suji)
    {   //数字同士だったら計算できるので計算
        ClasProc(A.O,Code.Code,B.O);
    }
    else
    {   //レジスターのからむ計算.
        RegisterProc(&A,Code.Code,&B);
    }
        
    return true;
}


/*****************************************************/
//演算子などのチェック
/*****************************************************/
void TScripteRevPorland::CodeCheck(int Code)
{
    struct CalsCode A;
    //現在のコードのレベル 
    int NowCodeLevel = CodeLevel[Code] + Kaco_Next_Add_Level;
    while(1)
    {
        if (CalsCodeStack.size() <= 0) break; //スタック空っぽ
        A = CalsCodeStack.top();              //前回の演算子を取得.
        if (A.O < NowCodeLevel) break;

        //現在より高レベルの演算子だったら処理.
        if ( ! StackClasProc() ) break;
    }
    //計算記号をスタックに登録
    NewClasCode(Code,NowCodeLevel);
}

/*****************************************************/
//レジスターを取得
/*****************************************************/
void TScripteRevPorland::Register(char* &s)
{
    s++;
    if (*s >= '0' && *s <= '6')
    {
        NewClasData(false,(*s - '0') );
    }
    else
    {
        printf("おかしいレジスター指定です. %d",*s - '0');
        throw 0;    //例外発生. RevPorlandMainまで吹き飛ばします (^^;;
    }
}

/*****************************************************/
//現在のアルゴリズムでは、最後によけいな push が入るのでそれを削除.
//また、ついでに　無駄なスタック PUSh POP をも削除して最適化を行う (ぷっ
/*****************************************************/
void TScripteRevPorland::PushKiller(int StartMActionNumber)
{
    enum _YokeinaPushPop{ YOKEI_NONE , YOKEI_POP};
    int IP;                         //現在の命令のアドレス.
    _YokeinaPushPop Yokei5,Yokei6;  //よけいな PUSH POP 削除のフラグ

    bool LastPushKill = false; //最後の Push を削除したというフラグ
    IP = _MAction.size() - 1;   //ループする回数.
    Yokei5 = Yokei6 = YOKEI_NONE;   //よけいな PUSH POP 削除のフラグを初期化.
    //最適化ループ. (配列を逆たどっていきます.)
    for( ; StartMActionNumber <= IP; IP--)
    {
        switch(_MAction[IP].Nim)
        {
		case E_MOV:
			if ( _MAction[IP].O1 == _MAction[IP].O2)
			{
                _MAction.erase(&_MAction[IP]);
			}
            Yokei5 = Yokei6 = YOKEI_NONE; break;
        case E_POP:
            switch(_MAction[IP].O1)
            {
            case REG_S: //5番レジスタ
                Yokei5 = YOKEI_POP; break;
            case REG_S_EX: //6番レジスタ
                Yokei6 = YOKEI_POP; break;
            default:
                Yokei5 = Yokei6 = YOKEI_NONE; break;
            }
        case E_PUSH:
            if (! LastPushKill)
            {
                //最後の push を消し去る.
                _MAction.erase(&_MAction[IP]);
                LastPushKill = true;    //削除承認.
            }
            else
            {
                if (Yokei5 == YOKEI_POP || Yokei6 == YOKEI_POP)
                {   //PUSH した次のステップで POP しているので無駄と判断.
                    _MAction.erase(&_MAction[IP]);        //消去. PUSH
                    _MAction.erase(&_MAction[IP-1]);        // POP
                }
            }
        default:
            Yokei5 = Yokei6 = YOKEI_NONE; break;
        }
    }
}

/*****************************************************/
//逆ポーランド法のメイン
/*****************************************************/
void TScripteRevPorland::RevPorlandMain(char *Target)
{
    char *s = Target;
    int SujiBuf;

    //括弧ネストで増える優先度の初期化
    Kaco_Next_Add_Level = 0;
    int StartMActionNumber;  //コンパイラが吐き出す命令コードの最初の命令の場所
    //最初の命令の場所を記録しておきます.
    //あとで、最適化するときに使用したりします. うへっ
    StartMActionNumber = 0;
    //リストクリア.
    _MAction.clear();
    //解析ループ
    while(*s)
    {   
        switch(*s )
        {
        case '=':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_E); s++; } 
            else CodeCheck(P_EQU);  //=
            break;
        case '+':            CodeCheck(P_ADD);            break;
        case '-':            CodeCheck(P_SUB);            break;
        case '*':            CodeCheck(P_MUL);            break;
        case '/':            CodeCheck(P_DIV);            break;
        case '%':            CodeCheck(P_MOD);            break;
        case '>':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_AE); s++; } 
            else if (*(s+1) == '>' ) //>>
            { CodeCheck(P_SHR); s++;}
            else CodeCheck(P_A); 
            break;
        case '<':
            if (*(s+1) == '=' ) //==
            { CodeCheck(P_BE); s++; } 
            else if (*(s+1) == '<' ) //<<
            { CodeCheck(P_SHL); s++;}
            else CodeCheck(P_B); 
            break;
        case '&':            CodeCheck(P_AND);            break;
        case '|':            CodeCheck(P_OR);             break;
        case '!':            CodeCheck(P_NOT);            break;
        case '~':            CodeCheck(P_XOR);            break;
        case '(':            Kaco_Next_Add_Level += KACO_NEST_LEVEL;           break;
        case ')':            Kaco_Next_Add_Level -= KACO_NEST_LEVEL;           break;
        case 'R':        case 'r':            Register(s);                 break;
        case '0':        case '1':        case '2':        case '3':
        case '4':        case '5':        case '6':        case '7':
        case '8':        case '9':
            //数字.
            SujiBuf = SujiGet(s);
            NewClasData(true,SujiBuf);        break;
        }
        //文字列を進める.
        s ++;
    }
    //スタックが空になるまで処理します. 上から押していくみたいな感じでっか.
    while( StackClasProc( ) );
    //現在のアルゴリズムでは、最後によけいな push が入るのでそれを削除.
    //また、最適化を行う.
    PushKiller(StartMActionNumber);
}

/*****************************************************/
//逆ポーランド法だよん
/*****************************************************/
bool TScripteRevPorland::RevPorland(char *Target,ACTION_VECTOR &MasterAction)
{

    try
    {
        RevPorlandMain(Target);
    }
    catch(...)
    {
        printf("解析ルーチンの中でエラーが発生しまた. へっ");
        return false;
    }
    //連結.
    for(int i = 0; i < _MAction.size() ; i++ )
    {
        MasterAction.push_back( _MAction[i] );
    }
    
    return true;
}

/*****************************************************/
//コンストラクタ
/*****************************************************/
TScripteRevPorland::TScripteRevPorland()
{
    //コードレベル.
    static unsigned char _CodeLevel[] = 
    {
        1,  //EQU
        5,  //ADD
        5,  //SUB
        7,  //MUL
        7,  //DIV
        7,  //MOD
        3,  //SHL
        3,  //SHR
        3,  //AND
        3,  //OR
        0,  //A
        0,  //B
        0,  //AE
        0,  //BE
        0,  //NE
        0,  //E
        3,  //NOT
    };
    CodeLevel = _CodeLevel ;
}
#endif //LIFE END
