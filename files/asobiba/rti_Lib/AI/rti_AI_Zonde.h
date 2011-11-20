#include <StartWithExeFile.h>
#ifdef ZONDE_LIFE

#ifndef ___ZONDEH
#define ___ZONDEH

#define ALL_CHAR	256	//認識可能な文字数
#define MAX_PATTERN	10	//一つの項目について記録できる事例.
#define ZONDE_LINE	12
#define LOG_NAME	"Zonde.dat"
//関数宣言.
class TAIZonde
{
	//記憶だよん.
	struct _CharPattern
	{
		int	LineHit[ZONDE_LINE];	//ラインとヒットした数
		int	OkPattern;				//認識に成功した数
		bool DataAlive;				//そのデータが有効かどうか.
	};

	struct _TestPattern
	{
		int	 LineHit[ZONDE_LINE];	//ラインとヒットした数
		bool BigLine[ZONDE_LINE];	//でかいラインを 1 とみなす.
	};

	//記憶領域の作成
	struct _CharPattern CharPattern[ALL_CHAR][MAX_PATTERN];
	//認識テスト用のデータです.
	struct _TestPattern	TestPattern;
	unsigned char Char;	//認識している文字
	int			  Char_PatternNo;	//その文字のデータを返してくれたところ
	//過去の記録をゲットします.
	bool LoadLog(char *filename);
	//過去の記録します.
	bool SaveLog(char *filename);
	//記憶喪失にします(ぉ
	void ClearLog();
	//情報をゲットします.
	void ZondeGetData(BMD bmd,int MojiColor);
	//チェックします!
	void ZondeCheck(BMD bmd,int x,int y,int n , int MojiColor);
	//チェックした内容を比較してみるるるるるる.
	void ZondeMatting();
	//データベースと現在のデータの比較. (距離を求める) 2乗和を求める.
	int ZondeDistance(int i,int l);
	//正解のデータを埋め込む
	void InsertData(unsigned char SekaiMoji);
public:
	TAIZonde(){};
	//ゾンデ法の初期化
	void Init();
	//ゾンデ法で求めた結果にたいして合否をいい渡す!!
	void HumanCheck(unsigned char SekaiMoji);
	//ゾンデ法の終了
	void End();
	//指定した絵にかかれている文字データを認識します!!
	void Start(BMD bmd,int MojiColor);
	//コード取得
	unsigned char GetCode(){return Char;};
};

#endif

#endif	//END LIFE
