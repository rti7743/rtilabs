// RFunctionScript.cpp: RFunctionScript クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RFunctionScript.h"
#include "RConv.h"
#include "Filer.h"
#include "RInputDialog.h"
#include "RExpr.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//構文をパースします。
string Sentence::Parse(string inSentence) throw(RParseException)
{
	bool isNest = false;
	int nestStart = 0;

	int parseStart = 0;
	string runnable;

	string stdOut;

	int size = inSentence.size();
	for( int i = 0; i < size ; i++)
	{
		char c = inSentence[i]; 

		//2Byte 文字だったらスキップ.
		if ( MultiString::isKanji(c , inSentence[i+1]) )
		{
			i ++;		continue;
		}

		if (!isNest)
		{
			if (c == '`')
			{//コマンド置換 開始
				//実行可能文字列に追加.
				runnable += inSentence.substr(parseStart , i - parseStart);
				isNest = true;
				nestStart = i+1;
			}
			else if (c == '|')
			{//パイプ
				runnable += inSentence.substr(parseStart , i - parseStart);
				stdOut = Run(runnable,stdOut);

				//すべてを初期化.
				runnable = "";
				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
			else if (c == ';')
			{//複数構文.
				runnable += inSentence.substr(parseStart , i - parseStart);
				(void) Run(runnable,stdOut);

				//すべてを初期化.
				runnable = "";
				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
		}
		else
		{	
			if (c == '`')
			{//コマンド置換 終了
				Sentence newSentence;
				runnable += newSentence.Parse( inSentence.substr(nestStart , i - nestStart) );

				parseStart = i + 1;
				nestStart = 0;
				isNest = false;
			}
		}
	}
	runnable += inSentence.substr(parseStart , i- parseStart);
	return Run(runnable,stdOut);
}


//一文になってので実行します。
string Sentence::Run(string inRunnable,string inStdOut) throw(RParseException)
{
	//左右の空白スペースの削除.
	string pure = RConv::ltrim( RConv::rtrim(inRunnable) );

	vector<string> param;
	bool isQuart = false;

	//構文解析.
	int size = pure.size();
	for(int i = 0; i < size ; i++)
	{
		//2Byte 文字だったらスキップ.
		if ( MultiString::isKanji(pure[i], pure[i+1]) )
		{
			i ++;		continue;
		}

		//ダブルクウォートだったらダブルクウォーとの終わりまで.
		if ( pure[i] == '"')
		{
			int start = i ;
			for(int p = i+1; p < size ; p++)
			{
				//2Byte 文字だったらスキップ.
				if ( MultiString::isKanji(pure[p], pure[p+1]) )
				{
					p ++;		continue;
				}
				//終わりですか?
				if (pure[p] == '"')
				{
					break;
				}
			}
			//スタックに push
			param.push_back( pure.substr(start + 1, p - start - 1) );

			i = p;
		}
		else if ( ! TRIM_SPACE(pure[i]) )
		{//空白以外が現れたらそこから空白以外があらわれるまで
			int start = i ;
			for(int p = i+1; p < size ; p++)
			{
				//2Byte 文字だったらスキップ.
				if ( MultiString::isKanji(pure[p], pure[p+1]) )
				{
					p ++;		continue;
				}
				//空白終わり?
				if (TRIM_SPACE(pure[p]))
				{
					break;
				}
			}
			//スタックに push
			param.push_back( pure.substr(start , p - start ) );

			i = p;
		}
	}
	//パラメータなし
	if ( param.size() <= 0 ) return "";

	//コマンド以外のパラメータがない場合...
	bool isNonParam = param.size() <= 1;

	//出力するあたい.
	string retStdOut ;
/*
  &VIEW		内臓エディタ
  &EXEC		関連付けを使って実行
  &NISEOLE	関連付けを使って実行 NISEOLE
  &RELOAD	パスの再読み込み
  &BACK		ヒストリーバック
  &NEXT		ヒストリーフォワード
  &UPDIR	アップ
  &TAB		指定されたタブに移動

  &MASK		指定されたマスク
  &CLIP		クリップボードに転送

  &TOW		反対側のウィンドウへ

  &FOR		$MS , ,`&ECHO`

  &EXPR		計算.

  &MARK		初期にマッチするファイルをマーク
  &PRPTY	プロパティを開く
  &ECHO		画面に出力
  &GREP		ファイルより検索
*/
	//ウィンドウ"管理"人
	const WindowsMain*	main = WindowsMain::getMainSystem();
	//ファイラー"管理"人
	const Filer*		filer = Filer::getFiler();
	//タブつきの"管理"人
	FilerSub*		ActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	FilerSub*		NonActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	//現在アクティヴなウィンドウの"管理"人
	Window*		ActiveWindow = ActiveFiler->getActiveWindow();
	Window*		TargetWindow = ActiveFiler->getTargetWindow();
	Window*		NonActiveWindow = NonActiveFiler->getActiveWindow();

	const string command = param[0];
	try
	{
		if (command == "&VIEW")
		{
			if (isNonParam)		ActiveWindow->PreviewOrLs(inStdOut);
			else				ActiveWindow->PreviewOrLs( param[1] );
		}
		else if (command == "&EXEC")
		{
			if (isNonParam)		::ShellExecute( main->getWindowHandle() , NULL , inStdOut.c_str() , NULL , NULL , SW_NORMAL);
			else				::ShellExecute( main->getWindowHandle() , NULL ,  param[1].c_str()  , NULL , NULL , SW_NORMAL);
		}
		else if (command == "&RELOAD")
		{
			ActiveWindow->Refresh();
		}
		else if (command == "&NEXT")
		{
			ActiveWindow->HistoryNext();
		}
		else if (command == "&BACK")
		{
			ActiveWindow->HistoryBack();
		}
		else if (command == "&UPDIR")
		{
			ActiveWindow->UpDirectory();
		}
		else if (command == "&MENU")
		{
			if (isNonParam)		ActiveWindow->PopupMenu(inStdOut);
			else				ActiveWindow->PopupMenu( param[1] );
		}
		else if (command == "&INPUT")
		{
			if ( param.size() <= 1)
			{
				throw RParseException("&INPUT のパラメータは最低でも2つ必要です");
			}
			//入力ダイアログを作成し、入力を求める.
			RInputDialog dia;
			if ( param.size() == 2)		retStdOut = dia.DoModal(param[1] , param[2] , inStdOut);
			else						retStdOut = dia.DoModal(param[1] , param[2] , param[3]);
		}
		else if (command == "&EXPR")
		{
			RExpr expr;
			if (isNonParam)		retStdOut = expr.Expr(inStdOut);
			else				retStdOut = expr.Expr(param[1]);
		}
		else if (command == "&ECHO")
		{
			if (isNonParam)		retStdOut = inStdOut;
			else				retStdOut = param[1];
		}
		else if (command == "&TABCLOSE")
		{
			ActiveFiler->CloseWindow(TargetWindow);
		}
		else if (command == "&STOPTASKS")
		{
			ActiveWindow->StopTasks();
		}
		else if (command == "&MASK")
		{
			if (isNonParam)		ActiveWindow->setMask(inStdOut);
			else				ActiveWindow->setMask( param[1] );
		}
	}
	catch(RException e)
	{
		throw RParseException("コマンド実行中にエラーが発生しました\n%s\n",e.getMessage() );
	}

	return retStdOut;
}




RFunctionScript::RFunctionScript()
{

}

RFunctionScript::~RFunctionScript()
{

}

string RFunctionScript::Interpreter(const string inScript) const throw (RParseException)
{
	Sentence	s;
	return s.Parse( MacroReplace(inScript) );
}

/*
      $F  .....  カーソル位置のファイル名に置き換えられます。
      $W  .....  カーソル位置の拡張子を除いたファイル名に置き換えられます。
      $E  .....  カーソル位置のファイル名の拡張子に置き換えられます。
      $P  .....  カーソルのある方のパス名
      $O  .....      〃    反対側のパス名

                   上記を $f $w $p $o のように小文字で記述すると
                   ショートファイルネームになります。
                   ショートファイルネームが得られなかった場合は
                   大文字で記述した時と同様の動作になります。

                   カーソルが ..（親フォルダ）を指している時
                   ファイル名は NULL（空文字）を返します。

      $K  .....  記述した位置にカーソルをセットしてキー入力ウインドウを
                 開きます。

      $$  .....  $ を表します。

      $JW  ..... 現在のウィンドウ の 横幅 を表します。
      $JH  ..... 現在のウィンドウ の 縦幅 を表します。

	  $Z  .....  マスクの取得.

      $V"env"
          .....  実行時点での環境変数 env に置き換えられます。

                 環境変数部分に全部小文字で afx と指定すると
                 あふの起動パスを得る事が出来ます。(末尾 \ 無し)
                 この場合１文字でも大文字が混じった場合は
                 環境変数 AFX の内容に置き換えられます。

      $0 ～ $9
          .....  変数（0～9）に入っている文字列に置き換えられます。
                 代入は内部命令 &SET で行います。
*/
string RFunctionScript::MacroReplace(const string inScript) const throw(RParseException)
{
	//ウィンドウ"管理"人
	const WindowsMain*	main = WindowsMain::getMainSystem();
	//ファイラー"管理"人
	const Filer*		filer = Filer::getFiler();
	//タブつきの"管理"人
	const FilerSub*		ActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	const FilerSub*		NonActiveFiler = filer->getFilerSub(filer->IsLeftFocus() );
	//現在アクティヴなウィンドウの"管理"人
	const Window*		ActiveWindow = ActiveFiler->getActiveWindow();
	const Window*		NonActiveWindow = NonActiveFiler->getActiveWindow();

	string activeFilename = ActiveWindow->getActiveFilename();

	string ret;
	int parseStart = 0;
	int size = inScript.size();
	if (size <= 0) return inScript;

	const char * aa = inScript.c_str();

	for(int i = 0 ; i < size ; i++)
	{
		if (inScript[i] == '$')
		{
			switch(inScript[i+1])
			{
			//$x 2バイトスキップ.
			case 'F':	//カーソル位置のファイル名に置き換えられます。
			case 'W':	//カーソル位置の拡張子を除いたファイル名に置き換えられます。
			case 'E':	//カーソル位置のファイル名の拡張子に置き換えられます。
			case 'P':	//カーソルのある方のパス名
			case 'O':	//カーソルの反対側のパス名
			case 'w':	//小文字版
			case 'p':	//小文字版
			case 'o':	//小文字版
			case 'K':	//キャッシッジの固定(ここでは、無視します)
			case 'Z':	//マスクの取得.
			case '$':	//$にマッチ.
			case '0':			case '1':			case '2':			case '3':
			case '4':			case '5':			case '6':			case '7':
			case '8':			case '9':
				{
					ret += inScript.substr(parseStart , i -parseStart );
					switch(inScript[i+1])
					{
					case 'F':	//カーソル位置のファイル名に置き換えられます。
						ret += activeFilename.c_str() ;
						break;
					case 'W':	//カーソル位置の拡張子を除いたファイル名に置き換えられます。
						ret += getFilenameOnly(activeFilename.c_str());
						break;
					case 'E':	//カーソル位置のファイル名の拡張子に置き換えられます。
						ret += getExtOly(activeFilename.c_str());
						break;
					case 'P':	//カーソルのある方のパス名
						ret += ActiveWindow->getNowPath() ;
						break;
					case 'O':	//カーソルの反対側のパス名
						ret += NonActiveWindow->getNowPath();
						break;
					case 'w':	//小文字版
						ret += ConvertSortFilename( getFilenameOnly(activeFilename.c_str()) );
						break;
					case 'p':	//小文字版
						ret += ConvertSortFilename( ActiveWindow->getNowPath() );
						break;
					case 'o':	//小文字版
						ret += ConvertSortFilename( NonActiveWindow->getNowPath() );
						break;
					case 'K':	//キャッシッジの固定(ここでは、無視します)
						ret += "$K";
						break;
					case '$':	//$にマッチ.
						ret += "$";
						break;
					case 'Z':	//マスクの取得.
						ret += ActiveWindow->getMask();
						break;
					case '0':			case '1':			case '2':			case '3':
					case '4':			case '5':			case '6':			case '7':
					case '8':			case '9':
						ret += activeFilename.c_str();
						break;
					}
					i = i + 1;
					parseStart = i+1;
				}
				break;
			//$x 3バイトスキップ.
			case 'J':	//幅と高さ
				{
					ret += inScript.substr(parseStart , i -parseStart );
					switch(inScript[i+1])
					{
					case 'J':	//幅と高さ
						{
							RECT rc;
							ActiveWindow->GetWindowRect(&rc);
							switch(inScript[i+2])
							{
							case 'W':
								ret += RConv::itoa( rc.right - rc.left );
								break;
							case 'H':
								ret += RConv::itoa( rc.bottom- rc.top );
								break;
							default:
								throw RParseException("マクロ展開エラー %s の %d 文字目:Jオプションのあとは W / H を指定してください",inScript.c_str() ,i);
								break;
							}
						}
						break;
					}
					i = i + 2;
					parseStart = i+2;
				}
				break;
			default:	//不明.
				throw RParseException("マクロ展開エラー %s の %d 文字目:そんなオプションありません",inScript.c_str() ,i);
				break;
			}
		}
	}

	ret += inScript.substr(parseStart , i - parseStart );
	return ret;
}

