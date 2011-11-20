#include <./Lim3D/rti_Lim3DMaster.h>

//各種管理クラス
TLim3DEyeCaretaker		TLim3DMaster::m_EyeCaretaker;
TLim3DModelCaretaker	TLim3DMaster::m_ModelCaretaker;
TLim3DLightCaretaker	TLim3DMaster::m_LightCaretaker;
TLim3DMaterialCaretaker	TLim3DMaster::m_MaterialCaretaker;

//アニメーション.
TLim3DAnimeControl		TLim3DMaster::m_AnimeControl;

//テクスチャー
TBmpChar				TLim3DMaster::m_TextureCaretaker;

//ネームサーバー
TStringHashTable		TLim3DMaster::NameServer;
//ZBuffer
TZBuffer				TLim3DMaster::m_ZBuffer;
//ポリゴンの ZBuffer
TLim3DZSortWithZBuffer	TLim3DMaster::m_ZSortBuffer;
//選択されている視点.
TLim3DEye*				TLim3DMaster::m_SelectEye;
//絵画する画面領域.
BMD						TLim3DMaster::m_SelectBMD;

