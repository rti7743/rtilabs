#include <./Lim3D/rti_Lim3DMaster.h>

#define __ROKU_FACE_LINE_MAX	4	//面を構築する線の最大値.


class TReadRoku
{
	struct TRokuVec
	{
		int		No;				//  点番号.
		int		GroupNum;		//　グループ化した時のグループ番号（グループは階層化不可）
		int		IsHide;			//　０なら表示。１なら隠す。
		int		SymmetricalNum;	//　０なら２次元の点。０以外なら左右対称な点の番号。
		Fixed	x,y,z;			//	座標
	};

	struct TRokuLine
	{
		int		VecNum[2];		//　つながる点（その1,2）の番号
		int		GroupNum;		//　グループ化した時のグループ番号（グループは階層化不可）
		int		SymmetricalNum;	//　０なら２次元の点。０以外なら左右対称な点の番号。
	};

	struct TRokuFace
	{
		int		FaceBlock;		//　面の画数
		int		ColorNum;		//　色の番号
		int		No;				//  面番号.
		int		LineNum[__ROKU_FACE_LINE_MAX];		//  面番号
	};
public:
	//コンストラクタ
	TReadRoku();
	//デストラクタ
	~TReadRoku();

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
	bool	CheckHeader(char ** ioReadPointer);
	//次の行へ.
	char*	NextLine(char * inLine);
	//指定した数、次の行に進む.
	char*	NextLine(char * inLine , int inSkipLines);
	//点を取得する.
	bool	GetPointes(char ** ioReadPointer);
	//線を取得する.
	bool	GetLines(char ** ioReadPointer);
	//面を取得する.
	bool	GetFaces(char ** ioReadPointer);
	//色の取得(マテリアル)
	bool	GetMaterial(char ** ioReadPointer);

	//使用するポリゴンの数と頂点を算出します.
	bool	GetModelCompriseCache(int inNo ,  MODEL outModel);
	//モデルの詳細な情報を取得します.
	bool	GetModelDetail(int inNo , MODEL outModel);
	//頂点を取得します.
	bool	SearchVec(int inVecNo , XYZFixed * outXYZ);

private:
	//マテリアル.
	MATERIAL*	m_Material;
	//マテリアルの数.
	int			m_MaterialNum;
	//データの長さ.
	int			m_FileDataLen;
	//頂点情報.
	_TCaretaker<TRokuVec>	m_VecCaretaker;
	//線情報.
	_TCaretaker<TRokuLine>	m_LineCaretaker;
	//面情報.
	_TCaretaker<TRokuFace>	m_FaceCaretaker;
};
