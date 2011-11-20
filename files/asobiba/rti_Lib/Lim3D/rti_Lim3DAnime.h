#ifndef	___LIM3D_ANIME__H	//重複防止
#define ___LIM3D_ANIME__H

#include <./Lim3D/rti_Lim3DModel.h>
#include <./Lim3D/rti_Lim3DEye.h>
#include <./Lim3D/rti_Lim3DLight.h>
#include <vector>

class TLim3DAnimeBase
{
public:
	//実行.
	virtual void				Action() = 0;
protected:
};

class TLim3DAnimeFream;
typedef bool (*THIS_ANALYSIS)
	(TLim3DAnimeFream * inFream , int inFreamTime,const char * inStart , const char * inEnd);


//モデルのアニメーション.
class TLim3DAnimeModel : public TLim3DAnimeBase
{
public:
	TLim3DAnimeModel()
	{
		m_Model = NULL;
	}
	//データのセット.
	void		DataSet(int inFream,Fixed inX , Fixed inY , Fixed inZ ,
									Angle inRX , Angle inRY , Angle inRZ )
	{
		__RTI_CHECKER(inFream > 0);

		m_X = inX / inFream;
		m_Y = inY / inFream;
		m_Z = inZ / inFream;
		m_RX = inRX / inFream;
		m_RY = inRY / inFream;
		m_RZ = inRZ / inFream;
	}
	//実行.
	virtual void	Action()
	{
		__RTI_CHECKER(m_Model !=  NULL);
		__RTI_CHECKER(*m_Model !=  NULL);

		(*m_Model)->AddPos(m_X  , m_Y  , m_Z);
		(*m_Model)->AddRotate(m_RX  , m_RY  , m_RZ);
	}
	//解析.
	static bool ThisAnalysis
		(TLim3DAnimeFream * inFream , int inFreamTime,const char * inStart , const char * inEnd);

private:
	Fixed						m_X  , m_Y  , m_Z;
	Angle						m_RX , m_RY , m_RZ;
	TLim3DModel**				m_Model;
};


class TLim3DAnimeFream
{
public:
	TLim3DAnimeFream(){}
	~TLim3DAnimeFream(){ Clear(); }
public:
	//各種アニメーション単位を分解します.
	bool	BaseSplit(int inFreamTime,const char * inStart , const char * inEnd);
	//リストに追加する.
	void	Add(TLim3DAnimeBase * inBase){ m_vector.push_back(inBase); }
	//
	bool	Action(int inTime);
	//アニメーションデータの破棄.
	void	Clear();
private:
	static THIS_ANALYSIS		m_AnalysisTable[];
	int							m_Time;
	std::vector<TLim3DAnimeBase*>	m_vector;
};

class TLim3DAnimeSeen
{
public:
	TLim3DAnimeSeen(){}
	~TLim3DAnimeSeen(){ Clear(); }

public:
	//フレームを分解します.
	bool	FreamSplit(const char *  inStart , const char * inEnd);
	//指定されたフレームを実行命令を出す.
	bool	Action(int inFreamNo, int inTime);
	//アニメーションは継続するの ?	継続 true / おしまい false;
	bool	IsContinue(int inFream) { return ( inFream < m_vector.size() ); } 
	//フレームデータの破棄.
	void	Clear();
private:
	////フレーム解析	Start と End がシーンデータ
	bool	FreamAnalysis(int inFreamTime,const char * inStart , const char * inEnd);

	std::vector<TLim3DAnimeFream*>		m_vector;
};

enum	__ENUM_LIM3D_ANIME_TASK_OPTION
{
	__ENUM_LIM3D_ANIME_TASK_OPTION_NONE		 =	0,
	__ENUM_LIM3D_ANIME_TASK_OPTION_REPERT	 =	1,
	__ENUM_LIM3D_ANIME_TASK_OPTION_AUTOKILL	 =	2,
};

class TLim3DAnimeTask
{
public:
	TLim3DAnimeTask(TLim3DAnimeSeen* inSeen) 
	{
		__RTI_CHECKER(inSeen != NULL);
		m_FreamNum = 0;
		m_Seen = inSeen;
		m_Time = 0;
		m_Option = __ENUM_LIM3D_ANIME_TASK_OPTION_NONE;
	}
	//このアニメタスクを実行.
	bool Action();

public:	//アクセスメソッド
	//オプション関係.
	__ENUM_LIM3D_ANIME_TASK_OPTION	GetOption(){return m_Option;}
	void	SetOption(__ENUM_LIM3D_ANIME_TASK_OPTION inM){m_Option = inM;}
	//時間関係
	int	GetTime(){return m_Time;}
	void	SetTime(int inM){m_Time = inM;}
	//現在実行しているフレーム関係.
	int	GetFream(){return m_FreamNum;}
	void	SetFream(int inM){m_FreamNum = inM;}
	

private:
	TLim3DAnimeSeen*				m_Seen;
	int								m_FreamNum;
	int								m_Time;

	__ENUM_LIM3D_ANIME_TASK_OPTION	m_Option;
};

class TLim3DAnimeControl
{
public:
	TLim3DAnimeControl(){}
	~TLim3DAnimeControl(){ Clear(); }
public:
	//タグをサーチする.
	static char* SearchTag
		(const char* inStart , const char* inEnd , const char* inTag , char** outEnd);

	//アニメーションの実行のためのタスクを作成する.
	TLim3DAnimeTask*	TaskCreate(const char * inAnimeName);
	//ファイルを読み込む.
	bool	Read(const char * inFileName);
	//シーンデータ & タスクデータの破棄.
	void	Clear();
private:
	//シーンを分解します.
	bool	SeenSplit(const char * theFileData ,const char* theFileLen);
	bool	SeenAnalysis(const char * inSeenName, const char * inStart , const char * inEnd);

	std::vector<TLim3DAnimeSeen*>		m_Seenvector;
	std::vector<TLim3DAnimeTask*>		m_Taskvector;
};

#endif				//重複防止