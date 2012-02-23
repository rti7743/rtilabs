/*
julius をMS-SAPIレベルぐらいに楽に使うために、我々はもっとハックをしなければならない。


julius「マナの本当の力を思い知れ!!」
*/
#include "julius-plus/JuliusPlus.h"
#include "julius-plus/JuliusPlusRule.h"


int main()
{
	{
		JuliusPlus julius;
		julius.Create(NULL);

		//採用する認識率と 二重検知を有効にするかを設定します。
		julius.SetRecognitionFilter(0.70,0.70,0.70,true);

		//呼びかけは「コンピュータ」で開始します。
		std::list<std::string> yobilist;
		yobilist.push_back("コンピューター");
		yobilist.push_back("コンピュータ");
		julius.SetYobikake(yobilist);

		//コマンド群です。
		julius.AddCommandRegexp("でんき(つけて|オン)",CallbackDataStruct([=](){
			//電気をつける処理をここに書きます。

			std::cout << "###RUN>でんきつけるよ!!" << std::endl;
		}));
		julius.AddCommandRegexp("でんき(けして|オフ)",CallbackDataStruct([=](){
			//電気を消す処理をここに書きます。
			std::cout << "###RUN>でんき消すよ!!" << std::endl;
		}));
		julius.AddCommandRegexp("エアコン(つけて|オン)",CallbackDataStruct([=](){
			//エアコンをつける処理をここに書きます。
			std::cout << "###RUN>エアコンつけるよ!!" << std::endl;
		}));
		julius.AddCommandRegexp("エアコン(けして|オフ)",CallbackDataStruct([=](){
			//エアコンを消す処理をここに書きます。
			std::cout << "###RUN>エアコン消すよ!!" << std::endl;
		}));
		julius.AddCommandRegexp("おんがくかけて",CallbackDataStruct([=](){
			//音楽をかける処理をここに書きます。
			std::cout << "###RUN>おんがくかけるよ" << std::endl;
		}));
		julius.AddCommandRegexp("どいつむら",CallbackDataStruct([=](){
			//日本で一番有名なテーマパークに関する処理をここに書きます。
			std::cout << "###RUN>日本で一番有名なテーマパーク" << std::endl;
		}));
		julius.AddCommandRegexp("(くまぽん|くーまぽん|くまぽーん)",CallbackDataStruct([=](){
			//くまぽんに関する処理をここに書きます。
			std::cout << "###RUN>( ・(エ)・)　くまぽーん" << std::endl;
		}));

		julius.CommitRule();

		//キー入力待ち
		std::string line;
		getline( std::cin, line );

		std::cout << "終了させます。" << std::endl;
	}

//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}
