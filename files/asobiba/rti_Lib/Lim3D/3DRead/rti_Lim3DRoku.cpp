#include <./Lim3D/3DRead/rti_Lim3DRoku.h>
#include <stdio.h>

/***********************************************************/
//コンストラクタ
//引数							なし
//戻り値						なし
/***********************************************************/
TReadRoku::TReadRoku()
{
	//マテリアル.
	m_Material			= NULL;
	//マテリアルの数.
	m_MaterialNum		= 0;
}

/***********************************************************/
//デストラクタ
//引数							なし
//戻り値						なし
/***********************************************************/
TReadRoku::~TReadRoku()
{
	Init();
}

/***********************************************************/
//初期化.
//引数							なし
//戻り値						なし
/***********************************************************/
void TReadRoku::Init()
{
	//マテリアル.
	if (m_Material) delete [] m_Material;
	m_Material			= NULL;
	//マテリアルの数.
	m_MaterialNum		= 0;
	//点コレクションの破棄.
	m_VecCaretaker.Clear();
	//線コレクションの破棄.
	m_LineCaretaker.Clear();
	//面コレクションの破棄.
	m_FaceCaretaker.Clear();
}

/***********************************************************/
//ファイルの読み込み.
//引数							inFileName	読み込むファイル名.
//戻り値						成功 true
/***********************************************************/
bool TReadRoku::Open(const char *inFileName)
{
	Init();

	FILE *		theFp;
	theFp = fopen(inFileName , "rb");
	if (theFp == NULL)	return false;

	//ファイルサイズを取得.
	::fseek( theFp , 0 , SEEK_END);
	m_FileDataLen = ::ftell(theFp);
	::fseek( theFp , 0 , SEEK_SET);

	//領域の確保.
	char *theFileData = new char[m_FileDataLen];

	//領域にデータを流し込む.
	::fread( theFileData , sizeof(char) , m_FileDataLen , theFp );

	::fclose( theFp );

	char* theReadPointer = theFileData;
		
	//ヘッダチェック.
	if ( ! CheckHeader(&theReadPointer) )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//点データの読み込み
	if ( ! GetPointes(&theReadPointer) )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//線データの読み込み.
	if ( ! GetLines(&theReadPointer) )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//面データの読み込み.
	if ( ! GetFaces(&theReadPointer) )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//マテリアルデータの読み込み.
	if ( ! GetMaterial(&theReadPointer) )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//なんか、もうファイルの中身入りません.
	delete theFileData;

	return true;
}

/***********************************************************/
//クローズ.
//引数							なし
//戻り値						成功 true
/***********************************************************/
bool TReadRoku::Close()
{
	Init();
	return true;
}

/***********************************************************/
//モデルの取得.
//引数							inObjectName	読み込むモデルの名前.
//								inModel			読み込ませるモデル.
//戻り値						成功 true
/***********************************************************/
bool TReadRoku::GetModel(const char* inObjectName , MODEL inModel)
{
	__RTI_CHECKER(m_Material != NULL);
	__RTI_CHECKER(inObjectName != NULL);

	//モデルが定義されていなかったら、ネームサーバーにそのモデルと
	//同じ名前を確保して、そこに格納します.
	MODEL theModel = inModel;
	if (theModel == NULL)
	{
		theModel = TLim3DMaster::NewModel();
		//ネームサーバーに追加する.
		TLim3DMaster::NameServer.Add( inObjectName , theModel );
	}
	//詳細な情報を取得します.
	if( !GetModelDetail(-1 , theModel) ) return false;

	return true;
}

/***********************************************************/
//モデルの詳細な情報を取得します.
//引数							inNo			グループ.
//								outModel		読み込ませるモデル.
//戻り値						なし
/***********************************************************/
bool TReadRoku::GetModelDetail(int inNo , MODEL outModel)
{
	__RTI_CHECKER(outModel != NULL);

	TRokuFace* theFace ;
	TRokuLine* theLine ;

	//各種戻り値.
	bool theRet;
	//ポリゴンデータの数.
	int theCount = m_FaceCaretaker.GetCount();
	//ポリゴンの格納データ.
	PolygonData  thePolygonData[4];

	//あらかじめ使うポリゴンと点の領域を確保.
	theRet = GetModelCompriseCache(inNo , outModel);
	if (!theRet) return true;

	XYZFixed theXYZ[2];
	int		 theVecNum = 0;
	for(int i = 0 ; i < theCount ; i++)
	{
		theFace = m_FaceCaretaker[i];
		//使用するラインデータの数.
		int theUseLineNum = theFace->FaceBlock;
		__RTI_CHECKER(theUseLineNum <= 4 && theUseLineNum >= 3);

		//色(マテリアル)データ.
		__RTI_CHECKER(theFace->ColorNum < m_MaterialNum && theFace->ColorNum >= 0);
		MATERIAL theMaterial = m_Material[ theFace->ColorNum ];

		XYZFixed	theVecData[4];
		//頂点情報の取得.
		for(int l = 0 ; l < theUseLineNum ; l++)
		{
			theLine = m_LineCaretaker[ theFace->LineNum[l]-1 ];
			theRet = SearchVec ( theLine->VecNum[0] , & (theXYZ[0])  );
			if (!theRet) return false;
			theRet = SearchVec ( theLine->VecNum[1] , & (theXYZ[1])  );
			if (!theRet) return false;

			//最初のデータ.
			if (l == 0)
			{// 0 と 1どっちをとるのかは、次の点が含まれているほう.
				XYZFixed	theNextVec[2];

				theLine = m_LineCaretaker[ theFace->LineNum[1]-1 ];
				theRet = SearchVec ( theLine->VecNum[0] , & (theNextVec[0])  );
				if (!theRet) return false;
				theRet = SearchVec ( theLine->VecNum[1] , & (theNextVec[1])  );
				if (!theRet) return false;

				if ( VectorCmp(&theXYZ[0] , &theNextVec[0] ) ||
					 VectorCmp(&theXYZ[0] , &theNextVec[1] ) )
									VectorSet(&theVecData[0]  , &theXYZ[0]);
				else if ( VectorCmp(&theXYZ[1] , &theNextVec[0] ) ||
						  VectorCmp(&theXYZ[1] , &theNextVec[1] ) )
								VectorSet(&theVecData[0]  , &theXYZ[1]);
				else	return false;
			}
			else
			{// 0 と 1どっちをとるのかは、前の点ではないほう.
				if ( VectorCmp(&theXYZ[0] , & theVecData[l-1] ) )
				{// 0 と前回が等しいので 1 をとります.
					VectorSet(&theVecData[l] , &theXYZ[1]);
				}
				else	if ( VectorCmp(&theXYZ[1] , & theVecData[l-1] ) )
				{// 1 と前回が等しいので 0 をとります.
					VectorSet(&theVecData[l] , &theXYZ[0]);
				}
				else return false;
			}
			//その頂点情報を記録(ポリゴンを作るときのつながりデータ).
			thePolygonData[l].Link = theVecNum++;
		}
		//頂点の追加.
		outModel->AddVec(theVecData , theUseLineNum);

		//ポリゴンの作成.
		POLYGON thePolygon = outModel->NewPolygon();
		if (thePolygon == NULL)
		{
			return false;
		}
		//つながりデータの設定.
		thePolygon->SetLink(thePolygonData , theUseLineNum);
		//シェーディング.
		thePolygon->SetShading(NONE_SHADING);
		//マテリアル.
		thePolygon->SetMaterial(theMaterial);

	}
	return true;
}

/***********************************************************/
//頂点を取得します.
//引数							inVecNo			取得したい頂点のナンバー.
//								outXYZ			その頂点の XYZ
//戻り値						成功 true.
/***********************************************************/
bool TReadRoku::SearchVec(int inVecNo , XYZFixed * outXYZ)
{
//	__RTI_CHECKER(inVecNo >= 0 && inVecNo < m_VecCaretaker.GetCount() );
	__RTI_CHECKER(outXYZ != NULL);

	TRokuVec * theVec ;
	int        theVecCount = m_VecCaretaker.GetCount();

	for(int i = 0; i < theVecCount ; i++)
	{
		theVec = m_VecCaretaker[i];
		//DEBUG3("%d		%d", theVec->No, inVecNo);
		if ( theVec->No == inVecNo)
		{
			outXYZ->x = theVec->x;
			outXYZ->y = theVec->y;
			outXYZ->z = theVec->z;
			return true;
		}
	}
	//見つからない.
	return false;
}

/***********************************************************/
//使用するポリゴンの数と頂点を算出します.
//引数							inNo			グループ.
//								outModel		読み込ませるモデル.
//戻り値						ポリゴンと頂点が少なくとも 1 個以上はある.
/***********************************************************/
bool TReadRoku::GetModelCompriseCache(int inNo ,  MODEL outModel)
{
	//ポリゴンデータの数.
	int theCount = m_FaceCaretaker.GetCount();

	int thePolygonNum = 0;
	int theVecNum = 0;

	for(int i = 0 ; i < theCount ; i++)
	{
		TRokuFace* theFace = m_FaceCaretaker[i];

		//使用するポリゴンの数を増やす.
		thePolygonNum++;
		//使用するラインデータの数.
		theVecNum += theFace->FaceBlock;
	}
	//ポリゴンまたは、頂点が含まれていない.
	if (thePolygonNum <= 0 || theVecNum <= 0) return false;
	//頂点領域の一括確保.
	outModel->GetVecCaretaker()->Alloc(theVecNum);
	//ポリゴン領域の一括確保.
	//outModel->GetPolygonCaretaker()->Alloc(thePolygonNum);

	return true;
}


/***********************************************************/
//ヘッダチェック.
//引数							なし
//戻り値						成功 true
/***********************************************************/
bool TReadRoku::CheckHeader(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);

	char	theFormatType[7];
	/*
	ROKU4               //　六角大王データの始まり
		-0.785398163    //　Rmatobj pp  （技術解説プログラム参照、
	    -0.314159265    //　Rmatobj pq　　大きさ1.0x1.0の仮想画面への変換行列）
		 0.000000000    //  Rmatobj pr
	     0.000000000    //  Rmatobj dx
		 0.000000000    //  Rmatobj dy
	     1.500000000    //  Rmatobj dz
		 1.000000000    //  Rmatobj bx
	     1.000000000    //  Rmatobj by
		1               //　Rmatobj pers
	    0               //　0：斜め向き　1：YZ平面の正面　2：同 ZX平面  3：同XY平面
		368.750000000    //　画面上座標でのＸ方向の拡大率（Rmatobj で得た座標を
		368.750000000    //　画面上座標でのＹ方向の拡大率　拡大､平行移動して画面表示
		217.000000000    //　画面上座標でのＸ方向の移動量　するときの、拡大/移動量）
		147.000000000    //　画面上座標でのＹ方向の移動量
	*/
	::sscanf( *ioReadPointer ,	"%5s\r",theFormatType );
	//六角大王形式でなければ没.
	if ( stricmp(theFormatType,"ROKU4") != 0 ) return false;

	//OK.
	return true;
}

//次の行へ.
char* TReadRoku::NextLine(char * inLine)
{
	//改行まで
	char* theNextLine = ::strstr(inLine , "\r");
	if (theNextLine == NULL) return NULL;

	//改行のサイズプラス.
	theNextLine += sizeof("\r");

	return theNextLine;
}

//指定した数、次の行に進む.
char* TReadRoku::NextLine(char * inLine , int inSkipLines)
{
	char* theNextLine = inLine;
	int	  theLine = inSkipLines;
	
	for( ; theLine > 0 ; theLine--)
	{
		theNextLine = NextLine(theNextLine);
		if (theNextLine == NULL) return NULL;
	}
	return theNextLine;
}

//点データの読み込み.
bool TReadRoku::GetPointes(char ** ioReadPointer) 
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//POINTタグを探す.
	char * thePointString = ::strstr( *ioReadPointer , "POINT" );
	if (thePointString == NULL) return false;
	//次のLINE0タグ.
	char * theLineString = ::strstr( *ioReadPointer , "LINE0" );
	if (theLineString == NULL) return false;

	//POINT の次の行へ.
	thePointString = NextLine(thePointString);
	if (thePointString == NULL) return false;

	/*
	    1               //　点の番号
		0               //　グループ化した時のグループ番号（グループは階層化不可）
	    0               //　０なら表示。１なら隠す。
		0               //　ダミー
	    2               //　０なら２次元の点。０以外なら左右対称な点の番号。
		-0.354941037    //　点のＸ座標
	     0.270283061    //　点のＹ座標
		 0.095652076    //　点のＺ座標
	*/
	TRokuVec	theRokuVec;
	int			theIntDummy;
	float		theX,theY,theZ;

	m_VecCaretaker.Alloc(10000);
	//ラインタグになるまで、繰り返します.
	for( ; theLineString > thePointString; )
	{
		sscanf(thePointString , "	%d\r	%d\r	%d\r"
								"	%d\r	%d\r"
								"	%f\r	%f\r	%f\r" ,
				&(theRokuVec.No)	,	&(theRokuVec.GroupNum),	&(theRokuVec.IsHide),
				&(theIntDummy)		,	&(theRokuVec.SymmetricalNum),
				&theX				,	&theY		,	&theZ	);
		//大きさの変更.
		theRokuVec.x = Float2Fixed(theX * 64   );
		theRokuVec.y = Float2Fixed(theY * -64   );
		theRokuVec.z = Float2Fixed(theZ * 64  );
		//コレクションに登録.
		m_VecCaretaker.Add(&theRokuVec,1);

		//8行したへ. 
		thePointString = NextLine(thePointString,8);
		if (thePointString == NULL) return false;
	}
	//次のデータへ.
	*ioReadPointer = theLineString;

	return true;
}

//線データの読み込み.
bool TReadRoku::GetLines(char ** ioReadPointer) 
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//LINE0タグを探す.
	char * theLineString = ::strstr( *ioReadPointer , "LINE0" );
	if (theLineString == NULL) return false;
	//次のFACE0タグ.
	char * theFaceString = ::strstr( *ioReadPointer , "FACE0" );
	if (theFaceString == NULL) return false;

	//LINE0 の次の行へ.
	theLineString = NextLine(theLineString);
	if (theLineString == NULL) return false;

	TRokuLine	theRokuLine;
	/*
	    3               //　つながる点（その１）の番号
		1               //　つながる点（その２）の番号
	    0               //　グループの番号（０の時はグループ化されていない）
		0               //　０のとき表示　１のとき隠す
	*/
	m_LineCaretaker.Alloc(5000);
	//フェィスタグになるまで、繰り返します.
	for( ; theFaceString > theLineString ; )
	{
		sscanf(theLineString , "	%d\r	%d\r	%d\r	%d\r"	,
				&(theRokuLine.VecNum[0])	,	&(theRokuLine.VecNum[1]),
				&(theRokuLine.GroupNum)		,	&(theRokuLine.SymmetricalNum)	);
		//コレクションに追加.
		m_LineCaretaker.Add(&theRokuLine,1);
		//4行したへ. 
		theLineString = NextLine(theLineString,4);
		if (theLineString == NULL) return false;
	}
	*ioReadPointer = theFaceString;
	return true;
}

//面データの読み込み.
bool TReadRoku::GetFaces(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//FACE0タグを探す.
	char * theFaceString = ::strstr( *ioReadPointer , "FACE0" );
	if (theFaceString == NULL) return false;
	//次のEND00タグ.
	char * theEndString = ::strstr( *ioReadPointer , "END00" );
	if (theEndString == NULL) return false;

	//FACE0 の次の行へ.
	theFaceString = NextLine(theFaceString);
	if (theFaceString == NULL) return false;

	TRokuFace	theRokuFace;
	/*
	    3               //　面の画数
		2               //　色の番号
	    1               //　面の番号
	---------------
		5               //　面の輪郭線その１の番号
	    3               //　面の輪郭線その２の番号
		4               //　面の輪郭線その３の番号
	*/
	m_FaceCaretaker.Alloc(1500);
	//エンドタグになるまで、繰り返します.
	for( ; theEndString > theFaceString ; )
	{
		sscanf(theFaceString , "	%d\r	%d\r	%d\r"	,
			&(theRokuFace.FaceBlock)	,	&(theRokuFace.ColorNum),	&(theRokuFace.No)	);
		//3行したへ. 
		theFaceString = NextLine(theFaceString,3);
		if (theFaceString == NULL) return false;

		//面を構成する数だけ線ナンバーを読み込みます.
		switch (theRokuFace.FaceBlock)
		{
		case 3:
			sscanf(theFaceString , "	%d\r	%d\r	%d\r"	,
				&(theRokuFace.LineNum[0])	,	&(theRokuFace.LineNum[1]),
				&(theRokuFace.LineNum[2])	);
			break;
		case 4:
			sscanf(theFaceString , "	%d\r	%d\r	%d\r	%d\r"	,
				&(theRokuFace.LineNum[0])	,	&(theRokuFace.LineNum[1]),
				&(theRokuFace.LineNum[2])	,	&(theRokuFace.LineNum[3])	);
			break;
		default:
			return false;
		}

		//theRokuFace.FaceBlock行したへ. 
		theFaceString = NextLine(theFaceString, theRokuFace.FaceBlock );
		if (theFaceString == NULL) return false;

		//コレクションに追加.
		m_FaceCaretaker.Add(&theRokuFace,1);
	}
	*ioReadPointer = theEndString;

	return true;
}

//色の取得(マテリアル)
bool TReadRoku::GetMaterial(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);
	__RTI_CHECKER(m_Material == NULL);
	//PALC0タグを探す.
	char * thePalString = ::strstr( *ioReadPointer , "PALC0" );
	if (thePalString == NULL) return false;
	//次のENDP0タグ.
	char * theEndPalString = ::strstr( *ioReadPointer , "ENDP0" );
	if (theEndPalString == NULL) return false;

	//PALC0 の次の行へ.
	thePalString = NextLine(thePalString);
	if (thePalString == NULL) return false;

	/*
		13                  //　色の数（現在は13のみサポート）
		0                   //　選択色（現在はダミー）
	*/
	int			theColorMax;
	int			theIntDummy;
	sscanf(thePalString , "	%d\r	%d\r", &theColorMax , &theIntDummy);
	//パレットデータへ 2行読み飛ばす.
	thePalString = NextLine(thePalString , 2);
	if (thePalString == NULL) return false;

	//色の最大数が マイナスだったら没.
	if (theColorMax < 0) return false;
	//領域確保.
	m_Material = new MATERIAL[theColorMax];
	if (m_Material == NULL) return false;
	m_MaterialNum = theColorMax;

	/*
		255                 //　１番目の色：明るい所の色(R) (0～255)
		255                 //　１番目の色：明るい所の色(G)
		255                 //　１番目の色：明るい所の色(B)
		128                 //　１番目の色：暗い所の色(R)
		128                 //　１番目の色：暗い所の色(G)
		128                 //　１番目の色：暗い所の色(B)
		0                   //　予約
	*/
	//フェィスタグになるまで、繰り返します.
	for( ; theColorMax > 0 ; theColorMax--)
	{
		int	theLightR,theLightG,theLightB,theDarkR,theDarkG,theDarkB;
		MATERIAL* theMaterial = & m_Material[m_MaterialNum - theColorMax];

		sscanf(thePalString ,	"	%d\r	%d\r	%d\r"
								"	%d\r	%d\r	%d\r"
								"	%d\r",
						&theLightR , &theLightG , &theLightB ,
						&theDarkR  , &theDarkG  , &theDarkB  ,
						&theIntDummy);

		//とりあえず、マスターにマテリアルの作成を申請します.
		*theMaterial = TLim3DMaster::NewMaterial();
		if (*theMaterial == NULL) return false;
		//色の設定. 明るい色を採用します(手抜き).
		(*theMaterial)->SetColor( RTIRGB(theLightR , theLightG , theLightB) );
		//(*theMaterial)->SetColor( RGB(255 , 255 , 255) );

		//7行したへ. 
		thePalString = NextLine(thePalString,7);
		if (thePalString == NULL) return false;
	}
	*ioReadPointer = thePalString;

	return true;
}
