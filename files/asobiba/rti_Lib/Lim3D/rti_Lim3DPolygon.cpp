
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./Lim3D/rti_Lim3DMaster.h>
#include <./Base/rti_vector.h>

#define _LIM3D_MAX_VEC 4		//最大点数.

/*****************/
//ユーザーのための高レベルメソッド.
/*****************/
//絵画
void TLim3DPolygon::Draw()
{
	PolygonDataEx*	UsingData = m_LinkData.GetStart();
	PolygonDataEx*	it = UsingData;
	PolygonDataEx*	end= m_LinkData.GetEnd();
	XYZFixed		Avg = {0};					//ポリゴンの平均値(重心)
	Fixed			Eye_Avg_Z = 0;				//視点座標のポリゴンの中心のZ座標.

	for( ; it != end ; it++ )
	{
		//平均値を求めます(そのための足し算).
		VectorAdd( &Avg , &Avg , & (it->LinkVecPointer->World) );
		//視点平均値を求めます(そのための足し算).
		Eye_Avg_Z +=  it->LinkVecPointer->Eye.z ;
	}
	//裏面だったら即没.
	if (IsBackstairs(UsingData) == true)	return ;

	//平均値を算出.
	switch( m_LinkData.GetCount() )
	{
	case 4:
		VectorSar(&m_AvgPoint , &Avg , 2);
		m_Eye_Avg_Z = Eye_Avg_Z / 4;
		break;
	case 3:
		VectorSar(&m_AvgPoint , &Avg , 1);
		VectorSub(&m_AvgPoint , &m_AvgPoint ,&Avg );
		m_Eye_Avg_Z = (Eye_Avg_Z / 2) - Eye_Avg_Z;
	}
	//法線ベクトルの算出(World / 単位)
	ClasHousenVector(UsingData);

	//ポリゴンを ZSort 対象に追加.
	TLim3DMaster::AddZSortPolygon(this);
}

//実際絵画部分
void TLim3DPolygon::TrueDraw()
{
	PolygonDataEx*	UsingData = m_LinkData.GetStart();
	DrawUse	du[ _LIM3D_MAX_VEC+1 ];	//+1はクリッピングのために...
	DrawUse	*dup = du;				//効率処理優先.

	//クリッピングして、絵画します.
	int i;
	int UseVec = m_PolygonClipping.DoClipping( du , UsingData , m_LinkData.GetCount() );
	if (UseVec <= 2) return;

	XYZFixed * Pos;
	//透視投影
	for( i=0 ; i < UseVec ; i++ , dup++ )
	{
		Pos = &(dup->Pos);
		dup->Display.x = IF(LIM3D_USING_AREAW/2) + (FixedDiv(Pos->x , Pos->z) << 8);
		dup->Display.y = IF(LIM3D_USING_AREAH/2) - (FixedDiv(Pos->y , Pos->z) << 8);
		dup->Display.z = Pos->z;
	}
	//DoClipping は絵画する頂点を返して、 du をにデータをセットします.
	//DoDraw は、その du を受け取って、絵画します.
	m_PolygonDraw.DoDraw(this , du , UseVec);
}


//つながり情報の登録.
bool TLim3DPolygon::LinkEntry(PolygonData *_c , int Many ,
										bool SizeNotChange , bool IsSet)
{
	//わけのわからない指令は無視.
	if (Many <= 0) return false;

	//各種戻り値.
	bool theRet;
	//頂点データへのアクセス経路の確定.
	VecWorldEye*	VecData = (GetParenceModel()->GetVecCaretaker())->GetStart();
	//うーん、配列確保面倒です.
	PolygonDataEx* thePDE = new PolygonDataEx[Many];
	//念のため確保失敗をチェック.
	if (thePDE == NULL) return false;

	for(int theI = 0 ; theI < Many ; theI ++)
	{
		//頂点のデータを リンク処理より ポインタ処理に置き換えます.
		//これにより、実行時にポインタに変換するのに比べて効率があがる
		//なお、危険度も上昇しますね (汗)
		thePDE[theI].LinkVecPointer = &VecData[ _c[theI].Link] ;
		//テクスチャーはそのまま.
		thePDE[theI].Texture        = _c[theI].Texture;
	}
	//登録.
	if (IsSet)
	{
		theRet =  m_LinkData.Set(thePDE,Many,SizeNotChange);
	}
	else
	{
		theRet =  m_LinkData.Add(thePDE,Many,SizeNotChange);
	}
	//ちゃんと削除しましょー.
	delete [] thePDE;
	//結果を返します.
	return theRet;
}

//つながり情報の登録.
bool TLim3DPolygon::LinkEntry(PolygonDataCoexist *_c , int Many ,
										bool SizeNotChange , bool IsSet)
{
	//わけのわからない指令は無視.
	if (Many <= 0) return false;

	//頂点グループの先頭.
	VecWorldEye*	VecData;
	//各種戻り値.
	bool theRet;
	//親モデル.
	TLim3DModel* thePareceModel = GetParenceModel();
	//うーん、配列確保面倒です.
	PolygonDataEx* thePDE = new PolygonDataEx[Many];
	//念のため確保失敗をチェック.
	if (thePDE == NULL) return false;

	for(int theI = 0 ; theI < Many ; theI ++)
	{
		//このポリゴンが所属しているモデル以外からの関連付けかどうか検査.
		if ( (_c->Model == NULL || _c->Model == thePareceModel) )
		{//現在所属しているところからの関連付けです.
			VecData = (thePareceModel->GetVecCaretaker())->GetStart();
		}
		else
		{//現在属していないところからの関連付けです.
			TLim3DModel*	theModel = ((TLim3DModel*)(_c[theI].Model));
			//属していない関連付けは、自分の先祖にあたる関係しか許さない.
			if( !thePareceModel->IsForefathers( theModel ) )
			{//エラー
				return false;
			}
			VecData = (theModel->GetVecCaretaker())->GetStart();
		}

		//頂点のデータを リンク処理より ポインタ処理に置き換えます.
		//これにより、実行時にポインタに変換するのに比べて効率があがる
		//なお、危険度も上昇しますね (汗)
		thePDE[theI].LinkVecPointer = &VecData[ _c[theI].Link] ;
		//テクスチャーはそのまま.
		thePDE[theI].Texture        = _c[theI].Texture;
	}
	//登録.
	if (IsSet)
	{
		theRet =  m_LinkData.Set(thePDE,Many,SizeNotChange);
	}
	else
	{
		theRet =  m_LinkData.Add(thePDE,Many,SizeNotChange);
	}
	//ちゃんと削除しましょー.
	delete [] thePDE;
	//結果を返します.
	return theRet;
}

//つながり情報セット.
bool TLim3DPolygon::SetLink(PolygonData *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  true);
}

//つながり情報セット.
bool TLim3DPolygon::AddLink(PolygonData *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  false);
}
//つながり情報セット.
bool TLim3DPolygon::SetLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  true);
}

//つながり情報セット.
bool TLim3DPolygon::AddLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  false);
}

//つながりクリア
void TLim3DPolygon::ClearLink()
{
	m_LinkData.Clear();
}

/*****************/
//ユーザーのための低レベルメソッド.
/*****************/
//つながりデータをベタで取得.
TLim3DLinkData*	TLim3DPolygon::GetLinkData()
{
	return &m_LinkData;
}


//法線ベクトルの算出.
void TLim3DPolygon::ClasHousenVector(PolygonDataEx* inUsing)
{
	/*
	*
	* 1                       0
	*  C----------------------A
	*   \
	*    \
	*     \
	*      \
	*       \
	*        \
	*         B 2
	*/
	XYZFixed* const Va = & (inUsing[2].LinkVecPointer->World);
	XYZFixed* const Vb = & (inUsing[0].LinkVecPointer->World);
	XYZFixed* const Vc = & (inUsing[1].LinkVecPointer->World);

	XYZFixed    A,B;    

	A.x = (Va->x - Vc->x);    	A.y = (Va->y - Vc->y);	    A.z = (Va->z - Vc->z);
	B.x = (Vb->x - Vc->x);	    B.y = (Vb->y - Vc->y);    	B.z = (Vb->z - Vc->z);
	//正規化.
	//Normalize(&A);	Normalize(&B);
	//法線ベクトルの算出.
	CrossProduct( &m_HousenVector,&A,&B );
	Normalize( &m_HousenVector );
}

//裏面判定. 
bool TLim3DPolygon::IsBackstairs(PolygonDataEx* inUsing)
{
	//return false;
	/*
	*
	* 1                       0
	*  C----------------------A
	*   \
	*    \
	*     \
	*      \
	*       \
	*        \
	*         B 2
	*/
	//視点座標でやっていることに注意せよ.
	XYZFixed* const Va = & (inUsing[2].LinkVecPointer->Eye);
	XYZFixed* const Vb = & (inUsing[0].LinkVecPointer->Eye);
	XYZFixed* const Vc = & (inUsing[1].LinkVecPointer->Eye);
	/*
	//外積の Z 成分だけを算出する.
	Fixed theZ = FixedMul( (Va->x - Vc->x) , (Vb->y - Vc->y) ) - 
						FixedMul( (Va->y - Vc->y) , (Vb->x - Vc->x) );
	// 0よりも小さい場合は裏面.
	if (theZ > 0) return true;
	//表面.
	return false;
	*/
	XYZFixed    A,B;    

	A.x = (Va->x - Vc->x);    	A.y = (Va->y - Vc->y);	    A.z = (Va->z - Vc->z);
	B.x = (Vb->x - Vc->x);	    B.y = (Vb->y - Vc->y);    	B.z = (Vb->z - Vc->z);
	//正規化.
	//Normalize(&A);	Normalize(&B);
	//法線ベクトルの算出.
	XYZFixed EyeHousen;
	CrossProduct( &EyeHousen,&A,&B );
	Normalize( &EyeHousen );

	if (EyeHousen.z > Float2Fixed(0.5) ) return true;

	return false;
}

//テクスチャーの再マッピング(AUTO).
bool TLim3DPolygon::ReMapTexture()
{
	TLim3DMaterial*		theMaterial	;		//マテリアル取得.
	theMaterial = GetMaterial() ;
	BMD theTexture = theMaterial->GetTexture();

	//念のためテクスチャーがちゃんとはられているかチェック.
	if (theTexture==NULL)	return false;
	return ReMapTexture( 0 ,0 ,
				theTexture->w , 0   ,
				theTexture->w , theTexture->h ,
				0 ,theTexture->h						);
}

//テクスチャーの再マッピング.
bool TLim3DPolygon::ReMapTexture(int X1 ,int Y1,
						int X2,int Y2, int X3,int Y3, int X4,int Y4)
{
	int				Count     = m_LinkData.GetCount();
	PolygonDataEx*	UsingData = m_LinkData.GetStart();

	if ( Count <= 0) return false;

	UsingData[0].Texture.x = X1;
	UsingData[0].Texture.y = Y1;
	if ( Count <= 1) return true;

	UsingData[1].Texture.x = X2;
	UsingData[1].Texture.y = Y2;
	if ( Count <= 2) return true;

	UsingData[2].Texture.x = X3;
	UsingData[2].Texture.y = Y3;
	if ( Count <= 3) return true;
	
	UsingData[3].Texture.x = X4;
	UsingData[3].Texture.y = Y4;
	if ( Count <= 4) return true;

	//頂点が多すぎる.
	return false;
}


inline TLim3DModel* TLim3DPolygon::GetParenceModel()
{
	return ( (TLim3DPolygonCaretaker*)m_Parence)->GetParenceModel(); 
}
