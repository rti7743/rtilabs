#include <./Lim3D/3DRead/rti_Lim3DMetasequoia.h>
#include <stdio.h>

/***********************************************************/
//コンストラクタ
//引数							なし
//戻り値						なし
/***********************************************************/
TReadMetasequoia::TReadMetasequoia()
{
	//マテリアル.
	m_Material			= NULL;
	//マテリアルの数.
	m_MaterialNum		= 0;
	//ファイルから読み込んだデータ.
	m_FileData			= NULL;
	//データの長さ.
	m_FileDataLen		= 0;
}

/***********************************************************/
//デストラクタ
//引数							なし
//戻り値						なし
/***********************************************************/
TReadMetasequoia::~TReadMetasequoia()
{
	Init();
}

/***********************************************************/
//初期化.
//引数							なし
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::Init()
{
	//マテリアル.
	if (m_Material) delete [] m_Material;
	m_Material			= NULL;
	//ファイルから読み込んだデータ.
	if (m_FileData) delete [] m_FileData;
	m_FileData			= NULL;
	//データの長さ.
	m_FileDataLen		= 0;
	//マテリアルの数.
	m_MaterialNum		= 0;
}

/***********************************************************/
//ファイルの読み込み.
//引数							inFileName	読み込むファイル名.
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::Open(const char *inFileName)
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
	m_FileData = new char[m_FileDataLen];

	//領域にデータを流し込む.
	::fread( m_FileData , sizeof(char) , m_FileDataLen , theFp );

	::fclose( theFp );
	
	//ヘッダチェック.
	if ( ! CheckHeader() )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	//マテリアル取得.
	if ( ! GetMaterial() )
	{//おかしいデータなので、終了.
		Init();
		return false;
	}
	return true;
}

/***********************************************************/
//クローズ.
//引数							なし
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::Close()
{
	Init();
	return true;
}


/***********************************************************/
//ヘッダチェック.
//引数							なし
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::CheckHeader()
{
	__RTI_CHECKER(m_FileData != NULL);

	char	theFormatType[10];
	float	theVer;

	::sscanf(m_FileData , "Metasequoia Document\r\nFormat %s Ver %f" 
												,theFormatType , &theVer );
	//フォーマット形式がテキストで無ければ没.
	if ( stricmp(theFormatType,"Text") != 0 ) return false;
	//バージョンが 1.0未満は没.
	if ( theVer < 1.0 ) return false;

	//OK.
	return true;
}

/***********************************************************/
//モデルの取得.
//引数							inObjectName	読み込むモデルの名前.
//								inModel			読み込ませるモデル.
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::GetModel(const char* inObjectName , MODEL inModel)
{
	__RTI_CHECKER(m_FileData != NULL);
	__RTI_CHECKER(m_Material != NULL);
	__RTI_CHECKER(inObjectName != NULL);

	//現在処理している場所.
	char *theNowProc = m_FileData;
	//オブジェクトの名前.
	char theModelName[64];

	while( 1 )
	{
		//モデルタグを探す.
		theNowProc = ::strstr( theNowProc , "Object " );
		//タグ見つからない.
		if ( theNowProc == NULL)	return false;
		//オブジェクトが、指定されたオブジェクトと等しいかチェック.
		::sscanf( theNowProc , "Object \"%63[^\"]\" {" , theModelName);
		//それが等しいか比べる、等しいならループを抜ける.
		if ( strcmp(inObjectName , theModelName) == 0)	break;
		//等しくないので、次のオブジェクトいくけど、 strstr を成功させるために +1で
		//文字列のポインタをずらします.
		theNowProc += 1;
	}
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
	GetModelDetail(theNowProc , theModel);

	return true;
}
/***********************************************************/
//すべてのモデルの取得.
//引数							なし
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::GetAllModel()
{
	__RTI_CHECKER(m_FileData != NULL);
	__RTI_CHECKER(m_Material != NULL);

	//現在処理している場所.
	char *theNowProc = m_FileData;
	//オブジェクトの名前.
	char theModelName[64];
	//一時モデル.
	MODEL	theDummyModel;

	while( 1 )
	{
		//モデルタグを探す.
		theNowProc = ::strstr( theNowProc , "Object " );
		//タグ見つからない.
		if ( theNowProc == NULL)	return false;
		//オブジェクトが、指定されたオブジェクトと等しいかチェック.
		::sscanf( theNowProc , "Object \"%63[^\"]\" {" , theModelName);

		//ネームサーバーにそのモデルと同じ名前を確保して、そこに格納します.
		theDummyModel = TLim3DMaster::NewModel();
		//ネームサーバーに追加する.
		TLim3DMaster::NameServer.Add( theModelName , theDummyModel );
		//詳細な情報を取得します.
		GetModelDetail(theNowProc , theDummyModel);

		//次のオブジェクトいくけど、 strstr を成功させるために +1で
		//文字列のポインタをずらします.
		theNowProc += 1;
	}

	return true;
}

/***********************************************************/
//モデルの詳細な情報を取得します.
//引数							inObjectData	モデルタグの内容.
//								outModel		読み込ませるモデル.
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetModelDetail(const char* inObjectData , MODEL outModel)
{
	__RTI_CHECKER(inObjectData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//ここで、 現在の オブジェクトは処理すべきオブジェクトになっています.
	char *	theTargetPos;
	//頂点情報方法のサーチ.
	theTargetPos = ::strstr(inObjectData , "vertex ");
	if (theTargetPos != NULL) GetModelDetailVertex(theTargetPos , outModel);
	//面情報方法のサーチ.
	theTargetPos = ::strstr(inObjectData , "face ");
	if (theTargetPos != NULL) GetModelDetailFace(theTargetPos , outModel);
	//シェーディング方法のサーチ.
	theTargetPos = ::strstr(inObjectData , "shading ");
	if (theTargetPos != NULL) GetModelDetailShading(theTargetPos , outModel);

	return;
}

/***********************************************************/
//シェーディング情報の取得.
//引数							inTagData		シェーディングの内容.
//								outModel		読み込ませるモデル.
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetModelDetailShading(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//取得するシェーディング.
	int theShading = 0;
	::sscanf(inTagData , " shading %d" , &theShading);

	//Lim3D ライブラリで使うシェーディング.
	ShadingLevel thePolygonShading ;
	switch(theShading)
	{
	case 0:
		thePolygonShading = FLAT_SHADING;		break;
	case 1:
		thePolygonShading = GROW_SHADING;		break;
	default:
		thePolygonShading = NONE_SHADING;		break;
	}
	
	//すべてのポリゴンにアクセスして､シェーディングレベルの設定.
	TLim3DPolygon* thePoly = outModel->FindFirstPolygon();
	if (thePoly == NULL)	return ;
	do
	{
		thePoly->SetShading(thePolygonShading);
	}
	while( thePoly = outModel->FindNextPolygon() );
}

/***********************************************************/
//頂点情報の取得.
//引数							inTagData		頂点の内容.
//								outModel		読み込ませるモデル.
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetModelDetailVertex(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//取得する頂点情報.
	int theVertex = 0;
	sscanf(inTagData , " vertex %d {" , &theVertex);
	if (theVertex <= 0) return ;
	
	//現在処理しているところ.
	char * theNowProc = (char*) inTagData;
	//この数だけの頂点を入れます.
	XYZFixed * theVertexBuffer = new XYZFixed[theVertex];
	if (theVertexBuffer == NULL) return;

	int theI;
	for(theI = 0 ; theI < theVertex ; theI++)
	{
		//改行まで読み飛ばす.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL)
		{
			delete [] theVertexBuffer;
			return ;
		}
		//改行コードを飛ばします.
		theNowProc += sizeof("\r\n");
		//その次の行には頂点情報があるはずなので、読み込みます.
		GetVertexCaseByCase(  theNowProc , &theVertexBuffer[theI]);
	}
	//その頂点の追加.
	outModel->SetVec( theVertexBuffer , theVertex );
	//不要な作業バッファのクリア.
	delete [] theVertexBuffer;
}

/***********************************************************/
//その次の行には頂点情報があるはずなので、読み込みます.
//引数							inVertexData	頂点の内容.
//								outVertex		頂点を格納するバッファ.
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetVertexCaseByCase
						(const char* inVertexData , XYZFixed * outVertex)
{
	__RTI_CHECKER(inVertexData != NULL);
	__RTI_CHECKER(outVertex != NULL);

	float theX,theY,theZ;
	::sscanf(inVertexData , " %f %f %f" , &theX, &theY, &theZ);

	outVertex->x = Float2Fixed(theX);
	outVertex->y = Float2Fixed(theY);
	outVertex->z = Float2Fixed(theZ);
}

/***********************************************************/
//面情報の取得.
//引数							inTagData		面の内容.
//								outModel		読み込ませるモデル.
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetModelDetailFace(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//取得する面情報.
	int theFace = 0;
	::sscanf(inTagData , " face %d {" , &theFace);
	if (theFace <= 0) return ;
	
	//現在処理しているところ.
	char * theNowProc = (char*) inTagData;

	int theI;
	for(theI = 0 ; theI < theFace ; theI++)
	{
		//改行まで読み飛ばす.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL)		return ;
		//改行コードを飛ばします.
		theNowProc += sizeof("\r\n");
		//その次の行には面情報があるはずなので、読み込みます.
		bool theRet = GetFaceCaseByCase(  theNowProc , outModel);
		if (theRet == false) return ;
	}
}

/***********************************************************/
//面情報の取得.
//引数							inFaceData		1面の内容.
//								outModel		読み込ませるモデル.
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::GetFaceCaseByCase(const char* inFaceData , MODEL outModel)
{
	__RTI_CHECKER(inFaceData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//頂点数.
	int theVertexCount;
	::sscanf(inFaceData , " %d " , &theVertexCount);
	//頂点数がいかれていたら没.
	if (theVertexCount < 3 || theVertexCount > 4)	return false;

	//リンクなデータ.
	int			theLink[4];
	//マテリアルナンバー
	int			theMaterial = 0;
	//UV
	float		theUV[4*2];

	if (theVertexCount == 3)
	{//三角形.
		::sscanf(inFaceData , " %d V(%d %d %d) M(%d) UV(%f %f %f %f %f %f)" , 
			&theVertexCount , &theLink[2] , &theLink[1] , &theLink[0] ,
			&theMaterial , 
			&theUV[4] , &theUV[5] , &theUV[2] , &theUV[3] , &theUV[0] , &theUV[1] );
		//値のセット.
		GetFaceCaseByCasePolygonCreate( outModel , 3 , theLink , theMaterial , theUV );
	}
	else
	{//四角形.
		::sscanf(inFaceData , " %d V(%d %d %d %d) M(%d) UV(%f %f %f %f %f %f %f %f)" ,
			&theVertexCount , &theLink[3] , &theLink[2] , &theLink[1] , &theLink[0] ,
			&theMaterial , 
			&theUV[6] , &theUV[7] , &theUV[4] , &theUV[5] ,
			&theUV[2] , &theUV[3] , &theUV[0] , &theUV[1]);
		//値のセット.
		GetFaceCaseByCasePolygonCreate( outModel , 4 , theLink , theMaterial , theUV );
	}
	return true;
}

/***********************************************************/
//そのつながりデータより、ポリゴンを作成する.
//引数							outModel		読み込ませるモデル.
//								inNum			頂点の数 3 or 4.
//								inLink			つながり
//								inMaterial		マテリアル
//								inUV			テクスチャのUV
//戻り値						なし
/***********************************************************/
void TReadMetasequoia::GetFaceCaseByCasePolygonCreate
				(MODEL outModel , int inNum ,
				 const int* inLink , int inMaterial , const float* inUV)
{
	__RTI_CHECKER(outModel != NULL);
	__RTI_CHECKER(inMaterial >= 0);
	__RTI_CHECKER(inMaterial <  m_MaterialNum);

	//ポリゴンの頂点のつながりをあらわすデータ.
	PolygonData thePolygonData[4];

	//テクスチャー.
	BMD theBmd = m_Material[inMaterial]->GetTexture();

	int theI ;
	for(theI = 0 ; theI < inNum ; theI++)
	{
		//頂点のつながり.
		thePolygonData[theI].Link = inLink[theI];
		//テクスチャーがあるとき.
		if (theBmd != NULL)
		{
			//頂点のテクスチャーの UV.
			thePolygonData[theI].Texture.x = ( inUV[theI*2 + 0] * theBmd->w);
			thePolygonData[theI].Texture.y = ( inUV[theI*2 + 1] * theBmd->h);
		}
	}
	//新規ポリゴン作成.
	TLim3DPolygon* thePolygon = outModel->NewPolygon();
	//そのポリゴンにさっきのデータをくわえて完成.
	thePolygon->SetLink( thePolygonData , inNum);
	//そのポリゴンとマテリアルを関連付けます.
	thePolygon->SetMaterial( m_Material[inMaterial] );
}


/***********************************************************/
//マテリアル取得.
//引数							なし.
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::GetMaterial()
{
	__RTI_CHECKER(m_FileData != NULL);

	//現在処理している場所.
	char *theNowProc;

	//マテリアルタグを探す.
	theNowProc = ::strstr( m_FileData , "Material" );
	//タグ見つからない.
	if ( theNowProc == NULL)	return false;

	//マテリアルタグにある、マテリアル個数を取得.
	m_MaterialNum = 0;
	sscanf( theNowProc , "Material %d {" , &m_MaterialNum);
	if (m_MaterialNum <= 0) return false;
	//マテリアル領域の確保.
	m_Material = new MATERIAL[m_MaterialNum];
	if (m_Material == NULL)		return false;

	int MaterialDataNum = 0;
	for(MaterialDataNum = 0 ; MaterialDataNum < m_MaterialNum ; MaterialDataNum++)
	{
		//改行まで読み飛ばす.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL) return false;
		//改行コードを飛ばします.
		theNowProc += sizeof("\r\n");
		//その次の行にはマテリアルがあるはずなので、読み込みます.
		bool theRet = GetMaterialCaseByCase
						(  theNowProc , &m_Material[MaterialDataNum]);
		//マテリアル取得中のエラー.
		if (theRet == false)		return false;
	}
	return true;	
}

/***********************************************************/
//1マテリアル取得.
//引数							inMaterialData		1マテリアルのデータ.
//								outMaterial			読み込むマテリアル.
//戻り値						成功 true
/***********************************************************/
bool TReadMetasequoia::GetMaterialCaseByCase
					(const char* inMaterialData , MATERIAL* outMaterial)
{
	char	theMaterialName[32]={0};		//材質名                 31Byte以内
	float	theR=0,theG=0,theB=0,theTrans=0;//色（ＲＧＢ）、不透明度 それぞれ0～1
	float	theDif=0;						//拡散光                 0～1
	float	theAmb=0;						//周囲光                 0～1
	float	theEmi=0;						//自己照明               0～1
	float	theSpc=0;						//反射光                 0～1
	float	thePow=0;						//反射光の強さ           0～100
	char	theTexName[64]={0};				//模様マッピング名       相対パスで記述（63Byte以内）
	char	theAlphaName[64]={0};			//透明マッピング名       相対パスで記述（63Byte以内)
	char	theBumpName[64]={0};			//凹凸マッピング名       相対パスで記述（63Byte以内）
	//こんな感じです.
	//	"mat1" col(1.000 0.275 0.459 1.000) dif(1.000) amb(0.250) emi(0.250) spc(0.000) power(5.00)
	::sscanf(inMaterialData , "	\"%31[^\"]\" " \
			"col(%f %f %f %f) dif(%f) amb(%f) emi(%f) spc(%f) power(%f) " \
			"tex(\"%63[^\"]\") alpha(\"%63[^\"]\") bump(\"%63[^\"]\")",
			theMaterialName , &theR , &theG , &theB , &theTrans,
			&theDif , &theAmb , &theEmi , &theSpc , &thePow ,
			theTexName , theAlphaName , theBumpName			);

	//とりあえず、マスターにマテリアルの作成を申請します.
	*outMaterial = TLim3DMaster::NewMaterial();
	if (*outMaterial == NULL) return false;
	//いちいち、アスタリクスつけるのはだるいので...
	MATERIAL theMaterial = *outMaterial;

	//そのマテリアルを構築します.
	//色の決定.
	theMaterial->SetColor( ClasColor( theR , theG , theB , theEmi) );
	//ディフューズ.
	theMaterial->SetDiffuseReflexFactor( Float2Fixed(theDif) );
	//アンビエント.
	theMaterial->SetAmbientReflexFactor( Float2Fixed(theAmb) );
	//スペキャラー
	theMaterial->SetSpecularReflexFactor( Float2Fixed(theSpc) );
	//スペキャラー(反射光の強さ)
	theMaterial->SetVelvetyReflexFactor( Float2Fixed(thePow) );
	//テクスチャーマッピング.
	theMaterial->SetTexture( ClasTexture(theTexName) );
	//で、テクスチャーは結局使うの ? 
	if ( theMaterial->GetTexture() == NULL)
	{	//No テクスチャー.
		theMaterial->SetDrawing(BETA_DRAW);
	}
	else
	{	//Use テクスチャー.
		theMaterial->SetDrawing(TEXTURE_DRAW);
	}
	//ライティングモデルの決定.
	theMaterial->SetLighting
				( ClasLighting( theDif , theAmb , theSpc , thePow) );
	//ネームサーバーに追加する.
	TLim3DMaster::NameServer.Add( theMaterialName , theMaterial );
	//成功.
	return true;
}


/***********************************************************/
//カラーを取得.
//引数							inR					赤.
//								inG					緑.
//								inB					青.
//								inEei				自分自身の色
//戻り値						色.
/***********************************************************/
DWORD TReadMetasequoia::ClasColor( float inR , float inG , float inB , float inEmi)
{
	int theR = (int)(inR * inEmi * 255);
	int theG = (int)(inG * inEmi * 255);
	int theB = (int)(inB * inEmi * 255);

	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	return (theR << 16) + (theG << 8) + (theB);
}

/***********************************************************/
//テクスチャーマッピングをやるの ? やる場合は、その テクスチャーの BMDを返す.
//引数							inTextureName		テクスチャーのパス.
//戻り値						BMD
/***********************************************************/
BMD TReadMetasequoia::ClasTexture(const char * inTextureName)
{
	//ファイル名そのものがない.
	if (inTextureName[0] == '\0') return NULL;
	//マスターのテクスチャーを使う.
	return TLim3DMaster::NewTexture(inTextureName);
}

/***********************************************************/
//ライティング効果の設定.
//引数							inDif
//								inAmb
//								inSpc
//								inPow
//戻り値						ライティングレベル.
/***********************************************************/
LightLevel TReadMetasequoia::ClasLighting(float inDif ,float inAmb ,float inSpc ,float inPow)
{
	return LAMBERT_LIGHTING;
}


