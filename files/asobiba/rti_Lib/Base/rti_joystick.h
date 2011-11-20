#include <StartWithExeFile.h>
#include <windows.h>

#ifdef JOY_STICK_LIFE

#ifndef ___JOY_STICKH
#define ___JOY_STICKH

#define JOY_POS_MIN		1000
#define JOY_POS_MAX		55000
//方向情報
#define JOY_POS_RIGHT	0x02 //右
#define JOY_POS_LEFT	0x01 //左
#define JOY_POS_TOP		0x08 //上
#define JOY_POS_DOWN	0x04 //下
#define JOY_POS_NN      0x00 //ニュートラル
//ボタン情報
#define JOY_BTN_A	0x01 //Aボタン
#define JOY_BTN_B	0x02 //Bボタン
#define JOY_BTN_C	0x04 //Cボタン
#define JOY_BTN_D	0x08 //Dボタン
//方向とボタンのパック.
//32  16 8 4 2 1	方向の上にキーがくる.
//B   A  U D R L
#define JOY_PACK_RIGHT	0x02 //右
#define JOY_PACK_LEFT	0x01 //左
#define JOY_PACK_TOP	0x08 //上
#define JOY_PACK_DOWN	0x04 //下
#define JOY_PACK_NN     0x00 //ニュートラル
#define JOY_PACK_A		0x10 //Aボタン
#define JOY_PACK_B		0x20 //Bボタン
#define JOY_PACK_C		0x40 //Cボタン
#define JOY_PACK_D		0x80 //Dボタン

#define JOY_PACK_BUTTOM_MASK	0xfffffff0
#define JOY_PACK_STICK_MASK		0x0f

class TJoyStick
{
private:
	unsigned int PadId;  //パッドID
	JOYINFOEX JoyInfo;

	//ジョイスティックは使える
	bool bPadAlive;
	//方向キーダウンのビット情報
	BYTE JoyKeyDown;
	//前回の方向キーダウンのビット情報
	BYTE JoyKeyDownOld;

	//前回のボタンキーダウンのビット情報
	DWORD JoyButtonDownOld;
public:

	TJoyStick(unsigned int UsePad,bool Sailent);
	MMRESULT JoyStickInfo(bool Sailent);
	void JoyStickRef();

	//パッドが有効かどうか調べる
	bool PadAlive(){return bPadAlive;};
	//ボタンを取得する
	DWORD ButtonGet(){return JoyInfo.dwButtons;}
	//前回は押されていなくて今回初めて押されたボタンを取得する
	DWORD ButtonOnesGet()
		{return (JoyInfo.dwButtons & (~JoyButtonDownOld) );};
	//方向を取得する
	BYTE KeyGet(){return JoyKeyDown;};
	//前回は押されていなくて今回初めて押されたやつ方向
	BYTE KeyOnesGet(){return (JoyKeyDown & (~JoyKeyDownOld) );};

	//ボタンと方向を DWORD につめて返します.
	DWORD PackKeyGet(){return JoyKeyDown | (JoyInfo.dwButtons << 4);};
	//ボタンと方向を DWORD につめて返します.(前回押されていない.)
	DWORD PackKeyGetOne(){
		return (JoyKeyDown & (~JoyKeyDownOld) ) | (JoyInfo.dwButtons & (~JoyButtonDownOld) );};
};

typedef TJoyStick* JSC;

#endif

#endif //LIFE END
