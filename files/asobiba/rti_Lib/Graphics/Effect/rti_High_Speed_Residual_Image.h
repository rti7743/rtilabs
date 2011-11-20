#include "StartWithExeFile.h"
#ifdef HIGH_SPEED_RESIDUAL_IMAGE_LIFE

#ifndef ___HIGH_SPEED_RESIDUAL_IMAGE_EFFECTH
#define ___HIGH_SPEED_RESIDUAL_IMAGE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

//高速残像
class THighSpeedResidualImageEffect
{
private:
	// ========= メンバ ===========
    BMD IOBmd;				 //入出力
	int IOBmdSizeDiv4;		 //入出力画像のサイズを / 4 したもの.(IOは /4 であまりがでない画像しか扱わない.)
	int BufferUse;			 //現在使うべきバッファ
	BYTE **Buffer;			 //バッファ
	// ========= メソッド =======
	void Init();			 //初期化
public:
	THighSpeedResidualImageEffect();
    ~THighSpeedResidualImageEffect();
    bool Set(BMD IOBmd);
	bool Draw();
};

#endif

#endif //LIFE END
