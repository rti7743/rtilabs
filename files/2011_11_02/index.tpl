<html>
<head>
<meta http-equiv="Content-type" content="text/html; charset=utf-8" /><title>正規表現じぇねれーた</title>
<link rel="stylesheet" href="jquery-ui-1.8.11.custom.css" type="text/css" media="all"> 
<link rel="stylesheet" href="jquery.snippet.min.css" type="text/css" media="all"> 

<style type="text/css" media="screen"><!--	.ui-tooltip {		background:		white;		color:			black;		border: 		2px solid green;	}

	#menudiv #title {
	    font-size: 2em;
	}--></style>

</head>
<body>
<img src="logo.png" /s>
<h1>正規表現を自動生成します。</h1>
改行区切りで単語を入れると、すべての単語にマッチする正規表現を自動的に作成します。<br />
<br />
<form action="index.php" method="POST">
   <textarea id="wordlist" name="wordlist" cols="200" rows="20"><?php echo $out['wordlist']; ?></textarea>
   <br />
   <input type="submit" name="gen" value="正規表現を作成してみる" /><br />
</form>
<br />
<br />
<br />
<?php if ($out['bad']) { ?>
     <span style="font-size: 3em; color: red;">内部エラーが発生しました。</span><br />
     <img src="Jumping_Humpback_whale.jpg" /><br />
     <br/>
     内部エラーが発生し、正しい正規表現を作れませんでした。<br />
     お詫びに盛大にじゃんぷするクジラの写真をご覧ください。<br />
     <small>画像 wikipedia のクジラの項目より</small>
     <br/>
     <br/>
     <a href="https://twitter.com/#!/super_rti">バグった報告をしてくれたら、それはとっても嬉しいなぁ、って。</a><br />
     <br/>
     <br/>
     <br/>
     <br/>
     <br/>
<?php } // $out['bad']?>
<span id="showsample" style="background-color: b0e0e0;">サンプルデータをもっと見る。</span><br>

<?php if ($out['regexp'] !== '') { ?>
<h1>正規表現</h1>
正規表現を自動生成しました。<br />
<b>
<pre class="code">
<?php echo $out['regexp']; ?>
</pre>
</b>
<br />


<h1>コード</h1>
<pre class="code">
<?php echo $out['code']; ?>
</pre>
<br />
<a href="index.php?download=Regexp_Assemble.php">正規表現を自動作成するモジュール PHP regexp_assembleをダウンロードする。</a><br />

<?php } // $out['regexp'] !== '' ?>
<br />
<br />
<br />
<h1>解説</h1>
perlには<a href="http://search.cpan.org/~dland/Regexp-Assemble-0.35/Assemble.pm">Regexp::Assembleという正規表現を作成してくれるモジュール</a>があります。<br />
これを PHPに移植して、 <a href="https://github.com/rti7743/Regexp_Assemble_For_PHP">Regexp Assemble For PHP</a>なるモジュールを作って見ました。 <br />
<br />
移植の経緯などは <a href="http://events.php.gr.jp/events/show/108">11/12の第57回PHP勉強会@東京</a> とかで話したいと思います。<br />
いろいろ大変でした。。。<br />
<br />
動作環境は、PHP 5.3以上( PHP5.4 推奨 早いよ!! )です。<br />
<a href="https://twitter.com/#!/super_rti">もし、間違った正規表現が表示されるようでしたら、ご連絡ください。</a><br />
<br />
<br />
<div id="hidden" style="display: none;">
    <!-- めにゅー -->
    <div id="menudiv">
    <table>
       <tr>
            <td height="200">
                 <a href="javascript:void(0)" onclick="return OnMenuClick('todoufuken');" ><div>
                 <span id="title">都道府県の正規表現</span><br>
                 全国の都道府県にマッチする<br />正規表現を作成します。<br />
                 </div></a>
            </td>
            <td height="200">
                 <a href="javascript:void(0)" onclick="return OnMenuClick('who');"><div>
                 <span id="title">who系</span><br>
                 'who', 'what', 'where', 'why', 'when'に<br />マッチする正規表を作成します。<br />
                 </div></a>
            </td>
            <td height="200">
                 <a href="javascript:void(0)" onclick="return OnMenuClick('hyadain');"><div>
                 <span id="title">ヒャダイン</span><br>
                 ヒャド系の呪文に<br />マッチする正規表現を作成します。<br />歌手ぢゃない方ね。<br />
                 </div></a>
            </td>
       </tr>
       <tr>
            <td height="200">
                 <a href="javascript:void(0)" onclick="return OnMenuClick('sister');"><div>
                 <span id="title">シスタープリンセス</span><br>
                 シスプリからもう十年目らしいので、<br />兄チャマの呼び方をすべてチェキする正規表現を<br />作ってみます。<br />
                 </div></a>
            </td>
            <td height="200">
                 <a href="javascript:void(0)" onclick="return OnMenuClick('sister_usa');"><div>
                 <span id="title">シスタープリンセス北米版</span><br>
                 北米版の兄の呼び名にマッチする<br />正規表現を作成します。<br />
                 </div></a>
            </td>
            <td height="200">
            </td>
       </tr>
    </table>
    </div>
</div>
<script src="jquery-1.5.1.min.js" type="text/javascript"></script>
<script src="jquery-ui-1.8.11.custom.min.js" type="text/javascript"></script>
<script src="jquery.ui.tooltip2.js" type="text/javascript"></script>
<script src="jquery.snippet.min.js" type="text/javascript"></script> 
<script type="text/javascript">
$(function(){
	$('#showsample').tooltip2({arrowtype: 'none' ,arrowstyle: 'bottom left', content: $('#menudiv') });

	$('.code').snippet("php");
});

function OnMenuClick(type) {
	//ツールチップを閉じる.
	$('#showsample').tooltip2('close');

	if (type == 'todoufuken') {
		$('#wordlist').val("北海道\n青森県\n岩手県\n宮城県\n秋田県\n山形県\n福島県\n茨城県\n栃木県\n群馬県\n埼玉県\n千葉県\n東京都\n神奈川県\n新潟県\n富山県\n石川県\n福井県\n山梨県\n長野県\n岐阜県\n静岡県\n愛知県\n三重県\n滋賀県\n京都府\n大阪府\n兵庫県\n奈良県\n和歌山県\n鳥取県\n島根県\n岡山県\n広島県\n山口県\n徳島県\n香川県\n愛媛県\n高知県\n福岡県\n佐賀県\n長崎県\n熊本県\n大分県\n宮崎県\n鹿児島県\n沖縄県");
	}
	else if (type == 'who') {
		$('#wordlist').val("who\nwhat\nwhere\nwhy\nwhen");
	}
	else if (type == 'zenpou') {
		$('#wordlist').val("crab\ndab\nlob");
	}
	else if (type == 'sister') {
		$('#wordlist').val("お兄ちゃん\nお兄ちゃま\nあにぃ\nお兄様\nおにいたま\n兄上様\nにいさま\nアニキ\n兄くん\n兄君さま\n兄チャマ\n兄や");
	}
	else if (type == 'sister_usa') {
		$('#wordlist').val("big brother\nbrother\nbig bro\ndear brother\nbro-bro\nbrother mine\nelder brother\nbro\nbrother darling\nbeloved brother\nbrother dearest\nmon frere");
	}
	else if (type == 'hyadain') {
		$('#wordlist').val("ヒャド\nヒャダルコ\nヒャダイン\nマヒャド\nマヒャデドス");
	}
}

</script>
</body>
</html>
