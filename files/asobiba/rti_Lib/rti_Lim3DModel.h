#ifndef	___LIM3D_MODEL__H	//重複防止
#define ___LIM3D_MODEL__H

#include <windows.h>

#include <./rti_define.h>
#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Matrix.h>

class	TLim3DVecCaretaker;
class	TLim3DPolygonCaretaker;
class	TLim3DMake;
class	TLim3DPoint;
class	TLim3DPolygon;

class TLim3DModel
{
public:
	/*****************/
	//クラス維持のためのメソッド
	/*****************/
	//コンストラクタ
	TLim3DModel();
	//デストラクタ
	~TLim3DModel();
	//ガページコレクションより削除.
	bool	DeleteCaretation(TLim3DModel* Model);
	//親モデルを教える.
	bool Mount(TLim3DModel * Model)
	{
		if (m_ParenceModel != NULL)	return false;	//先客がいる.
		m_ParenceModel = Model;
		return true;
	}

	/*****************/
	//ユーザーのための高レベルメソッド.
	/*****************/
	//このモデル以下のモデルの絵画.
	void			Drawing();
	//このモデル以下のモデルの絵画.
	void			Drawing(TMatrix * Ma);
	//破棄
	bool			Release();
	//頂点情報の設定.
	bool			SetVec(XYZFixed* _c ,	int Many , bool SizeNotChange = true);
	//頂点情報の追加.
	bool			AddVec(XYZFixed* _c ,	int Many , bool SizeNotChange = true);
	//すべての頂点の破棄
	void			ClearVec();
	//ポリゴン情報の設定.
	TLim3DPolygon*	NewPolygon();
	//モデルに存在するすべてのポリゴンの破棄.
	void			ClearPolygon();
	//ポリゴン検索開始
	TLim3DPolygon* 	FindFirstPolygon();
	//ポリゴン検索続行.
	TLim3DPolygon* 	FindNextPolygon();
	//モデルの回転角度を指定.
	void			SetRotate(Angle x,Angle y,Angle z);
	//モデルの場所を指定.
	void			SetPos(Fixed x,Fixed y,Fixed z);
	//モデルの回転角度を取得.
	void			GetRotate(Angle *x,Angle *y,Angle *z);
	//モデルの場所を取得.
	void			GetPos(Fixed *x,Fixed *y,Fixed *z);
	//モデルの回転角度を増やす.
	void			AddRotate(int x,int y,int z);
	//モデルの場所を移動する.
	void			AddPos(Fixed x,Fixed y,Fixed z);
	//子モデルを作成する.
	TLim3DModel*	NewModel();
	//すべての子モデルを削除
	void			ClearModel();
	//モデル検索開始
	TLim3DModel*	FindFirstModel();
	//モデル検索続行.
	TLim3DModel*	FindNextModel();
	//重複頂点の切り離し(これ危険あるねー)
	void			Optimize();
	//指定したモデルのコピーに自分がなる.
	void			CopyForm(TLim3DModel * inModel);

	/*****************/
	//ユーザーのための低レベルメソッド.
	/*****************/
	//計算のみ(頂点計算)
	void					ClasOnly();
	//計算のみ(頂点計算)
	void					ClasOnly(TMatrix * inParenceMatrix);
	//絵画のみ(頂点計算がすでにされていることが条件)
	void					DrawOnly();
	//重複する頂点の削除.
	void					Smart();
	//親モデル取得.
	TLim3DModel*			GetParenceModel();
	//頂点情報クラスへのベタアクセス
	TLim3DVecCaretaker*		GetVecCaretaker();
	//ポリゴン情報クラスへのベタアクセス
	TLim3DPolygonCaretaker*	GetPolygonCaretaker();
	//親子情報クラスへのベタアクセス
	TLim3DMake*				GetMake();
	//場所情報クラスへのベタアクセス
	TLim3DPoint*			GetPoint();

	/*****************/
	//ユーザーのための低^2 レベルメソッド. (危険!!)
	/*****************/
	//ポリゴンの指定されたつながりデータ(inTarget)を新しいデータ(inNew)に変更します.
	void					OptimizePolygonLinkChange
								(VecWorldEye* inTarget ,VecWorldEye* inNew);
	//頂点データ管理クラスで、頂点データの再アロケーションが実行された.
	void					ReAllocMemoryVec(VecWorldEye* OldMemoryAddress ,
								VecWorldEye* NewMemoryAddress ,  int OldUseSize);

protected:

	TLim3DModel*	m_ParenceModel;			//親モデル.
	bool			m_Breaking;				//自分が崩壊しているなら true

	TLim3DVecCaretaker*		m_Vec;			//頂点情報
	TLim3DPolygonCaretaker*	m_Polygon;		//ポリゴン情報
	TLim3DMake*				m_Make;			//親子関係
	TLim3DPoint*			m_Point;		//位置

private:
};

//モデル定義
typedef TLim3DModel* MODEL;

#include <./Lim3D/rti_Lim3DVecCaretaker.h>
#include <./Lim3D/rti_Lim3DPolygonCaretaker.h>
#include <./Lim3D/rti_Lim3DMake.h>
#include <./Lim3D/rti_Lim3DPoint.h>

#endif				//重複防止