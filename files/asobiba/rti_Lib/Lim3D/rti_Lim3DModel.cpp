#include <./Lim3D/rti_Lim3DModel.h>

#include <./Lim3D/rti_Lim3DMaster.h>

//コンストラクタ
TLim3DModel::TLim3DModel()
{
	m_Vec		= new TLim3DVecCaretaker;		//頂点情報集積クラス
	m_Polygon	= new TLim3DPolygonCaretaker;	//ポリゴン情報集積クラス
	m_Make		= new TLim3DMake;				//モデル親子関係
	m_Point		= new TLim3DPoint;				//位置
	m_Breaking		= false;					//自分が崩壊している.

	m_Vec->Mount_form_Model(this);				//頂点関係に自分が親だと伝える.
	m_Make->Mount_form_Model(this);				//モデル親子関係に自分が親だと伝える.
	m_Polygon->Mount_form_Model(this);			//ポリゴン管理に自分が親だと伝える.
}

//デストラクタ
TLim3DModel::~TLim3DModel()
{
	m_Point->Release();
	m_Polygon->Release();
	m_Vec->Release();
	m_Make->Release();
}

//ガページコレクションより削除.
bool	TLim3DModel::Del(TLim3DModel* Model)
{
	//現在自分が崩壊にあるなら....
	if (m_Breaking == true) return true;	//自分が崩壊しているので子どものことは無視.
	//ガページコレクションより探し出し削除.
	m_Make->Del( Model );

	return true;
}

//親モデルの取得
inline TLim3DModel* TLim3DModel::GetParenceModel()
{
	return  ((TLim3DMake*)(m_Parence))->GetParenceModel();
}

/*****************/
//ユーザーのための高レベルメソッド.
/*****************/
//このモデル以下のモデルの絵画.
void	TLim3DModel::Drawing()
{
	ClasOnly();
	DrawOnly();
}

//このモデル以下のモデルの絵画.
void	TLim3DModel::Drawing(TMatrix * Ma)
{
	ClasOnly(Ma);
	DrawOnly();
}

//破棄.
void	TLim3DModel::Release()
{
	//自分が崩壊状態とする.
	m_Breaking = true;
	//親クラス.
	TLim3DModel*	theModel = GetParenceModel();
	//親に削除を伝える.
	if (theModel == NULL)
		(TLim3DMaster::GetModelCaretaker())->Del(this);
	else
		theModel->Del(this);
	delete this;
}

//頂点情報の設定.
bool			TLim3DModel::SetVec(XYZFixed* _c ,	int Many , bool SizeNotChange)
{
	return m_Vec->Set(_c,Many,SizeNotChange);
}

//頂点情報の追加.
bool			TLim3DModel::AddVec(XYZFixed* _c ,	int Many , bool SizeNotChange )
{
	return m_Vec->Add(_c,Many,SizeNotChange);
}

//すべての頂点の破棄
void			TLim3DModel::ClearVec()
{
	m_Vec->Clear();
}
//ポリゴン情報の設定.
TLim3DPolygon* 	TLim3DModel::NewPolygon()
{
	return m_Polygon->New();
}
//モデルに存在するすべてのポリゴンの破棄.
void			TLim3DModel::ClearPolygon()
{
	m_Polygon->Clear();
}
//ポリゴン検索開始
TLim3DPolygon*	TLim3DModel::FindFirstPolygon()
{
	return m_Polygon->FindFirst();
}
//ポリゴン検索続行.
TLim3DPolygon*	TLim3DModel::FindNextPolygon()
{
	return m_Polygon->FindNext();
}
//モデルの回転角度を指定.
void			TLim3DModel::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point->SetAngleX(x);
	m_Point->SetAngleY(y);
	m_Point->SetAngleZ(z);
}
//モデルの場所を指定.
void			TLim3DModel::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point->SetPosX(x);
	m_Point->SetPosY(y);
	m_Point->SetPosZ(z);
}
//モデルの回転角度を取得.
void			TLim3DModel::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point->GetAngleX();
	*y = m_Point->GetAngleY();
	*z = m_Point->GetAngleZ();
}
//モデルの場所を取得.
void			TLim3DModel::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point->GetPosX();
	*y = m_Point->GetPosY();
	*z = m_Point->GetPosZ();
}
//モデルの回転角度を増やす.
void			TLim3DModel::AddRotate(int x,int y,int z)
{
	m_Point->SetAngleX(m_Point->GetAngleX() + x);
	m_Point->SetAngleY(m_Point->GetAngleY() + y);
	m_Point->SetAngleZ(m_Point->GetAngleZ() + z);
}
//モデルの場所を移動する.
void			TLim3DModel::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point->SetPosX(m_Point->GetPosX() + x);
	m_Point->SetPosY(m_Point->GetPosY() + y);
	m_Point->SetPosZ(m_Point->GetPosZ() + z);
}
//子モデルを作成する.
TLim3DModel*	TLim3DModel::NewModel()
{
	return m_Make->New();
}
//すべての子モデルを削除
void			TLim3DModel::ClearModel()
{
	m_Make->Clear();
}
//モデル検索開始
TLim3DModel*	TLim3DModel::FindFirstModel()
{
	return m_Make->FindFirst();
}
//モデル検索続行.
TLim3DModel*	TLim3DModel::FindNextModel()
{
	return m_Make->FindNext();
}

//重複頂点の切り離し(これ危険あるねー)
void			TLim3DModel::Optimize()
{
	//頂点情報クラスへのベタアクセス
	TLim3DVecCaretaker*		theVec = GetVecCaretaker();
	//ポリゴン情報クラスへのベタアクセス
	TLim3DPolygonCaretaker*	thePoly = GetPolygonCaretaker();

	//格納されしデータ.
	VecWorldEye* theSrc  = theVec->GetStart();
	//すべてのデータの数.
	int theEnd = theVec->GetCount();
	//念のため、現在データがあるのかどうかチェック.
	if (theEnd <= 1) return ;
	//ループカウンター.
	int			 theI , theL ;
	//オリジナルデータを格納するバッファ.
	XYZFixed* theOrignalDataTemp = NULL;
	//重複していたら true を代入するフラグ.
	bool*     theDuplicationBuffer = NULL;

	try
	{
		//オリジナルデータを格納するバッファ.
		theOrignalDataTemp = new XYZFixed[ theEnd ];
		//重複していたら true を代入するフラグ.
		theDuplicationBuffer = new bool[ theEnd ];
		//念のため確保できたかチェック. 
		if ( theOrignalDataTemp == NULL || theDuplicationBuffer == NULL) throw ;
		//重複フラグを false でクリアします.
		memset(theDuplicationBuffer , 0 ,sizeof(bool)*theEnd );
		//オリジナルデータの添え字です.
		int			 theOrignalDataNum = 0;
	
		for( theI = 0 ; theI < theEnd ; theI++)
		{//theI に対しての重複データがないかチェックします.
			//重複フラグがたっていたら、そのデータは飛ばします.
			if (theDuplicationBuffer[theI] == true) continue;

			XYZFixed * theSrcVec = &(theSrc[theI].Vec);
			for( theL = theI ; theL < theEnd ; theL++)
			{
				XYZFixed * theDestVec = &(theSrc[theL].Vec);
				if ( theSrcVec->x == theDestVec->x && 
					 theSrcVec->y == theDestVec->y && 
					 theSrcVec->z == theDestVec->z )
				{//この TheL は、 theI のデータと重複しています.
					//重複フラグを立てます.
					theDuplicationBuffer[theL] = true;
					//この TheL を使用しているポリゴンの Linkデータを theOrignalDataNumに付け替えます.
					//OptimizePolygonLinkChange(theL , theOrignalDataNum);
					OptimizePolygonLinkChange( 
						&(theSrc[theL]) , &(theSrc[theOrignalDataNum]) );
				}
			}
			//オリジナルデータとして記録します.
			theOrignalDataTemp[theOrignalDataNum++] = *theSrcVec;
		}
		//重複していないデータをセット.
		theVec->Set( theOrignalDataTemp , theOrignalDataNum	 , true);
		//DEBUG3("最適化によって %d 頂点が %d になりました",theEnd , theOrignalDataNum);
	}
	catch(...)
	{
	}
	//重複フラグの破棄.
	delete [] theDuplicationBuffer;
	//重複していないデータを破棄.
	delete [] theOrignalDataTemp;
}


/*****************/
//ユーザーのための低レベルメソッド.
/*****************/

//計算のみ(頂点計算)
void	TLim3DModel::ClasOnly()
{
	//まず、回転行列を作成して、くるくるくるーって感じでワールドへ移行します.
	TMatrix		Mat;			//行列
	//回転行列作成.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());
	//すべての頂点を移動させるぞ!
	VecWorldEye* it  = m_Vec->GetStart();
	VecWorldEye* end = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		//it->World = it->Vec;
		Mat.VectorMul( &(it->World) , &(it->Vec) );
		//これで、 ワールドはもとまりました.

		//次は視点座標に変換します.
		(TLim3DMaster::GetSelectEye())->EyeConbert( &(it->Eye) , &(it->World) );
		//視点座標になりました.
	}
	//子モデルを絵画します.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->Drawing( &Mat );
	}
}

//計算のみ(頂点計算)
void	TLim3DModel::ClasOnly(TMatrix * inParenceMatrix)
{
	//まず、回転行列を作成して、くるくるくるーって感じでワールドへ移行します.
	TMatrix		Mat;			//行列
	//回転行列作成.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());
	//親の行列を加えます.
	Mat.Synthesis(inParenceMatrix);

	//すべての頂点を移動させるぞ!
	VecWorldEye* it  = m_Vec->GetStart();
	VecWorldEye* end = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		//it->World = it->Vec;
		Mat.VectorMul( &(it->World) , &(it->Vec) );
		//これで、 ワールドはもとまりました.

		//次は視点座標に変換します.
		(TLim3DMaster::GetSelectEye())->EyeConbert( &(it->Eye) , &(it->World) );
		//視点座標になりました.
	}
	//子モデルを絵画します.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->Drawing( &Mat );
	}
}

//絵画のみ(頂点計算がすでにされていることが条件)
void	TLim3DModel::DrawOnly()
{
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{
		Poly->Draw();		//ポリゴンに絵画命令発行.
	}
	while( Poly = m_Polygon->FindNext() );
}


//頂点情報クラスへのベタアクセス
TLim3DVecCaretaker*		TLim3DModel::GetVecCaretaker()
{
	return m_Vec;
}

//ポリゴン情報クラスへのベタアクセス
TLim3DPolygonCaretaker*	TLim3DModel::GetPolygonCaretaker()
{
	return m_Polygon;
}

//親子情報クラスへのベタアクセス
TLim3DMake*				TLim3DModel::GetMake()
{
	return m_Make;
}

//場所情報クラスへのベタアクセス
TLim3DPoint*			TLim3DModel::GetPoint()
{
	return m_Point;
}


//ポリゴンの指定されたつながりデータ(inTarget)を新しいデータ(inNew)に変更します.
void	TLim3DModel::OptimizePolygonLinkChange(VecWorldEye* inTarget ,VecWorldEye* inNew)
{
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{	//つながりデータ機能への べたアクセス取得.
		TLim3DLinkData* theLinkData = Poly->GetLinkData();
		//ポリゴンのリンクデータにアクセスして、書き換えを行います.
		PolygonDataEx* thePolygonLinkDataIt = theLinkData->GetStart();
		PolygonDataEx* thePolygonLinkDataEnd= theLinkData->GetEnd();
		
		for( ; thePolygonLinkDataIt != thePolygonLinkDataEnd ; thePolygonLinkDataIt ++)
		{
			if ( thePolygonLinkDataIt->LinkVecPointer == inTarget)
			{	//ターゲットを発見しました. 書き換えます.
				thePolygonLinkDataIt->LinkVecPointer = inNew;
			}
		}
	}
	while( Poly = m_Polygon->FindNext() );
}

//頂点データ管理クラスで、頂点データの再アロケーションが実行された.
void	TLim3DModel::ReAllocMemoryVec(VecWorldEye* OldMemoryAddress ,
							VecWorldEye* NewMemoryAddress ,  int OldUseSize)
{
	VecWorldEye*	TempData;
	VecWorldEye*	OldMemoryAddressEnd = OldMemoryAddress+OldUseSize;
	TLim3DPolygon* Poly = m_Polygon->FindFirst();
	if (Poly == NULL)	return ;
	do
	{	//つながりデータ機能への べたアクセス取得.
		TLim3DLinkData* theLinkData = Poly->GetLinkData();
		//ポリゴンのリンクデータにアクセスして、書き換えを行います.
		PolygonDataEx* thePolygonLinkDataIt = theLinkData->GetStart();
		PolygonDataEx* thePolygonLinkDataEnd= theLinkData->GetEnd();

		for( ; thePolygonLinkDataIt != thePolygonLinkDataEnd ; thePolygonLinkDataIt ++)
		{
			//長ったらしくなるので、処理対象のデータを写しておく.
			TempData = thePolygonLinkDataIt->LinkVecPointer;
			if ( TempData >= OldMemoryAddress && TempData < OldMemoryAddressEnd)
			{	//ターゲットを発見しました. 書き換えます.
				thePolygonLinkDataIt->LinkVecPointer =
					(VecWorldEye*)((DWORD)TempData -
							(DWORD)OldMemoryAddress + (DWORD)NewMemoryAddress);
			}
		}
	}
	while( Poly = m_Polygon->FindNext() );

	//子モデルによって、共有されているかもしれないので、
	//念のためすべての子を検索.
	TLim3DModel*	theChildModel ;
	for( theChildModel = FindFirstModel()  ;
				theChildModel != NULL ; theChildModel = FindNextModel() )
	{
		theChildModel->ReAllocMemoryVec
					(OldMemoryAddress , NewMemoryAddress , OldUseSize);
	}
}


//指定したモデルのコピーに自分がなる.
void	TLim3DModel::CopyForm(TLim3DModel * inModel)
{
	m_Point->CopyForm( inModel->GetPoint() );
//	m_Polygon->CopyForm( inModel->GetPolygonCaretaker() );
//	m_Vec->CopyForm( inModel->GetVecCaretaker() );
//	m_Make->CopyForm( inModel->GetMake() );
}


//指定されたモデルが自分達が属する　親子関係で言う「ご先祖」にあたるかどうか調べる.
bool TLim3DModel::IsForefathers(TLim3DModel * inModel)
{
	if ( this == inModel) return true;

	TLim3DModel * theParenceModel = GetParenceModel();
	if (theParenceModel == NULL) return false;
	return theParenceModel->IsForefathers(inModel);
}



//自分の親に対してのスキン処理を実行する.
void TLim3DModel::Skin()
{
	TLim3DModel * theParenceModel = GetParenceModel();
	//親が居ない場合は没.
	if (theParenceModel == NULL) return ;

	//自分の親の頂点へのアクセス経路を樹立.
	TLim3DVecCaretaker* theParenceVec = theParenceModel->GetVecCaretaker();

	//ワークエリア.
	VecWorldEye *it , *end , *ParenceIt , *ParenceEnd;
	//まず、回転行列を作成して、くるくるくるーって感じでワールドへ移行します.
	TMatrix		Mat;			//行列
	//回転行列作成.
	Mat.MakeTrans(m_Point->GetAngleX() , m_Point->GetAngleY() , m_Point->GetAngleZ() ,
					m_Point->GetPosX() , m_Point->GetPosY() , m_Point->GetPosZ());

	//すべての頂点を移動させるぞ!
	it  = m_Vec->GetStart();
	end  = m_Vec->GetEnd();
	for ( ; it != end ; it++)
	{
		Mat.VectorMul( &(it->World) , &(it->Vec) );
	}

	//共有できる頂点をサーチします.
	ParenceIt  = theParenceVec->GetStart();
	ParenceEnd = theParenceVec->GetEnd();
	for( ; ParenceIt != ParenceEnd ; ParenceIt++)
	{
		it  = m_Vec->GetStart();
		end  = m_Vec->GetEnd();
		for ( ; it != end ; it++)
		{
			//この World は、親座標での話です.
			if ( VectorCmp( &ParenceIt->Vec , &it->World , IF(3) ) )
			{	//変更できそうなので、ポリゴン君につたえる.
				OptimizePolygonLinkChange( it , ParenceIt);
			}
		}
	}
}