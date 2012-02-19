/*
julius をMS-SAPIレベルぐらいに楽に使うために、我々はもっとハックをしなければならない。


julius「マナの本当の力を思い知れ!!」
*/
#include "julius-plus/JuliusPlus.h"
#include "julius-plus/rule.h"


#include <conio.h>
#include <ctype.h>

int main()
{
	{
		JuliusPlus julius;
		julius.Create(NULL);

		//呼びかけは「なのー」で開始します。
		std::list<std::string> yobilist;
		yobilist.push_back("なのー");
//		yobilist.push_back("なの");
		julius.SetYobikake(yobilist);

		//コマンド群です。
		//callbackはまだ意味はありません。
		{
			CallbackDataStruct callback(100);
			julius.AddCommandRegexp(callback,"でんきつけて");
		}
		{
			CallbackDataStruct callback(200);
			julius.AddCommandRegexp(callback,"でんきけして");
		}
		{
			CallbackDataStruct callback(300);
			julius.AddCommandRegexp(callback,"エアコンつけて");
		}
		{
			CallbackDataStruct callback(400);
			julius.AddCommandRegexp(callback,"エアコンけして");
		}
		{
			CallbackDataStruct callback(500);
			julius.AddCommandRegexp(callback,"エアコンまっくす");
		}
		{
			CallbackDataStruct callback(600);
			julius.AddCommandRegexp(callback,"エアコンおふ");
		}
		{
			CallbackDataStruct callback(700);
			julius.AddCommandRegexp(callback,"おんがくかけて");
		}
		{
			CallbackDataStruct callback(800);
			julius.AddCommandRegexp(callback,"どいつむら");
		}
		{
			CallbackDataStruct callback(900);
			julius.AddCommandRegexp(callback,"えどむら");
		}


		julius.CommitRule();
		::_getch();

	}
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
