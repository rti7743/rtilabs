/**************************************************************************************/
// Z Buffer
/**************************************************************************************/

#ifndef ___ZBUFFERH
#define ___ZBUFFERH

#include <string.h>
#include <.\Base\rti_fixed.h>

class TZBuffer
{
    int   BufferW,BufferH;
    int   BufferSize;   //ZBufferのサイズ
    Fixed *ZBuffer;     //ZBufferの本体.
public:
    //コンストラクタ
    TZBuffer()
    {
        BufferSize = 0;
        ZBuffer    = NULL;
    }
    //デストラクタ
    ~TZBuffer()
    {
        Destroy();
    }
    //Z Buffer の確保
    bool Alloc(int w,int h)
    {
        Destroy();  //念のため現在使っているバッファをクリア.
        BufferW = w;
        BufferH = h;
        ZBuffer = new Fixed[ w*h ];
        BufferSize = sizeof(Fixed) * w * h;    //バッファサイズ(バイト)

        if (!ZBuffer) return false;
        return true;
    }
    //Z Buffer の初期化
    void Clear()
    {
        //ZBuffer は、めっちゃデカイ値で初期化しておく.
        memset(ZBuffer , 0x7f7f7f7f,BufferSize);
    }
    //Z Buffer の破棄.
    void Destroy()
    {
        delete [] ZBuffer;
		ZBuffer = NULL;
    }
    //自分自信の破棄.
    void Release()
    {
        delete this;
    }
    //ZBuffer の取得(クラスのカプセル化を破壊すると言えば破壊しますね　このメソッド)
    Fixed* GetZBuffer()
    {
        return ZBuffer;
    }
    //指定した位置のバッファの位置を返す.(クラスのカプセル化を　以下同文)
    Fixed* GetZBuffer(int w,int h)
    {
        return (ZBuffer + (h * BufferW) + w);
    }
    //バッファのサイズを返す(バイト単位.)
    int GetSize()
    {
        return BufferSize;
    }
    //1ラインの長さ(バイトにあらず)を返します.
    int GetPitch()
    {
        return BufferW;
    }
};

typedef TZBuffer* LPZBUFFER;

#endif
