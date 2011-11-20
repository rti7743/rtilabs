#include <windows.h>
#include <.\Base\rti_key.h>

#define KEY_MAX 256
int MousePX,MousePY;  //マウスの現在座標
int MouseRX,MouseRY;  //マウスの現在座標リアルタイム
bool MouseCL,MouseCR;  //マウスが押されている(1回だけ)
bool MouseEL,MouseER;  //マウスが押されている
unsigned char LastKey;


unsigned long CheckKey(UINT Code,...)
{
	int i;    // 汎用カウンター
	unsigned long Key; // 押されたキーコード
    unsigned long ret = 0;

	for (i=0;*(&Code+i)!=NULL;i++)
	{
		Key=*(&Code+i);
		if (GetAsyncKeyState(Key) & 0x8000)
		{
            ret |= 0x01 << i;
		}
	}
    return ret;

}

unsigned long CheckKeyOnes(UINT Code,...)
{
	int i;    // 汎用カウンター
	unsigned long Key; // 押されたキーコード
    unsigned long ret = 0;

	for (i=0;*(&Code+i)!=NULL;i++)
	{
		Key=*(&Code+i);
		if (  (GetAsyncKeyState(Key) & 0x8001) == 0x8000   )
		{
            ret |= 0x01 << i;
		}
	}
    return ret;

}

//そのキーが押されているか判断
bool KeyPush(int vkey)
{
    return (bool)(GetAsyncKeyState(vkey) & 0x8000);
}

//一回入力を返す
bool KeyPushOne(int vkey)
{
    return ( (GetAsyncKeyState(vkey) & 0x8001) == 0x8000 );
}


