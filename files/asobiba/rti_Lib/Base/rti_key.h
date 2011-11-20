#ifndef __KEY_H
#define __KEY_H

/********************************************************************************/
//ゲイツのボケがやっていない キー定義.
/********************************************************************************/
#define VK_EXECUTE      0x2B      //Executeキー
#define VK_SNAPSHOT     0x2C      //Print Screenキー (Windows 3.0以降用)
#define VK_INSERT       0x2D      //Insキー
#define VK_DELETE       0x2E      //Delキー
#define VK_HELP 0x2F      //Helpキー
#define VK_0    0x30      //0キー
#define VK_1    0x31      //1キー
#define VK_2    0x32      //2キー
#define VK_3    0x33      //3キー
#define VK_4    0x34      //4キー
#define VK_5    0x35      //5キー
#define VK_6    0x36      //6キー
#define VK_7    0x37      //7キー
#define VK_8    0x38      //8キー
#define VK_9    0x39      //9キー
#define VK_A    0x41      //Aキー
#define VK_B    0x42      //Bキー
#define VK_C    0x43      //Cキー
#define VK_D    0x44      //Dキー
#define VK_E    0x45      //Eキー
#define VK_F    0x46      //Fキー
#define VK_G    0x47      //Gキー
#define VK_H    0x48      //Hキー
#define VK_I    0x49      //Iキー
#define VK_J    0x4A      //Jキー
#define VK_K    0x4B      //Kキー
#define VK_L    0x4C      //Lキー
#define VK_M    0x4D      //Mキー
#define VK_N    0x4E      //Nキー
#define VK_O    0x4F      //Oキー
#define VK_P    0x50      //Pキー
#define VK_Q    0x51      //Qキー
#define VK_R    0x52      //Rキー
#define VK_S    0x53      //Sキー
#define VK_T    0x54      //Tキー
#define VK_U    0x55      //Uキー
#define VK_V    0x56      //Vキー
#define VK_W    0x57      //Wキー
#define VK_X    0x58      //Xキー
#define VK_Y    0x59      //Yキー
#define VK_Z    0x5A      //Zキー
#define VK_F1   0x70      //F1キー
#define VK_F2   0x71      //F2キー
#define VK_F3   0x72      //F3キー
#define VK_F4   0x73      //F4キー
#define VK_F5   0x74      //F5キー
#define VK_F6   0x75      //F6キー
#define VK_F7   0x76      //F7キー
#define VK_F8   0x77      //F8キー
#define VK_F9   0x78      //F9キー
#define VK_F10  0x79      //F10キー
#define VK_F11  0x7A      //F11キー
#define VK_F12  0x7B      //F12キー
#define VK_F13  0x7C      //F13キー
#define VK_F14  0x7D      //F14キー
#define VK_F15  0x7E      //F15キー
#define VK_F16  0x7F      //F16キー
#define VK_F17  0x80     //F17キー
#define VK_F18  0x81     //F18キー
#define VK_F19  0x82     //F19キー
#define VK_F20  0x83     //F20キー
#define VK_F21  0x84     //F21キー
#define VK_F22  0x85     //F22キー
#define VK_F23  0x86     //F23キー
#define VK_F24  0x87     //F24キー

//そのキーが押されているか判断
bool KeyPush(int vkey);
//一回入力を返す
bool KeyPushOne(int vkey);
unsigned long CheckKeyOnes(UINT Code,...);
unsigned long CheckKey(UINT Code,...);

extern int MousePX,MousePY;  //マウスの現在座標
extern int MouseRX,MouseRY;  //マウスの現在座標リアルタイム
extern bool MouseCL,MouseCR;  //マウスが押されている(1回だけ)
extern bool MouseEL,MouseER;  //マウスが押されている
extern unsigned char LastKey;

#endif
