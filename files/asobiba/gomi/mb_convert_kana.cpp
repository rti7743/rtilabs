////////////
//ICUなんて使わないガチンコ実装。
//もしかしたら、オリジナルと一部違うところがあるかも・・・
//バグがあっても泣かない。

//みんな大好き PHPのmb_convert_kanaの移植
//n	 「全角」数字を「半角」に変換します。
//N	 「半角」数字を「全角」に変換します。
//a	 「全角」英数字を「半角」に変換します。
//A	 「半角」英数字を「全角」に変換します 
//s	 「全角」スペースを「半角」に変換します（U+3000 -> U+0020）。
//S	 「半角」スペースを「全角」に変換します（U+0020 -> U+3000）。
//k	 「全角カタカナ」を「半角カタカナ」に変換します。
//K	 「半角カタカナ」を「全角カタカナ」に変換します。
//h	 「全角ひらがな」を「半角カタカナ」に変換します。
//H	 「半角カタカナ」を「全角ひらがな」に変換します。
//c	 「全角カタカナ」を「全角ひらがな」に変換します。
//C	 「全角ひらがな」を「全角カタカナ」に変換します。
std::string XLStringUtil::mb_convert_kana(const std::string &inTarget,const std::string& option)
{
	std::string ret = inTarget;
	static const char *replaceTableAplha[] = {
		 "Ａ","A"
		,"Ｂ","B"
		,"Ｃ","C"
		,"Ｄ","D"
		,"Ｅ","E"
		,"Ｆ","F"
		,"Ｇ","G"
		,"Ｈ","H"
		,"Ｉ","I"
		,"Ｊ","J"
		,"Ｋ","K"
		,"Ｌ","L"
		,"Ｍ","M"
		,"Ｎ","N"
		,"Ｏ","O"
		,"Ｐ","P"
		,"Ｑ","Q"
		,"Ｒ","R"
		,"Ｓ","S"
		,"Ｔ","T"
		,"Ｕ","U"
		,"Ｖ","V"
		,"Ｗ","W"
		,"Ｘ","X"
		,"Ｙ","Y"
		,"Ｚ","Z"
		,"ａ","a"
		,"ｂ","b"
		,"ｃ","c"
		,"ｄ","d"
		,"ｅ","e"
		,"ｆ","f"
		,"ｇ","g"
		,"ｈ","h"
		,"ｉ","i"
		,"ｊ","j"
		,"ｋ","k"
		,"ｌ","l"
		,"ｍ","m"
		,"ｎ","n"
		,"ｏ","o"
		,"ｐ","p"
		,"ｑ","q"
		,"ｒ","r"
		,"ｓ","s"
		,"ｔ","t"
		,"ｕ","u"
		,"ｖ","v"
		,"ｗ","w"
		,"ｘ","x"
		,"ｙ","y"
		,"ｚ","z"
		,"ｰ","ー"
		,"‘","'"
		,"’","'"
		,"“","\""
		,"”","\""
		,"（","("
		,"）",")"
		,"〔","["
		,"〕","]"
		,"［","["
		,"］","]"
		,"｛","{"
		,"｝","}"
		,"〈","<"
		,"〉",">"
		,"《","<"
		,"》",">"
		,"「","{"
		,"」","}"
		,"『","{"
		,"』","}"
		,"【","["
		,"】","]"
		,"・","･"
		,"！","!"
		,"♯","#"
		,"＆","&"
		,"＄","$"
		,"？","?"
		,"：",":"
		,"；",";"
		,"／","/"
		,"＼","\\"
		,"＠","@"
		,"｜","|"
		,"－","-"
		,"＝","="
		,"≒","="
		,"％","%"
		,"＋","+"
		,"－","-"
		,"÷","/"
		,"＊","*"
		,NULL,NULL
	};
//r	 「全角」英字を「半角」に変換します。
//R	 「半角」英字を「全角」に変換します。
//a	 「全角」英数字を「半角」に変換します。
//A	 「半角」英数字を「全角」に変換します 
	if ( option.find("r") != -1 ||   option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,false );
	}
	else if ( option.find("R") != -1 ||  option.find("A") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableAplha,true );
	}

	static const char *replaceTableNum[] = {
		 "１","1"
		,"２","2"
		,"３","3"
		,"４","4"
		,"５","5"
		,"６","6"
		,"７","7"
		,"８","8"
		,"９","9"
		,"０","0"
		,NULL,NULL
	};
//n	 「全角」数字を「半角」に変換します。
//N	 「半角」数字を「全角」に変換します。
//a	 「全角」英数字を「半角」に変換します。
//A	 「半角」英数字を「全角」に変換します 
	if ( option.find("n") != -1 ||  option.find("a") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,false );
	}
	else if ( option.find("N") != -1 ||  option.find("A") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableNum,true );
	}

	static const char *replaceTableSpace[] = {
		 "　"," "
		,NULL,NULL
	};
//s	 「全角」スペースを「半角」に変換します
//S	 「半角」スペースを「全角」に変換します
	if ( option.find("s") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,false );
	}
	else if ( option.find("S") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableSpace,true );
	}

	const char *replaceTableHankanaToHiragana[] = {
		 "ｳﾞ","う゛"
		,"ｶﾞ","が"
		,"ｷﾞ","ぎ"
		,"ｸﾞ","ぐ"
		,"ｹﾞ","げ"
		,"ｺﾞ","ご"
		,"ｻﾞ","ざ"
		,"ｼﾞ","じ"
		,"ｽﾞ","ず"
		,"ｾﾞ","ぜ"
		,"ｿﾞ","ぞ"
		,"ﾀﾞ","だ"
		,"ﾁﾞ","ぢ"
		,"ｽﾞ","ず"
		,"ｾﾞ","ぜ"
		,"ｿﾞ","ぞ"
		,"ﾊﾞ","ば"
		,"ﾋﾞ","び"
		,"ﾌﾞ","ぶ"
		,"ﾍﾞ","べ"
		,"ﾎﾞ","ぼ"
		,"ﾊﾟ","ぱ"
		,"ﾋﾟ","ぴ"
		,"ﾌﾟ","ぷ"
		,"ﾍﾟ","ぺ"
		,"ﾎﾟ","ぽ"
		,"ｱ","あ"
		,"ｲ","い"
		,"ｳ","う"
		,"ｴ","え"
		,"ｵ","お"
		,"ｶ","か"
		,"ｷ","き"
		,"ｸ","く"
		,"ｹ","け"
		,"ｺ","こ"
		,"ｻ","さ"
		,"ｼ","し"
		,"ｽ","す"
		,"ｾ","せ"
		,"ｿ","そ"
		,"ﾀﾞ","だ"
		,"ﾁﾞ","ぢ"
		,"ﾂﾞ","づ"
		,"ﾃﾞ","で"
		,"ﾄﾞ","ど"
		,"ﾅ","な"
		,"ﾆ","に"
		,"ﾇ","ぬ"
		,"ﾈ","ね"
		,"ﾉ","の"
		,"ﾊ","は"
		,"ﾋ","ひ"
		,"ﾌ","ふ"
		,"ﾍ","へ"
		,"ﾎ","ほ"
		,"ﾏ","ま"
		,"ﾐ","み"
		,"ﾑ","む"
		,"ﾒ","め"
		,"ﾓ","も"
		,"ﾔ","や"
		,"ﾕ","ゆ"
		,"ﾖ","よ"
		,"ﾗ","ら"
		,"ﾘ","り"
		,"ﾙ","る"
		,"ﾚ","れ"
		,"ﾛ","ろ"
		,"ｦ","を"
		,"ﾜ","わ"
		,"ﾝ","ん"
		,"ｧ","ぁ"
		,"ｨ","ぃ"
		,"ｩ","ぅ"
		,"ｪ","ぇ"
		,"ｫ","ぉ"
		,"ｬ","ゃ"
		,"ｭ","ゅ"
		,"ｮ","ょ"
		,"ｯ","っ"
		,"ｰ","ー"
		,NULL,NULL
	};
	const char *replaceTableKatakanaToHiragana[] = {
		 "ヴ","う゛"
		,"ア","あ"
		,"イ","い"
		,"ウ","う"
		,"エ","え"
		,"オ","お"
		,"カ","か"
		,"キ","き"
		,"ク","く"
		,"ケ","け"
		,"コ","こ"
		,"サ","さ"
		,"シ","し"
		,"ス","す"
		,"セ","せ"
		,"ソ","そ"
		,"タ","た"
		,"チ","ち"
		,"ツ","つ"
		,"テ","て"
		,"ト","と"
		,"ナ","な"
		,"ニ","に"
		,"ヌ","ぬ"
		,"ネ","ね"
		,"ノ","の"
		,"ハ","は"
		,"ヒ","ひ"
		,"フ","ふ"
		,"ヘ","へ"
		,"ホ","ほ"
		,"マ","ま"
		,"ミ","み"
		,"ム","む"
		,"メ","め"
		,"モ","も"
		,"ヤ","や"
		,"ユ","ゆ"
		,"ヨ","よ"
		,"ラ","ら"
		,"リ","り"
		,"ル","る"
		,"レ","れ"
		,"ロ","ろ"
		,"ワ","わ"
		,"ヲ","を"
		,"ン","ん"
		,"ァ","ぁ"
		,"ィ","ぃ"
		,"ゥ","ぅ"
		,"ェ","ぇ"
		,"ォ","ぉ"
		,"ガ","が"
		,"ギ","ぎ"
		,"グ","ぐ"
		,"ゲ","げ"
		,"ゴ","ご"
		,"ザ","ざ"
		,"ジ","じ"
		,"ズ","ず"
		,"ゼ","ぜ"
		,"ゾ","ぞ"
		,"ダ","だ"
		,"ヂ","ぢ"
		,"ヅ","づ"
		,"デ","で"
		,"ド","ど"
		,"バ","ば"
		,"ビ","び"
		,"ブ","ぶ"
		,"ベ","べ"
		,"ボ","ぼ"
		,"パ","ぱ"
		,"ピ","ぴ"
		,"プ","ぷ"
		,"ペ","ぺ"
		,"ポ","ぽ"
		,"ャ","ゃ"
		,"ュ","ゅ"
		,"ョ","ょ"
		,"ッ","っ"
		,"ヮ","ゎ"
		,NULL,NULL
	};
//k	 「全角カタカナ」を「半角カタカナ」に変換します。
//K	 「半角カタカナ」を「全角カタカナ」に変換します。
	if ( option.find("k") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,false );
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,true );
	}
	else if ( option.find("K") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,false );
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,true );
	}

//c	 「全角カタカナ」を「全角ひらがな」に変換します。
//C	 「全角ひらがな」を「全角カタカナ」に変換します。
	if ( option.find("c") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,false );
	}
	else if ( option.find("C") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableKatakanaToHiragana,true );
	}

//h	 「全角ひらがな」を「半角カタカナ」に変換します。
//H	 「半角カタカナ」を「全角ひらがな」に変換します。
	if ( option.find("h") != -1 )
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,true );
	}
	else if ( option.find("H") != -1)
	{
		ret = XLStringUtil::replace(ret ,replaceTableHankanaToHiragana,false );
	}

	return ret;
}

//////////////////
//おまけ
//////////////////

//ローマ字をひらがなにします。
std::string XLStringUtil::RomajiToHiragana(const std::string &inTarget)
{
	static const char *replaceTableRoma[] = {
		 "ltsu","っ"
		,"whu","う"
		,"lyi","ぃ"
		,"xyi","ぃ"
		,"lye","ぇ"
		,"xye","ぇ"
		,"wha","うぁ"
		,"whi","うぃ"
		,"whe","うぇ"
		,"who","うぉ"
		,"kyi","きぃ"
		,"kye","きぇ"
		,"kya","きゃ"
		,"kyu","きゅ"
		,"kyo","きょ"
		,"kwa","くぁ"
		,"qwa","くぁ"
		,"qwi","くぃ"
		,"qyi","くぃ"
		,"qwu","くぅ"
		,"qwe","くぇ"
		,"qye","くぇ"
		,"qwo","くぉ"
		,"qya","くゃ"
		,"qyu","くゅ"
		,"qyo","くょ"
		,"syi","しぃ"
		,"swi","しぇ"
		,"sha","しゃ"
		,"shu","しゅ"
		,"sho","しょ"
		,"syi","しぇ"
		,"sya","しゃ"
		,"syu","しゅ"
		,"syo","しょ"
		,"shi","し"
		,"swa","すぁ"
		,"swi","すぃ"
		,"swu","すぅ"
		,"swe","すぇ"
		,"swo","すぉ"
		,"cyi","ちぃ"
		,"tyi","ちぃ"
		,"che","ちぇ"
		,"cye","ちぇ"
		,"tye","ちぇ"
		,"cha","ちゃ"
		,"cya","ちゃ"
		,"tya","ちゃ"
		,"chu","ちゅ"
		,"cyu","ちゅ"
		,"tyu","ちゅ"
		,"cho","ちょ"
		,"cyo","ちょ"
		,"tyo","ちょ"
		,"chi","ち"
		,"tsa","つぁ"
		,"tsi","つぃ"
		,"tse","つぇ"
		,"tso","つぉ"
		,"tsu","つ"
		,"ltu","っ"
		,"xtu","っ"
		,"thi","てぃ"
		,"the","てぇ"
		,"tha","てゃ"
		,"thu","てゅ"
		,"tho","てょ"
		,"twa","とぁ"
		,"twi","とぃ"
		,"twu","とぅ"
		,"twe","とぇ"
		,"two","とぉ"
		,"nyi","にぃ"
		,"nye","にぇ"
		,"nya","にゃ"
		,"nyu","にゅ"
		,"nyo","にょ"
		,"hyi","ひぃ"
		,"hye","ひぇ"
		,"hya","ひゃ"
		,"hyu","ひゅ"
		,"hyo","ひょ"
		,"fwa","ふぁ"
		,"fwi","ふぃ"
		,"fyi","ふぃ"
		,"fwu","ふぅ"
		,"few","ふぇ"
		,"fye","ふぇ"
		,"fwo","ふぉ"
		,"fya","ふゃ"
		,"fyu","ふゅ"
		,"fyo","ふょ"
		,"myi","みぃ"
		,"mye","みぇ"
		,"mya","みゃ"
		,"myu","みゅ"
		,"myo","みょ"
		,"lya","ゃ"
		,"xya","ゃ"
		,"lyu","ゅ"
		,"xyu","ゅ"
		,"lyo","ょ"
		,"xyo","ょ"
		,"ryi","りぃ"
		,"rye","りぇ"
		,"rya","りゃ"
		,"ryu","りゅ"
		,"ryo","りょ"
		,"gyi","ぎぃ"
		,"gye","ぎぇ"
		,"gya","ぎゃ"
		,"gyu","ぎゅ"
		,"gyo","ぎょ"
		,"gwa","ぐぁ"
		,"gwi","ぐぃ"
		,"gwu","ぐぅ"
		,"gwe","ぐぇ"
		,"gwo","ぐぉ"
		,"jyi","じぃ"
		,"zyi","じぃ"
		,"jye","じぇ"
		,"zye","じぇ"
		,"jya","じゃ"
		,"zya","じゃ"
		,"lwa","ゎ"
		,"xwa","ゎ"
		,"jyu","じゅ"
		,"zyu","じゅ"
		,"jyo","じょ"
		,"zyo","じょ"
		,"dyi","ぢぃ"
		,"dye","ぢぇ"
		,"dya","ぢゃ"
		,"dyu","ぢゅ"
		,"dyo","ぢょ"
		,"dhi","でぃ"
		,"dhe","でぇ"
		,"dha","でゃ"
		,"dhu","でゅ"
		,"dho","でょ"
		,"dwa","どぁ"
		,"dwi","どぃ"
		,"dwu","どぅ"
		,"dwe","どぇ"
		,"dwo","どぉ"
		,"byi","びぃ"
		,"bye","びぇ"
		,"bya","びょ"
		,"byu","びゅ"
		,"byo","びょ"
		,"pyi","ぴぃ"
		,"pye","ぴぇ"
		,"pya","ぴゃ"
		,"pyu","ぴゅ"
		,"pyo","ぴょ"
		,"lka","か" //ヵ
		,"xka","か" //ヵ
		,"lke","け" //ヶ
		,"xke","け" //ヶ
		,"vyi","う゛ぃ"
		,"vye","う゛ぇ"
		,"vya","う゛ゃ"
		,"vyu","う゛ゅ"
		,"vyo","う゛ょ"
		,"wu","う"
		,"la","ぁ"
		,"li","ぃ"
		,"xi","ぃ"
		,"lu","ぅ"
		,"xu","ぅ"
		,"le","ぇ"
		,"xe","ぇ"
		,"lo","ぉ"
		,"xo","ぉ"
		,"ye","いぇ"
		,"ka","か"
		,"ca","か"
		,"ki","き"
		,"qa","くぁ"
		,"qi","くぃ"
		,"qe","くぇ"
		,"qo","くぉ"
		,"ku","く"
		,"cu","く"
		,"qu","く"
		,"ke","け"
		,"ko","こ"
		,"co","こ"
		,"sa","さ"
		,"si","し"
		,"ci","し"
		,"su","す"
		,"se","せ"
		,"ce","せ"
		,"so","そ"
		,"ta","た"
		,"ti","ち"
		,"tu","つ"
		,"te","て"
		,"to","と"
		,"na","な"
		,"ni","に"
		,"nu","ぬ"
		,"ne","ね"
		,"no","の"
		,"ha","は"
		,"hi","ひ"
		,"fa","ふぁ"
		,"fa","ふぁ"
		,"fi","ふぃ"
		,"fe","ふぇ"
		,"fo","ふぉ"
		,"hu","ふ"
		,"fu","ふ"
		,"he","へ"
		,"ho","ほ"
		,"ma","ま"
		,"mi","み"
		,"mu","む"
		,"me","め"
		,"mo","も"
		,"ya","や"
		,"yu","ゆ"
		,"yo","よ"
		,"ra","ら"
		,"ri","り"
		,"ru","る"
		,"re","れ"
		,"wa","わ"
		,"wo","を"
		,"nn","ん"
		,"xn","ん"
		,"ga","が"
		,"gi","ぎ"
		,"gu","ぐ"
		,"ge","げ"
		,"go","ご"
		,"za","ざ"
		,"je","じぇ"
		,"ja","じゃ"
		,"ju","じゅ"
		,"jo","じょ"
		,"zi","じ"
		,"ji","じ"
		,"zu","ず"
		,"ze","ぞ"
		,"zo","ぞ"
		,"ji","じ"
		,"da","だ"
		,"di","ぢ"
		,"du","づ"
		,"de","で"
		,"do","ど"
		,"ba","ば"
		,"bi","び"
		,"bu","ぶ"
		,"be","べ"
		,"bo","ぼ"
		,"pa","ぱ"
		,"pi","ぴ"
		,"pu","ぷ"
		,"pe","ぺ"
		,"po","ぽ"
		,"va","う゛ぁ"
		,"vi","う゛ぃ"
		,"ve","う゛ぇ"
		,"vo","う゛ぉ"
		,"vu","う゛"
		,"a","あ"
		,"i","い"
		,"u","う"
		,"e","え"
		,"o","お"
		,NULL,NULL
	};
	return XLStringUtil::replace(inTarget ,replaceTableRoma,false );
}

//かな入力の人がかなを入れ忘れたときに入力される文字からひらがなにします。
std::string XLStringUtil::KanaTypoHiragana(const std::string &inTarget)
{
	static const char *replaceTableRoma[] = {
		 "4@","う゛"
		,"a","あ"
		,"e","い"
		,"4","う"
		,"5","え"
		,"6","お"
		,"t","か"
		,"g","き"
		,"h","く"
		,":","け"
		,"b","こ"
		,"x","さ"
		,"d","し"
		,"r","す"
		,"p","せ"
		,"c","そ"
		,"q","た"
		,"a","ち"
		,"z","つ"
		,"w","て"
		,"s","と"
		,"u","な"
		,"i","に"
		,"1","ぬ"
		,",","ね"
		,"k","の"
		,"f","は"
		,"v","ひ"
		,"2","ふ"
		,"^","へ"
		,"-","ほ"
		,"j","ま"
		,"n","み"
		,"]","む"
		,"/","め"
		,"m","も"
		,"7","や"
		,"8","ゆ"
		,"9","よ"
		,"o","ら"
		,"l","り"
		,".","る"
		,";","れ"
		,"\\","ろ"
		,"0","わ"
		//,"","を"
		,"y","ん"
		,"#","ぁ"
		,"E","ぃ"
		,"$","ぅ"
		,"%","ぇ"
		,"&","ぉ"
		,"t@","が"
		,"g@","ぎ"
		,"h@","ぐ"
		,":@","げ"
		,"b@","ご"
		,"x@","ざ"
		,"d@","じ"
		,"r@","ず"
		,"p@","ぜ"
		,"c@","ぞ"
		,"q@","だ"
		,"a@","ぢ"
		,"z@","づ"
		,"w@","で"
		,"s@","ど"
		,"f@","ば"
		,"v@","び"
		,"2@","ぶ"
		,"^@","べ"
		,"-@","ぼ"
		,"f[","ぱ"
		,"v[","ぴ"
		,"2[","ぷ"
		,"^[","ぺ"
		,"-[","ぽ"
		,"'","ゃ"
		,"(","ゅ"
		,")","ょ"
		,"Z","っ"
		//,"ヮ","ゎ"
		,NULL,NULL
	};
	return XLStringUtil::replace(inTarget ,replaceTableRoma,false );
}



////////////
//ベース
////////////

//strstrのマルチバイトセーフ 文字列検索
const char* XLStringUtil::strstr(const std::string& target, const std::string & need )
{
#ifdef _WINDOWS
	//SJISだとこんな感じかな・・・
	return (const char*) _mbsstr( (unsigned char*) target.c_str() ,(unsigned char*) need.c_str() );
#else
	//UTF-8だと仮定してやるよw
	return strstr( target.c_str() ,need.c_str() );
#endif
}


//マルチバイト対応 なぜか std::string に標準で用意されていない置換。ふぁっく。
std::string XLStringUtil::replace(const std::string &inTarget ,const std::string &inOld ,const std::string &inNew)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//先読み.

	const char * p = inTarget.c_str();
	const char * match;
	while( match = XLStringUtil::strstr( p , inOld.c_str() ) )
	{
		//ret += std::string(p,0,(int)(match - p));
		ret.append(p,(int)(match - p));
		ret += inNew;

		p = match + inOld.size();
	}
	//残りを足しておしまい.
	return ret + p;
}



//マルチバイト対応 複数の候補を一括置換 const char * replacetable[] = { "A","あ"  ,"I","い"  , "上","うえ" , NULL , NULL}  //必ず2つ揃えで書いてね
std::string XLStringUtil::replace(const std::string &inTarget ,const char** replacetable,bool isrev)
{
	std::string ret;
	ret.reserve( inTarget.size() );	//先読み.

	if (inTarget.empty())
	{
		return inTarget;
	}

	const char * p = inTarget.c_str();
	for(; *p ; )
	{
		const char * pp = p;
		p = nextChar(p);

		int compareIndex = isrev == false ? 0 : 1;
		int replaceIndex = isrev == false ? 1 : 0;
		const char ** r1 = replacetable;
		for( ; *r1 != NULL ; r1+=2)
		{
			const char * ppp = pp;
			const char * r2 = *(r1+compareIndex);
			for( ; 1 ; ++r2,++ppp )
			{
				if ( *r2 == NULL || *ppp != *r2)
				{
					break;
				}
			}
			if (*r2 == NULL)  //無事比較文字列の方が終端にたどりついた
			{
				ret.append(*(r1+replaceIndex));
				p = ppp;
				break;
			}
		}
		if ( *r1 == NULL )
		{
			ret.append(pp,(int) (p - pp));
		}
	}
	return ret;
}