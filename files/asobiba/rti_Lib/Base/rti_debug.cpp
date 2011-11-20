/*****************************************************************/
/*****************************************************************/
// グランドなデバッグルーチン
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef DEBUG_LIFE

#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_debug.h>

#define DEBUGFILE "DEBUG.TXT"


void DEBUG(char *str)
{
	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	fprintf(fp,"%s\r\n",str);
	fclose(fp);
}


void DEBUG2(int i)
{
	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	fprintf(fp,"%d\r\n",i);
	fclose(fp);
}

void DEBUG3(char* Format,...)
{

	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	vfprintf(fp,Format,(char*)(&Format+1));
	fputs("\r\n",fp);
	fclose(fp);

}

//時間計測を行う場合.
static unsigned long Max = 0;			//最大レート(最悪の結果)
static unsigned long Min = 0xffff;		//最低レート(最高の結果)
static unsigned long Avg = 100;			//平均レート
void NewRecordFPS(unsigned long rec)
{
	Avg = (Avg + rec) /2;
	if (Max < rec) Max = rec;
	if (Min > rec) Min = rec;
}

void WriteRecordFPS(unsigned long DesignatedTime)
{
	DEBUG3(" 指定レート     %ld \n 瞬間最大レート %ld "
				"\n 瞬間最低レート %ld \n 平均レート     %ld",DesignatedTime,Max,Min,Avg);
}


#endif // LIFE END
