#ifndef	___LIM3D_MASTER__H	//重複防止
#define ___LIM3D_MASTER__H

#include <./rti_define.h>

#include  <./Lim3D/rti_Lim3DEyeCaretaker.h>		//視点管理
#include  <./Lim3D/rti_Lim3DModelCaretaker.h>	//モデル管理
#include  <./Lim3D/rti_Lim3DLightCaretaker.h>	//光源管理
#include  <./Lim3D/rti_Lim3DMaterialCaretaker.h>//マテリアル管理.
#include  <./Graphics/rti_zbuffer.h>			//ZBUffer
#include  <./Lim3D/rti_Lim3DZSortWithZBuffer.h>	//ポリゴンの Z ソート.
#include  <./Base/rti_NameServer.h>				//3Dエンジンの一時ネームテーブル.
#include  <./Graphics/rti_bitmaplib.h>			//ビットマップライブラリ.
#include  <./Lim3D/rti_Lim3DAnime.h>			//アニメーション.


#define LIM3D_USING_AREAW	320
#define LIM3D_USING_AREAH	240

//マスター3Dクラス.
class TLim3DMaster
{
public:
	static void Init()
	{
		//ZBuffer確保.
		m_ZBuffer.Alloc(LIM3D_USING_AREAW , LIM3D_USING_AREAH);
	}
	static void End()
	{
		//各種管理クラス
		m_EyeCaretaker.Clear();
		m_ModelCaretaker.Clear();
		m_LightCaretaker.Clear();
		m_MaterialCaretaker.Clear();
		//テクスチャー
		m_TextureCaretaker.Clear();
		//ネームサーバー
		NameServer.Release();
		//ZBuffer
		m_ZBuffer.Destroy();
		//ポリゴンの ZBuffer
		m_ZSortBuffer.Clear();
		//アニメーション管理.
		m_AnimeControl.Clear();
	}
	//選択されている視点を得る.
	static TLim3DEye*	GetSelectEye(){	return m_SelectEye;	}
	//視点を選択する.
	static void			SetSelectEye(TLim3DEye* SelectEye){	m_SelectEye = SelectEye;}
	//ZBuffer 取得.
	static TZBuffer*	GetZBuffer(){	return &m_ZBuffer;}
	//絵画領域取得.
	static BMD			GetOutBuffer(){	return m_SelectBMD;	}
	//絵画領域設定.
	static void			SetOutBuffer(BMD SelectBMD){	m_SelectBMD = SelectBMD;	}
	//マテリアルにアクセス.
	static TLim3DMaterial* GetMaterial(int inNo){return m_MaterialCaretaker.At(inNo);}

	//トップモデルの作成.
	static TLim3DModel*	NewModel(){ return m_ModelCaretaker.New(); }
	//視点の作成.
	static TLim3DEye*	NewEye(){ return m_EyeCaretaker.New(); }
	//ライトモデルの作成.
	static TLim3DLight*	NewLight(){ return m_LightCaretaker.New(); }
	//マテリアルの作成.
	static TLim3DMaterial*	NewMaterial(){ return m_MaterialCaretaker.New(); }
	//テクスチャーの作成.
	static BMD	NewTexture(const char* inTextureName){ return m_TextureCaretaker.Make(inTextureName); }
	//アニメーションタスク.
	static TLim3DAnimeTask*	NewAnimeTask
			(const char* inSeenTag){return m_AnimeControl.TaskCreate(inSeenTag);}	
	//アニメーションを読み込む
	static bool	NewAnimeData
			(const char* inAnimeFile){return m_AnimeControl.Read(inAnimeFile);}	

	//作画開始.
	static void Cls()
	{
		//ZBuffer をクリア.
		m_ZBuffer.Clear();
		//ポリゴンの ZBuffer
		m_ZSortBuffer.Clear();
		//視点回転マトリックスの作成.
		m_SelectEye->MakeMatrix();
	}
	//作画終了.
	static void Ref()
	{
		//ポリゴンの ZBuffer のソート
		m_ZSortBuffer.Sort();
		//ポリゴンの ZBuffer の絵画
		m_ZSortBuffer.AllDraw();
	}

	//視点管理クラスにベタアクセス.
	static TLim3DEyeCaretaker* GetEyeCaretaker(){return &m_EyeCaretaker;}
	//ライト管理クラスにベタアクセス.
	static TLim3DLightCaretaker* GetLightCaretaker(){return &m_LightCaretaker;}
	//モデル管理クラスにべたアクセス.
	static TLim3DModelCaretaker* GetModelCaretaker(){return &m_ModelCaretaker;}
	//マテリアル管理クラスにべたアクセス.
	static TLim3DMaterialCaretaker* GetMaterialCaretaker(){return &m_MaterialCaretaker;}
	//ポリゴンの Z ソートへ追加.
	static void AddZSortPolygon(TLim3DPolygon* inPolygon)
									{m_ZSortBuffer.Add( inPolygon ) ;}

	static TStringHashTable			NameServer;
protected:
	//各種管理クラス
	static TLim3DEyeCaretaker		m_EyeCaretaker;
	static TLim3DModelCaretaker		m_ModelCaretaker;
	static TLim3DLightCaretaker		m_LightCaretaker;
	static TLim3DMaterialCaretaker	m_MaterialCaretaker;
	static TLim3DAnimeControl		m_AnimeControl;
	
	//テクスチャー.
	static TBmpChar				m_TextureCaretaker;
	//ZBuffer
	static TZBuffer				m_ZBuffer;
	//選択されている視点.
	static TLim3DEye*			m_SelectEye;
	//絵画する画面領域.
	static BMD					m_SelectBMD;
	//ポリゴンの Zソート
	static TLim3DZSortWithZBuffer	m_ZSortBuffer;
private:
};


#endif							//重複防止
