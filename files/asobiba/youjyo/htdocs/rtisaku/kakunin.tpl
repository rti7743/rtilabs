<html><!-- 2ch_X:cookie -->
<head>
	<META http-equiv="Content-Type" content="text/html; charset=x-sjis">
	<title>■ 書き込み確認 ■</title>
</head>
<body bgcolor=#EEEEEE>
<font size=+1 color=#FF0000><b>書きこみ＆クッキー確認</b></font>
<ul>
  <br>
  <br>
  <b> </b><br>
  ホスト<b> 
  <!--?V:YIPAddress-->
  </b><br>
  <br>
  名前： 
  <!--?V:YKakikoName-->
  <br>
  E-mail： 
  <!--?V:YKakikoMail-->
  <br>
  内容：<br>
  <!--?V:YkakikoText-->
  <br>
  <br>
</ul>
<b>
投稿確認<br>
・投稿された内容はコピー、保存、引用、転載等される場合があります。<br>
・投稿に関して発生する責任は全て投稿者に帰します。<br>
</b>
	<form method=POST action="../test/bbs.cgi">
		<input type=hidden name="subject" value="">
		<input TYPE=hidden NAME=FROM  value="<!--?V:YKakikoName-->">
		<input TYPE=hidden NAME=mail  value="<!--?V:YKakikoMail-->">
		<input type=hidden name=MESSAGE value="<!--?V:YkakikoText-->">
		<input type=hidden name=bbs value=<!--?V:YItaName-->>
		<input type=hidden name=key value=<!--?V:YThreadNO-->>
<br>
<input type=submit value="全責任を負うことを承諾して書き込む" name="submit"><br>
</form>
変更する場合は戻るボタンで戻って書き直して下さい。<br><br>

現在、荒らし対策でクッキーを設定していないと書きこみできないようにしています。<br>
<font size=-1>(cookieを設定するとこの画面はでなくなります。)</font><br>
</body></html>
