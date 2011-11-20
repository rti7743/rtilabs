/*****************************************************************/
/*****************************************************************/
//エラーメッセージ
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef ERROR_MESSAGE_LIFE

#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_error_message.h>
#include <.\VCL\rti_vcl_apprication.h>

static int MsgOut(const char *Title,int Flg,char* Format,...);

int MsgOut(const char *Title,int Flg,char* Format,...)
{
	char Buffer[1024];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	return MessageBox(NULL,Buffer,Title,Flg);
}

void MsgError3(char* Format,...)
{
	char Buffer[512];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	MessageBox(NULL,Buffer,"エラーだにょー",MB_OK | MB_TASKMODAL | MB_ICONSTOP | MB_SETFOREGROUND	);
}

int MsgOr3(char* Format,...)
{
	char Buffer[512];

	vsprintf(Buffer,Format,(char*)(&Format+1));
	strcat(Buffer,"\r\n");

	return MessageBox(NULL,Buffer,"確認!!",MB_OKCANCEL | MB_TASKMODAL);
}

void FriendlyError(const char *FunktionName,const char *Inpor,const char *Reason,const char *Support)
{
    MsgError3("場所  : 関数 %s \n行為  : %sを失敗したにょー.\n理由  : %s\n対応  : %s",FunktionName,Inpor,Reason,Support);
}

void FriendlyNameError(const char *FunktionName,const char *Name,const char *Reason,const char *Support)
{
    MsgError3("場所  : 関数 %s \n行為  : %sはおかしいにょー.\n理由  : %s\n対応  : %s",FunktionName,Name,Reason,Support);
}

void FileNotOpen(const char *FunktionName,const char *Filename)
{
    MsgError3("場所  : 関数 %s \n行為  : ファイル %s オープンに失敗したにょー",FunktionName,Filename);
}

void MemoryDeficient(const char *FunktionName,const char *Inpor,int Size)
{
    MsgError3("場所  : 関数 %s \n行為  : %sを失敗したにょー.\n理由  : メモリが %d バイト不足しているゲマ\n対応  : メモリをもっと確保してほしいにょー",FunktionName,Inpor,Size);
}

void ProgramError(const char *FunktionName,const char *Inpor,const char *Reason)
{
    MsgError3("場所  : 関数 %s \n行為  : %sを失敗したにょー.\n理由  : %s\n対応  : プログラムエラーなので作者にクレームをたれましょう(汗)",FunktionName,Inpor,Reason);
}


#endif //LIFE END
