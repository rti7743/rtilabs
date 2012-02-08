/*
julius をMS-SAPIレベルぐらいに楽に使うために、我々はもっとハックをしなければならない。


julius「マナの本当の力を思い知れ!!」
*/
#include <Windows.h>
#include "julius-plus/rule.h"
#include "julius-plus/JuliusPlus.h"



int main()
{
//	_CrtSetBreakAlloc(447);
	{
//julius付属の mkdfa を抽出してみた。
//しかし、オリジナルがそもそもメモリーリークがあるのとと static変数を使いまくっているので関数化する意味はないと思われるが。
//mkdf.plを使わないで済むので、windows系には嬉しいかもね。
		JuliusPlus julius;
		julius.AddRegexp(0,"テスト(さくら|なのは)たん",&julius.Grammer);

		std::ofstream dfa("regexp_test.dfa");
		std::ofstream dict("regexp_test.dict");
		julius.RuleCommit(&dfa,&dict);
	}

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
return 0;
}