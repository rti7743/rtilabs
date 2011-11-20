#include <StartWithExeFile.h>
#ifdef ZONDE_LIFE
/**************************************************************/
//手書き文字認識ルーチン (ゾンデ法) (予定(^^;)
/**************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Graphics\rti_graphics.h>
#include <\.AI\rti_Ai_Zonde.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>



/**************************************************************/
//ゾンデ法の初期化
/**************************************************************/
void TAIZonde::Init()
{
	if (! LoadLog(LOG_NAME) )
	{
		ClearLog();	
	}
}
/**************************************************************/
//指定した絵にかかれている文字データを認識します!!
/**************************************************************/
void TAIZonde::Start(BMD bmd,int MojiColor)
{
	//情報をゲットします.
	ZondeGetData(bmd,MojiColor);
	//その情報とデータベースを比較します!
	ZondeMatting();
}

/**************************************************************/
//ゾンデ法で求めた結果にたいして合否をいい渡す!!
/**************************************************************/
void TAIZonde::HumanCheck(unsigned char SekaiMoji)
{
	if (SekaiMoji == Char)
	{
		//大正解!! 頭をなでなでしてあげる.
		CharPattern[Char][Char_PatternNo].OkPattern ++;
	}
	else
	{
		//不正解なので、正解の文字のデータすべてケツバット!!
		for(int i = 0; i < MAX_PATTERN ; i++)
		{
			//ケツバットを食らわせる!! ばしっ 
			CharPattern[Char][i].OkPattern --;
		}
		//正解の文字の認識データをぶち込む.
		InsertData(SekaiMoji);
	}
}

/**************************************************************/
//ゾンデ法の終了
/**************************************************************/
void TAIZonde::End()
{
	SaveLog(LOG_NAME);
}

/**************************************************************/
//過去の記録をゲットします.
/**************************************************************/
bool TAIZonde::LoadLog(char *filename)
{
	FILE *fp;
	//ファイルよ! 開け!
	fp = fopen(filename,"rb");
	if (!fp)
	{
		//ファイルが開けなかったんがー
        FILE_NOT_OPEN("TAIZonde::LoadLog",filename);
		return false;
	}
	//書き込みだんがー
	fread(CharPattern,sizeof(struct _CharPattern),ALL_CHAR * MAX_PATTERN,fp);
	//ファイルよ、閉じよ!!
	fclose(fp);
	return true;
}

/**************************************************************/
//過去の記録します.
/**************************************************************/
bool TAIZonde::SaveLog(char *filename)
{
	FILE *fp;
	//ファイルよ! 開け!
	fp = fopen(filename,"wb");
	if (!fp)
	{
		//ファイルが開けなかったんがー
        FILE_NOT_OPEN("TAIZonde::SaveLog",filename);
		return false;
	}
	//書き込みだんがー
	fwrite(CharPattern,sizeof(struct _CharPattern),ALL_CHAR * MAX_PATTERN,fp);
	//ファイルよ、閉じよ!!
	fclose(fp);
	return true;
}

/**************************************************************/
//記憶喪失にします(ぉ
/**************************************************************/
void TAIZonde::ClearLog()
{
	int i,l,m;
	for(i = 0; i < ALL_CHAR ; i++)
	{
		for(l = 0; l < MAX_PATTERN ; l++)
		{
			for (m = 0 ; m < ZONDE_LINE ; m++)
			{
				CharPattern[i][l].LineHit[m] = 100;
			}
			CharPattern[i][l].OkPattern = 0;
			CharPattern[i][l].DataAlive = false;
		}
	}
}

/**************************************************************/
//情報をゲットします.
/**************************************************************/
void TAIZonde::ZondeGetData(BMD bmd,int MojiColor)
{
	//最適化しないボケ ゲイツ君のコンパイラ用にこうでもしておきましょうか.
	int BmdW = bmd->w;
	int BmdH = bmd->h;
	int w4,w2;
	int i;
	//クリアしておきましょうか. 本当は宣言の時にやってもいいんですが(^^;;
	ZeroMemory(&TestPattern,sizeof(struct _TestPattern) );

	w4 = BmdW / 4;
	for(i = 0 ; i < BmdW ; i++){
		ZondeCheck(bmd,w4,i,0,MojiColor);
		ZondeCheck(bmd,w4*2,i,1,MojiColor);
		ZondeCheck(bmd,w4*3,i,2,MojiColor);
		ZondeCheck(bmd,i,w4,3,MojiColor);
		ZondeCheck(bmd,i,w4*2,4,MojiColor);
		ZondeCheck(bmd,i,w4*3,5,MojiColor);
		ZondeCheck(bmd,i,i,6,MojiColor);
		ZondeCheck(bmd,BmdW - i,i,7,MojiColor);
	}
	w2 = BmdW / 2;
	for(i = 0 ; i < w2 ; i++){
		ZondeCheck(bmd,i,w2 - i,8,MojiColor);
		ZondeCheck(bmd,i,w2 + i,9,MojiColor);
		ZondeCheck(bmd,i + w2,BmdW - i - 1,10,MojiColor);
		ZondeCheck(bmd,i + w2,i,11,MojiColor);
	}

}

/**************************************************************/
//チェックします!
/**************************************************************/
void TAIZonde::ZondeCheck(BMD bmd,int x,int y,int n , int MojiColor)
{
	long c;
		
	c = PGet(bmd,x,y);
	if( c != MojiColor)
	{  //背景
		TestPattern.BigLine[n] = false;
	}
	else
	{ //線
		//太い線を1つとみなす。
		if( ! TestPattern.BigLine[n] )
		{ 
			TestPattern.LineHit[n]++; //交差した回数をカウント
			TestPattern.BigLine[n] = true;
		}
	}
}

/**************************************************************/
//チェックした内容を比較してみるるるるるる.
/**************************************************************/
void TAIZonde::ZondeMatting()
{
	int ihozon,lhozon;
	int min;
	int i,l,d;
	ihozon = 0;
	lhozon = 0;
	min  = 0xffff;

	for(i = 0; i < ALL_CHAR ; i++)
	{
		for(l = 0; l < MAX_PATTERN ; l++)
		{
			d = ZondeDistance(i,l);
			if (d < min)
			{	//最低値更新.
				min = d;
				ihozon = i;
				lhozon = l;
			}
		}
	}
	//返り血もどき.
	Char = (unsigned char)ihozon;
	Char_PatternNo = lhozon;
}

/**************************************************************/
//データベースと現在のデータの比較. (距離を求める) 2乗和を求める.
/**************************************************************/
int TAIZonde::ZondeDistance(int i,int l)
{
	int m,sum , t;
	struct _CharPattern *CPI;
	sum = 0;
	CPI = &CharPattern[i][l];

	for(m = 0 ; m < 12 ; m++)
	{
		t = ( CPI->LineHit[m] - TestPattern.LineHit[m]);
		sum += t*t;
	}
	return( sum );
}

/**************************************************************/
//正解のデータを埋め込む
/**************************************************************/
void TAIZonde::InsertData(unsigned char SekaiMoji)
{
	bool InsertOk = false;
	int  MinOk = 0xffff;
	int  iMin  = 0;
	struct _CharPattern *CP = CharPattern[SekaiMoji];
	struct _CharPattern *CPI;

	for(int i = 0; i < MAX_PATTERN ; i++)
	{
		//あいているデータを探す.
		CPI = &CP[i];
		if ( ! CPI->DataAlive )
		{	//空き発見!!
			InsertOk = true;
			memcpy( &CPI->LineHit , &TestPattern.LineHit , sizeof(int) * ZONDE_LINE);
			CPI->OkPattern = 0;
			CPI->DataAlive  = true;
			break;
		}
		//もっとも認識率のわるデータ君を探してます.
		if (MinOk > CPI->OkPattern)
		{	//最低値更新!!
			MinOk = CPI->OkPattern;
			iMin  = i;
		}
	}
	if (InsertOk) return ; //挿入されました. めでたし めでたし.

	CPI = & CharPattern[SekaiMoji][iMin];
	//挿入するスペースがないので、
	//もっとも認識率の悪いデータを消して、そのデータをいれる.
	memcpy( &CPI->LineHit , &TestPattern.LineHit , sizeof(int) * ZONDE_LINE);
	CPI->OkPattern = 0;
	CPI->DataAlive  = true;
}

#endif	//LIFE END
