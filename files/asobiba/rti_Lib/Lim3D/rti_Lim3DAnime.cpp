#include <./Lim3D/rti_Lim3DAnime.h>
#include <./Lim3D/rti_Lim3DMaster.h>
#include <stdio.h>

/*******************************************************************************/
//TLim3DAnimeControlクラス	アニメーションの管理をつかさどります
//							ユーザーはこのクラスの内容を知っていれば、
//							その他の下請けクラスは知らなくても OK なのらー.
/*******************************************************************************/

/*******************************************************************************/
//タグをサーチする.
//引数						inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//							inTag			タグ名.	Ex. "<MODEL = "
//							outEnd			見つけた文字列の最後.(失敗の時は未定義)
//戻り値					見つけた文字列の先頭.
/*******************************************************************************/
char* TLim3DAnimeControl::SearchTag
		(const char* inStart , const char* inEnd , const char* inTag , char** outEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);
	__RTI_CHECKER(inTag != NULL);
	__RTI_CHECKER(outEnd != NULL);

	//最初にヒットする場所を返す.
	char * theNowPos  = ::strstr( inStart , inTag);
	//取得失敗だったら没.
	if (theNowPos == NULL || theNowPos >= inEnd) return NULL;

	//次にヒットする場所を探す.
	*outEnd = ::strstr( theNowPos+1 , inTag);
	//取得できない or inEnd オーバーの時は、 inEnd が終端とする.
	if (*outEnd == NULL || *outEnd >= inEnd) *outEnd = (char*)inEnd;

	return theNowPos;
}

/*******************************************************************************/
//シーンデータ & タスクデータの破棄.
//引数						なし
//戻り値					なし
/*******************************************************************************/
void	TLim3DAnimeControl::Clear()
{
	{
		std::vector<TLim3DAnimeSeen*>::iterator it = 	m_Seenvector.begin();
		std::vector<TLim3DAnimeSeen*>::iterator end= 	m_Seenvector.end();
		for( ; it != end ; it++)	
		{	
			delete *it;
		}
		m_Seenvector.clear();
	}
	{
		std::vector<TLim3DAnimeTask*>::iterator it = 	m_Taskvector.begin();
		std::vector<TLim3DAnimeTask*>::iterator end= 	m_Taskvector.end();
		for( ; it != end ; it++)	
		{	
			delete *it;
		}
		m_Taskvector.clear();
	}
}

/*******************************************************************************/
//アニメーションファイルを読み込みます.
//引数						inFileName		読み込むファイル名.
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeControl::Read(const char * inFileName)
{
	__RTI_CHECKER(inFileName != NULL);

	FILE *		theFp;
	theFp = fopen(inFileName , "rb");
	if (theFp == NULL)	return false;

	char *	theFileData;
	int		theFileLen;

	//ファイルサイズを取得.
	::fseek( theFp , 0 , SEEK_END);
	theFileLen = ::ftell(theFp);
	::fseek( theFp , 0 , SEEK_SET);

	//領域の確保.
	theFileData = new char[theFileLen];

	//領域にデータを流し込む.
	::fread( theFileData , sizeof(char) , theFileLen , theFp );
	//ファイルのクローズ.
	::fclose( theFp );

	//シーンを追加したりしていく.
	bool theRet = SeenSplit( theFileData , &theFileData[theFileLen] );
	if (theRet == false) return false;

	//読み込んだデータの破棄.
	delete [] theFileData;

	return true;
}

/*******************************************************************************/
//アニメーションの実行のためのタスクを作成する.
//引数						inAnimeName			アニメーション名.
//戻り値					成功 TLim3DAnimeTask*   / 失敗 NULL
/*******************************************************************************/
TLim3DAnimeTask*	TLim3DAnimeControl::TaskCreate(const char * inAnimeName)
{
	__RTI_CHECKER(inAnimeName != NULL);

	//ネームサーバーより、theModelName のモデルを探す.
	TLim3DAnimeSeen* theAnimeSeen = 
				(TLim3DAnimeSeen*) TLim3DMaster::NameServer.Get( inAnimeName );
	if (theAnimeSeen == NULL)	return NULL;

	//タスクの作成.
	TLim3DAnimeTask* theTask = new TLim3DAnimeTask(theAnimeSeen);
	if (theTask == NULL) return NULL;

	//リストに追加する.
	m_Taskvector.push_back(theTask);

	return theTask;
}

/*******************************************************************************/
//シーンを分解します.
//引数						inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeControl::SeenSplit(const char * inStart ,const char * inEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	//場所制御.
	char * theNext =  (char*)inStart;
	char * theOld ;

	//シーンの名前.
	char theSeenName[64];

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<SEEN = " , &theNext );
		if (theOld == NULL) break;
		//そのシーンの名前の取得.
		::sscanf(theOld , "<SEEN = \"%63[^\"]\">" , &theSeenName);
		//シーン解析.
		SeenAnalysis(theSeenName, theOld , theNext);
	}
	return true;
}

/*******************************************************************************/
//シーン解析	Start と End がシーンデータ
//引数						inSeenName		シーンの名前
//							inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeControl::SeenAnalysis(const char * inSeenName,
								  const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inSeenName != NULL);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	bool theRet;
	//まず、シーンデータを作成する.
	TLim3DAnimeSeen* theSeen = new TLim3DAnimeSeen;
	if (theSeen == NULL)
	{//メモリ不足.
		return false;
	}
	//シーンをフレーム単位で分解します.
	theSeen->FreamSplit( inStart , inEnd);

	//ネームサーバーに追加する.
	theRet = TLim3DMaster::NameServer.Add( inSeenName , theSeen );
	if (theRet == false)
	{//ネームサーバーに登録失敗.
		delete theSeen;
		return false;
	}
	//シーンをリストに追加します.
	m_Seenvector.push_back(theSeen);

	return true;
}

/*******************************************************************************/
//TLim3DAnimeTaskクラス		アニメーション実行を携わる.
//引数							
//戻り値					まだ継続 true / おしまい false
/*******************************************************************************/
bool TLim3DAnimeTask::Action()
{
	__RTI_CHECKER(m_Seen != NULL);

	bool	theRet;
	//アニメーション指令を出す.
	theRet = m_Seen->Action( m_FreamNum , m_Time);	
	//時間を増やします.
	m_Time++;
	//アニメーションはまだ継続させます.
	if (theRet == true)	return true;

	//現在の m_FreamNoのフレームのアニメーションは終了しました.
	//次のフレームにセットして、時間をリセットします.
	m_FreamNum++;
	m_Time = 0;

	//ところで、アニメーションはまだつづくの ?
	theRet = m_Seen->IsContinue(m_FreamNum);
	if (theRet == true)	return true;

	//もう、おしまいの場合.

	switch( m_Option )
	{
	case __ENUM_LIM3D_ANIME_TASK_OPTION_NONE	:
		break;
	case __ENUM_LIM3D_ANIME_TASK_OPTION_REPERT	:
		m_FreamNum = 0;
		m_Time = 0;
		break;
	case __ENUM_LIM3D_ANIME_TASK_OPTION_AUTOKILL:
		break;
	}

	return false;

}



/*******************************************************************************/
//TLim3DAnimeSeenクラス		アニメのシーンを取り扱います.
//							アニメは、1 シーン
/*******************************************************************************/

/*******************************************************************************/
//指定されたフレームを実行命令を出す.
//							ioFream			再生するフレーム.
//							inTime			
//戻り値					true / アニメーション終了 false
/*******************************************************************************/
bool	TLim3DAnimeSeen::Action(int inFreamNo, int inTime)
{
	__RTI_CHECKER(inFreamNo >= 0);
	__RTI_CHECKER(inFreamNo <= m_vector.size());

	bool theEndOfFreram;

	//そのフレームをアニメーション.
	theEndOfFreram = (m_vector[inFreamNo])->Action(inTime);
	//このフレームのアニメーションが完了したら、即リターン.
	if (theEndOfFreram == true)  return true;

	//このフレームは、終わりましたのでを求めます.
	return false;
}

/*******************************************************************************/
//フレームデータの破棄.
//引数						なし
//戻り値					なし
/*******************************************************************************/
void	TLim3DAnimeSeen::Clear()
{
	std::vector<TLim3DAnimeFream*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeFream*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	
		delete *it;
	}
	m_vector.clear();
}

/*******************************************************************************/
//フレームを分解します.
//							inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeSeen::FreamSplit(const char *  inStart , const char * inEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	//このフレームを何フレームで終了させればよいのか..
	int theFreamTime;

	//場所制御.
	char * theNext = (char*)inStart;
	char * theOld;

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<FREAM = " , &theNext );
		if (theOld == NULL) break;
		//そのフレームの終了させる時間の取得.
		::sscanf(theOld , "<FREAM = %d" , &theFreamTime);
		//シーン解析.
		FreamAnalysis(theFreamTime, theOld , theNext);
	}
	return true;
}

/*******************************************************************************/
//フレーム解析	Start と End がシーンデータ
//引数						inSeenName		シーンの名前
//							inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeSeen::FreamAnalysis(int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd   != NULL);

	bool	theRet;
	//新規フレーム作成.
	TLim3DAnimeFream * theFream = new TLim3DAnimeFream;
	if (theFream == NULL) return false;
	//このフレームを各種アニメーション単位に分解.
	theRet = theFream->BaseSplit(inFreamTime , inStart , inEnd );
	if (theRet == false)
	{
		delete theFream;
		return false;
	}
	//フレームを登録.
	m_vector.push_back(theFream);

	return true;
}

/*******************************************************************************/
//TLim3DAnimeFreamクラス	アニメのシーンを取り扱います.
//							アニメは、1 シーン
/*******************************************************************************/
THIS_ANALYSIS	 TLim3DAnimeFream::m_AnalysisTable[] = 
{
	&TLim3DAnimeModel::ThisAnalysis,
	NULL,
};

/*******************************************************************************/
//指定されたフレームを実行命令を出す.
//引数						inTime			現在の時間.
//戻り値					true / アニメーション終了 false
/*******************************************************************************/
bool	TLim3DAnimeFream::Action(int inTime)
{
	__RTI_CHECKER(inTime <= m_Time);

	//このフレームが持っているすべてのアニメーション単位(TLim3DAnimeBase)に
	//アニメーション指令を出します.
	std::vector<TLim3DAnimeBase*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeBase*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	//アニメーション実行.
		(*it)->Action();
	}
	//このフレームのアニメーションが終了していないならば...
	if ( inTime < m_Time)	return true;

	//終了しました.
	return false; 
}


/*******************************************************************************/
//各種アニメーション単位を分解します.
//引数						inSeenName		シーンの名前
//							inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeFream::BaseSplit(int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	THIS_ANALYSIS	* theThisAnalysis = TLim3DAnimeFream::m_AnalysisTable;

	for( ; *theThisAnalysis ; theThisAnalysis++)
	{	//その各種アニメーション単位で分解.
		(*theThisAnalysis)( this , inFreamTime , inStart , inEnd);
	}
	m_Time = inFreamTime;

	return true;
}

/*******************************************************************************/
//各種アニメーション単位の破棄.
//引数						なし
//戻り値					なし
/*******************************************************************************/
void	TLim3DAnimeFream::Clear()
{
	std::vector<TLim3DAnimeBase*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeBase*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	
		delete *it;
	}
	m_vector.clear();
}






/*******************************************************************************/
//TLim3DAnimeModel			モデルのアニメーション.
//							
/*******************************************************************************/


/*******************************************************************************/
//モデルアニメーションの解析.
//引数						inFream			Fream.
//							inSeenName		シーンの名前
//							inStart			シーンの開始場所.
//							inEnd			シーンの終了場所
//戻り値					成功 true
/*******************************************************************************/
bool TLim3DAnimeModel::ThisAnalysis
		(TLim3DAnimeFream * inFream ,int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);
	//場所制御.
	char * theNext = (char*)inStart;
	char * theOld ;

	//モデルの名前.
	char theModelName[64];
	int	 theX , theY , theZ , theRX , theRY  , theRZ;
	int  theRev;

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<MODEL = " , &theNext );
		if (theOld == NULL) break;
		//そのモデルの各種設定を取得、失敗したら没.
		theRev = ::sscanf(theOld , "<MODEL = \"%63[^\"]\" M(%d %d %d) R(%d %d %d)>" ,
						theModelName , &theX , &theY , &theZ , &theRX , &theRY  , &theRZ);
		if (theRev != 7) return false;

		//ネームサーバーより、theModelName のモデルを探す.
		MODEL* theModelPointer = (MODEL*)TLim3DMaster::NameServer.GetPointer( theModelName );
		MODEL theModel = *theModelPointer;

		if (theModel == NULL)	return false;

		//モデルの作成、失敗したら没.
		TLim3DAnimeModel* theAnimeModel = new TLim3DAnimeModel;
		if (theAnimeModel == NULL) return false;

		//データのセット.
		theAnimeModel->m_Model = theModelPointer;
		theAnimeModel->DataSet(inFreamTime , (Fixed)theX , (Fixed)theY , (Fixed)theZ , 
											(Angle)theRX , (Angle)theRY ,(Angle)theRZ );
		//Freamに登録.
		inFream->Add(theAnimeModel);
	}
	return true;
}


