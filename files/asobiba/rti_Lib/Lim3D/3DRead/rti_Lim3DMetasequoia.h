#include <./Lim3D/rti_Lim3DMaster.h>

class TReadMetasequoia
{
public:
	//コンストラクタ
	TReadMetasequoia();
	//デストラクタ
	~TReadMetasequoia();

public:
	//ファイルの読み込み.
	bool Open(const char *inFileName);
	//クローズ.
	bool Close();
	//オブジェクトを読み込み.
	bool GetModel(const char* inOnjectName , MODEL outModel);
	//すべてのモデルの取得.
	bool GetAllModel();

	//初期化.
	void Init();

private:
	//ヘッダチェック.
	bool CheckHeader();
	//マテリアル取得.
	bool GetMaterial();
	//マテリアル取得.
	bool GetMaterialCaseByCase(const char* inMaterialData , MATERIAL* outMaterial);
	//カラーを取得.
	DWORD ClasColor( float inR , float inG , float inB , float inEmi);
	//テクスチャーマッピングをやるの ? やる場合は、その テクスチャーの BMDを返す.
	BMD ClasTexture(const char * inTextureName);
	//ライティング効果の設定.
	LightLevel ClasLighting(float inDif ,float inAmb ,float inSpc ,float inPow);

	//モデルの詳細な情報を取得します.
	void GetModelDetail(const char* inObjectData , MODEL outModel);
	//シェーディング情報の取得.
	void GetModelDetailShading(const char* inTagData , MODEL outModel);
	//頂点情報の取得.
	void GetModelDetailVertex(const char* inTagData , MODEL outModel);
	//その次の行には頂点情報があるはずなので、読み込みます.
	void GetVertexCaseByCase(const char* inVertexData , XYZFixed * outVertex);
	//面情報の取得.
	void GetModelDetailFace(const char* inTagData , MODEL outModel);
	//面情報の取得.
	bool GetFaceCaseByCase(const char* inFaceData , MODEL outModel);
	//そのつながりデータより、ポリゴンを作成する.
	void GetFaceCaseByCasePolygonCreate
						(MODEL outModel , int inNum , 
						 const int* inLink , int inMaterial , const float* inUV);



private:
	//マテリアル.
	MATERIAL*	m_Material;
	//マテリアルの数.
	int			m_MaterialNum;
	//ファイルから読み込んだデータ.
	char*		m_FileData;
	//データの長さ.
	int			m_FileDataLen;
};
