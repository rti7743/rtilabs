<?php
include("Regexp_Assemble.php");

/*
function log($msg)
{
	global $L_SETTING;
	$logfilename = "/tmp/regexp_assemble_for_php_" . date('Ymd') . '.log';

	$isExits = file_exists($logfilename);
	$fp = fopen($logfilename , "ab");
	if ($fp === FALSE)
	{
		return FALSE;
	}
	fwrite($fp ,$msg . "\r\n" );
	fclose($fp);

	//新規に作成したファイルの場合、誰でも書けるように権限を変更する.
	if (!$isExits)
	{
		@chmod($logfilename,0777);
	}
}

if ( isset($_POST['gen']) ) {
   $assemble = new Regexp_Assemble();
   foreach(explode("\n" , $_POST['wordlist']) as $_) {
       $_ = rtrim($_);
       if ($_ === '') continue;
       $assemble->add( $_ );
   }
   $out['regexp'] = $assemble->re();
   
   log("入力単語:\r\n" . $_POST['wordlist'] . "\r\n出力正規表現:\r\n" . $out['regexp']. "\r\n" );


   $out['code'] = '$assemble = new Regexp_Assemble();' . "\n";
   foreach(explode("\n" , $_POST['wordlist']) as $_) {
       $_ = rtrim($_);
       if ($_ === '') continue;
       $out['code'].= '$assemble->add( "' . $_. '" );'
   }
   $out['code'].= 'echo $assemble->re(); //正規表現';

   $out['wordlist'] = $_POST['wordlist'];
}
else if ( isset($_GET['download']) ) {
   if ($_GET['download'] == 'Regexp_Assemble.php') {
      header("Content-Disposition: inline; filename=Regexp_Assemble.php");
      header("Content-Type: text/plan");
      file_get_contents("Regexp_Assemble.php");
      die;
   }
   else {
      echo "いやーん";
      die;
   }
}
else {
   $out['wordlist'] = "who\nwhat\nwhere\nwhy\nwhen\nここに改行区切りで単語を入れよう。\n";
   $out['regexp'] = '';
   $out['code'] = '';
}
*/