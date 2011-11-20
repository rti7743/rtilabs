#include "StartWithExeFile.h"
#ifdef OBJECT_LIFE


#ifndef ___OBJECTH
#define ___OBJECTH
#include <windows.h>
#include <list>

class TObject ;

//シンプルなイベント
typedef void (*SimpleEvent)(TObject *my);
//マウスイベント
typedef void (*MouseEvent)(TObject *my ,UINT fwKeys, int xPos, int yPos);
//fwKeys = wParam;	/* キーのフラグ	*/
//xPos = LOWORD(lParam);	/* カーソルの水平方向の位置	*/
//yPos = HIWORD(lParam);	/* カーソルの垂直方向の位置	*/

//キーイベント
typedef void (*KeyEvent)(TObject *my ,int nVirtKey, UINT lKeyData);
//nVirtKey = (int) wParam;	/* 仮想キー コード	*/
//lKeyData = lParam;	/* キーのデータ	*/

//ブールイベント
typedef bool (*BoolEvent)(TObject *my);


typedef std::list<TObject*>::iterator ObjectIT;
/*****************************************************************/
//すべての元になるやつ
//すべてのコンポーネント(コントロールorオブジェクト)は、
//この TObject を継承するはずです (ぉ
/*****************************************************************/
class TObject 
{
private:
	//そのオブジェクトの破棄するときのあるべき姿にキャスト
    void ObjectTrancerationAndBreak(TObject* hObject);
    //自分の所有しているオブジェクトリスト
    std::list<TObject*> ObjectList;
    int Tag; //ご自由にお使いください(^^;;
    int TagSub; //ご自由にお使いください(^^;;
	int OwnerUse; //オーナーがしようすることもある

protected:
	/******************************************/
	/*                メソッド                */
	/******************************************/
	//自殺
    void IamDestroy();
	//子供殺し
    void ChildDestroy();
	//新規にオブジェクト追加
    void OwnerAdd(TObject* hOwner,TObject* hMy);
	//オブジェクトの削除
    void virtual DestroyComponent() = 0;
	//オブジェクトの初期化
    void virtual InitComponent() = 0;
	/******************************************/
	/*            プロパティ                  */
	/******************************************/
    int Kind;        //自分の種類
    int Id;            //ID識別子

    HANDLE hWnd;       //ウィンドハンドル
    TObject* hMyOwner;    //自分をもっているもの
  	bool OwerKill;   //オーナーが死んでいるか

public:
	/******************************************/
	/*                メソッド                */
	/******************************************/
	TObject(); //コンストラクタ
	//WM_COMMAND処理
    virtual void WmCommandCome(WPARAM wParam) {};
    //終了させる
    virtual void UserDestroy() {};

    ObjectIT GetComponentFarst(){return ObjectList.begin() ;};
    ObjectIT GetComponentEnd(){return ObjectList.end() ;};
    //自分の持っているコントロールの数をかえす
    unsigned short HowMenyHaveControls(){return ObjectList.size() ;};

    //タグ関係
    int GetTag(){return Tag;};
    int GetTagSub(){return TagSub;};
    void SetTag(int tag){Tag = tag;};
    void SetTagSub(int tag){TagSub = tag;};

    //オーナーユーズ
    int GetOwerUse(){return OwnerUse;};
    void SetOwerUse(int use){OwnerUse = use;};

    TObject* GethOwer(){return hMyOwner;};
    HANDLE GethOwerWnd(){return hMyOwner->hWnd;};
    //IDをゲット
    int GetId(){return Id;};
    HWND GethWnd(){return (HWND)hWnd;};
    HANDLE GethHandle(){return hWnd;};
    void SethWnd(HWND k){hWnd = (HANDLE)k;};
    void SetHandle(HANDLE k){hWnd = k;};
    int GetKind(){return Kind;};
};

#define OBJECT_KIND_BAD         0//作成に失敗したヤツ. 危険なので取り除いてください,
#define OBJECT_KIND_APPLICATION 1 //最上位
#define OBJECT_KIND_FORM        2 //フォーム
#define OBJECT_KIND_BUTTON      3 //ボタン
#define OBJECT_KIND_GRID        4 //グリッド
#define OBJECT_KIND_PROGRESS    5 //プログラレスバー
#define OBJECT_KIND_STATUS      6 //ステータスバー
#define OBJECT_KIND_LISTBOX     7 //リストボックス
#define OBJECT_KIND_COMBOBOX	8 //コンボボックス
#define OBJECT_KIND_CHECKBUTTON 9 //チェックボタン
#define OBJECT_KIND_RADIOBUTTON 10//ラジオボタン
#define OBJECT_KIND_GROUPBOX    11//グループボックス
#define OBJECT_KIND_RADIOBOX    12//ラジオボックス
#define OBJECT_KIND_TABCONTROL  13//タブコントロール
#define OBJECT_KIND_EDIT		14//エディト
#define OBJECT_KIND_SCROLL      15//スクロールバー
#define OBJECT_KIND_STATICTEXT  16//スタティックテキスト
#define OBJECT_KIND_CHECKBOX    17//チェックボックス
#define OBJECT_KIND_MENU        18//メニュー
#define OBJECT_KIND_TOOLBAR     19//ツールバー

#endif


#endif //LIFE END
