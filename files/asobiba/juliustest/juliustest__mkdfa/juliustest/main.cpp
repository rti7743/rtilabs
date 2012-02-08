/*
julius をMS-SAPIレベルぐらいに楽に使うために、我々はもっとハックをしなければならない。


julius「マナの本当の力を思い知れ!!」
*/
#include <Windows.h>
#include "julius-plus/mkdfa.h"



int main()
{
//	_CrtSetBreakAlloc(447);
	{
//julius付属の mkdfa を抽出してみた。
//しかし、オリジナルがそもそもメモリーリークがあるのとと static変数を使いまくっているので関数化する意味はないと思われるが。
//mkdf.plを使わないで済むので、windows系には嬉しいかもね。
		mkdfa mkdfa;
		mkdfa.conv("fruit");
	}

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
return 0;
}