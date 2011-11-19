<html>
<head>
<meta http-equiv="Content-type" content="text/html; charset=utf-8" /><title>正規表現じぇねれーた</title>
<link rel="stylesheet" href="jquery-ui-1.8.11.custom.css" type="text/css" media="all"> 
<style type="text/css" media="screen"><!--	.ui-tooltip {		background:		white;		color:			black;		border: 		2px solid green;	}

	#menudiv #title {
	    font-size: 2em;
	}--></style>

</head>
<body>
<img src="logo.png" /s>
<h1>単語</h1>
改行区切りで単語を入れると、すべての単語にマッチする正規表現を自動的に作成します。<br />
<br />
<textarea id="wordlist" name="wordlist" cols="200" rows="20">
<?php echo $out['wordlist']; ?>
</textarea>
<br />
<input type="submit" name="gen" value="正規表現を作成してみる" /><br />
<br />
<br />
<br />
<span id="showsample" style="background-color: b0e0e0;">サンプルデータをもっと見る。</span><br>

<?php if ($out['regexp'] !== '') { ?>
<h1>正規表現</h1>
<pre>
<?php echo $out['regexp']; ?>
</pre>
<br />


<h1>コード</h1>
<pre>
<?php echo $out['code']; ?>
</pre>
<br />
<a href="">正規表現を自動作成するモジュール PHP regexp_assembleをダウンロードする。</a><br />

<br />
<br />
<br />

<a href="https://github.com/rti7743/Regexp_Assemble_For_PHP">perlのRegexp::Assemble を PHP に移植して、 Regexp Assemble For PHPで動作させています。</a><br />
<a href="https://twitter.com/#!/super_rti">もし、間違った正規表現が表示されるようでしたら、ご連絡ください。</a><br />
<br />
<?php } // $out['regexp'] !== '' ?>
<div id="hidden">
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
<script src="jquery-1.5.1.min.js" type="text/javascript"></script><script src="jquery-ui-1.8.11.custom.min.js" type="text/javascript"></script><script src="jquery.ui.tooltip2.js" type="text/javascript"></script><script type="text/javascript">
$(function(){
	$('#showsample').tooltip2({arrowtype: 'none' ,arrowstyle: 'bottom left', content: $('#menudiv') });});

function OnMenuClick(type) {
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

	//ツールチップを閉じる.
	$('#showsample').tooltip2('hide');
}

</script>
</body>
</html>
