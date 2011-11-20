#include "StartWithExeFile.h"
#ifdef SCRIPTE_LIFE

#include <.\Game\rti_Game_Scripte_Run.h>
#include <.\Game\rti_Game_Scripte_Comple.h>

/*****************************************************/
//デバッグ
/*****************************************************/
void TScripteRun::Debug(int c)
{
    char *Operation[] = 
    {
    "E_MOV","E_MOVI","E_MOVII","E_ADD","E_ADDI","E_ADDII","E_SUB","E_SUBI","E_SUBII",
    "E_MUL","E_MULI","E_MULII","E_DIV","E_DIVI","E_DIVII","E_MOD","E_MODI","E_MODII",
    "E_SHL","E_SHLI","E_SHLII","E_SHR","E_SHRI","E_SHRII","E_AND","E_ANDI","E_ANDII",
    "E_OR","E_ORI","E_ORII",   "E_XOR","E_XORI","E_XORII",
    "E_A","E_AI","E_AII","E_B","E_BI","E_BII",
    "E_AE","E_AEI","E_AEII","E_BE","E_BEI","E_BEII",
    "E_NE","E_NEI","E_NEII","E_E","E_EI","E_EII",
    "E_NOT","E_NOTI","E_NOTII",
    "E_PUSH","E_PUSHI","E_POP","E_POPI",
    "E_JMP","E_IF","E_HAL","E_FUNK",
    };
    std::vector<Action>::iterator it ;
    for(it = MAction[c].begin() ; it != MAction[c].end() ; it ++)
    {
        printf("%s      %d  %d\n",
            Operation[ (*it).Nim ] ,(*it).O1,(*it).O2);
    }
}
/*****************************************************/
//そのファイルをコンパイルしてデータを格納
/*****************************************************/
bool TScripteRun::Comple(char *filename,USER_FUNKTION *Funk,char **Str)
{
    TScripteComple SC;
    //UserFanktion登録
	UserFunKeep(Funk,Str);
    //文字列UserFanktion登録.
    SC.SetFunktion(Str);
    return SC.Comple(filename,MAction,&UseString);
}

/*****************************************************/
//そのファイルをコンパイルしてデータを格納
/*****************************************************/
bool TScripteRun::Comple(char *filename,TScripteComple *SC,USER_FUNKTION *Funk,char **Str)
{
    //UserFanktion登録
	UserFunKeep(Funk,Str);
    //文字列UserFanktion登録.
    SC->SetFunktion(Str);
    return SC->Comple(filename,MAction,&UseString);
}

/*****************************************************/
//UserFunk の保存
/*****************************************************/
void TScripteRun::UserFunKeep(USER_FUNKTION *Funk,char **StrCommand)
{
	int UseNum;	//Funk の個数.
	char *StrTemp;
	 
	if (StrCommand == NULL) return; //ユーザ定義ファンクションは存在しない.
	//Funk の個数を数える. Funk の個数は StrCommand の数と等しい(ただし最後のNULL は Funk にはない.)
	for(UseNum = 0 ,StrTemp = StrCommand[0] ; *StrTemp  ; UseNum++ , StrTemp++ );

	if (UseNum == 0) return;	//それまた、ユーザ定義ファンクションは存在しない.

	//確保.
	UserFunk = new USER_FUNKTION[UseNum];
	//転送.
	memcpy(UserFunk,Funk,sizeof(USER_FUNKTION) * UseNum);
}

/*****************************************************/
//Run
/*****************************************************/
int __fastcall TScripteRun::Run(int c,TScripeRunArea *SRA)
{
    register int *R;
    if ((MAction[c]).size() <= SRA->IP) return RET_END_WORD_RUN;  //最後まで命令を実行した.
    //現在の命令を取得. なお、読み込んだ後に　次の命令に IP を進めておく.
    Action *A = & ((MAction[c])[SRA->IP++]);
    R = SRA->R; //レジスタはよく使うので、あらかじめ取得しておく (^^;
    //ニモニック別に処理
    //None       Reg    Reg
    //I          Reg    Int
    //I          Int    Reg
    switch(A->Nim)
    {
    case E_MOV:     R[A->O1] = R[A->O2];        break;
    case E_MOVI:    R[A->O1] = A->O2;           break;
    case E_MOVII:   /*不正*/                    break;
    case E_ADD:     R[REG_S] = R[A->O1] + R[A->O2];       break;
    case E_ADDI:    R[REG_S] = R[A->O1] + A->O2;          break;
    case E_ADDII:   R[REG_S] = A->O1    + R[A->O2];       break;
    case E_SUB:     R[REG_S] = R[A->O1] - R[A->O2];       break;
    case E_SUBI:    R[REG_S] = R[A->O1] - A->O2;          break;
    case E_SUBII:   R[REG_S] = A->O1    - R[A->O2];       break;
    case E_MUL:     R[REG_S] = R[A->O1] * R[A->O2];       break;
    case E_MULI:    R[REG_S] = R[A->O1] * A->O2;          break;
    case E_MULII:   R[REG_S] = A->O1    * R[A->O2];       break;
    case E_DIV:     R[REG_S] = R[A->O1] / R[A->O2];       break;
    case E_DIVI:    R[REG_S] = R[A->O1] / A->O2;          break;
    case E_DIVII:   R[REG_S] = A->O1    / R[A->O2];       break;
    case E_MOD:     R[REG_S] = R[A->O1] % R[A->O2];       break;
    case E_MODI:    R[REG_S] = R[A->O1] % A->O2;          break;
    case E_MODII:   R[REG_S] = A->O1    % R[A->O2];       break;
    case E_SHL:     R[REG_S] = R[A->O1] << R[A->O2];      break;
    case E_SHLI:    R[REG_S] = R[A->O1] << A->O2;         break;
    case E_SHLII:   R[REG_S] = A->O1    << R[A->O2];      break;
    case E_SHR:     R[REG_S] = R[A->O1] >> R[A->O2];      break;
    case E_SHRI:    R[REG_S] = R[A->O1] >> A->O2;         break;
    case E_SHRII:   R[REG_S] = A->O1    >> R[A->O2];      break;
    case E_AND:     R[REG_S] = R[A->O1] & R[A->O2];       break;
    case E_ANDI:    R[REG_S] = R[A->O1] & A->O2;          break;
    case E_ANDII:   R[REG_S] = A->O1    & R[A->O2];       break;
    case E_OR:      R[REG_S] = R[A->O1] | R[A->O2];       break;
    case E_ORI:     R[REG_S] = R[A->O1] | A->O2;          break;
    case E_ORII:    R[REG_S] = A->O1    | R[A->O2];       break;
    case E_XOR:     R[REG_S] = R[A->O1] ^ R[A->O2];       break;
    case E_XORI:    R[REG_S] = R[A->O1] ^ A->O2;          break;
    case E_XORII:   R[REG_S] = A->O1    ^ R[A->O2];       break;
    case E_A:       R[REG_S] = (R[A->O1] > R[A->O2]);       break;
    case E_AI:      R[REG_S] = (R[A->O1] > A->O2);          break;
    case E_AII:     R[REG_S] = (A->O1    > R[A->O2]);       break;
    case E_B:       R[REG_S] = (R[A->O1] < R[A->O2]);       break;
    case E_BI:      R[REG_S] = (R[A->O1] < A->O2);          break;
    case E_BII:     R[REG_S] = (A->O1    < R[A->O2]);       break;
    case E_AE:      R[REG_S] = (R[A->O1] >= R[A->O2]);       break;
    case E_AEI:     R[REG_S] = (R[A->O1] >= A->O2);          break;
    case E_AEII:    R[REG_S] = (A->O1    >= R[A->O2]);       break;
    case E_BE:      R[REG_S] = (R[A->O1] <= R[A->O2]);       break;
    case E_BEI:     R[REG_S] = (R[A->O1] <= A->O2);          break;
    case E_BEII:    R[REG_S] = (A->O1    <= R[A->O2]);       break;
    case E_NE:      R[REG_S] = (R[A->O1] != R[A->O2]);       break;
    case E_NEI:     R[REG_S] = (R[A->O1] != A->O2);          break;
    case E_NEII:    R[REG_S] = (A->O1    != R[A->O2]);       break;
    case E_E:       R[REG_S] = (R[A->O1] == R[A->O2]);       break;
    case E_EI:      R[REG_S] = (R[A->O1] == A->O2);          break;
    case E_EII:     R[REG_S] = (A->O1    == R[A->O2]);       break;
    case E_PUSH:    SRA->ActionStack.push(R[A->O1]);            break;
    case E_PUSHI:   SRA->ActionStack.push(A->O1);               break;
    case E_POP:     R[A->O1] = SRA->ActionStack.top();   SRA->ActionStack.pop();  break;
    case E_POPI:    A->O1 = SRA->ActionStack.top();   SRA->ActionStack.pop();  break;
    case E_JMP:     SRA->IP = A->O1;                            return 0;
    case E_IF:      if( !R[REG_S] ) { SRA->IP = A->O1; } break;
    case E_HAL:     SRA->IP = 99999; return RET_END_WORD_RUN;    //終了命令.
    case E_FUNK:    
        {
            int ParaMeta[USER_FUNKTION_MAX];
            //引数は逆にとっていかなくてはならない(スタックだもん.FILO)
            for(register int i = A->O2 ; i >= 0 ; i-- )
            {
                ParaMeta[i] = SRA->ActionStack.top();
                SRA->ActionStack.pop();
            }
            //ユーザ定義の関数の呼び出し.
            return R[REG_S] = (UserFunk[A->O1])(SRA,ParaMeta[0],ParaMeta[1],ParaMeta[2],ParaMeta[3],ParaMeta[4]);
        }
        break;
    default:
        return UserOperation(A,SRA);   //ユーザ定義のニモニック.
        break;
    }
    return RET_RESERVED_WORD_RUN;
}

#endif //LIFE END
