<?php
require_once('Regexp_Assemble.php');

function mylog($msg)
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
//危険な文字を回避
function h($text)
{
	return htmlspecialchars($text,ENT_QUOTES,'UTF-8');
}

//_REQUESTの値をそのままユーザーに返せるように変換します.
function hh(&$array)
{
	if (!is_array($array))
	{
		if (is_object($array))
		{
			return array();
		}
		return h($array);
	}
	else
	{
		$newArray = array();
		foreach($array as $key => $value)
		{
			if ($key[0] == '&')
			{
				$newArray[$key] = $value;
			}
			else
			{
				$newArray[$key] = hh($value);
			}
		}
		return $newArray;
	}
}
function _perl_quotemeta($str) {
//   foreach( array('\\' , '.', '+', '*', '?', '[', '^', ']', '$', '(', ')', '{', '}', '=', '!', '|', ':', '-', '/') 
   foreach( array('\\' , '.', '+', '*', '?', '[', '^', ']', '$', '(', ')', '{', '}', '=', '!', '|', ':', '-', '/') 
            as $meta) {
       $str = str_replace($meta , "\\{$meta}" , $str  );
   }
   
   return $str;
}

function _singleqoute($str) {
   return str_replace("'" , "\'" , $str  );
}


function _cleaningMojoCode($str) {
   $str = mb_convert_encoding($str , 'UTF-16','UTF-8');
   $str = mb_convert_encoding($str , 'UTF-8','UTF-16');
   return $str;
}

if ( isset($_REQUEST['gen']) ) {
   //文字コードを洗う.
   $_REQUEST['wordlist'] = _cleaningMojoCode($_REQUEST['wordlist']);
   $list = array();
   foreach(explode("\n" ,$_REQUEST['wordlist'] ) as $_) {
       $_ = rtrim($_);
       if ($_ === '') continue;
       $list[] = $_;
   }
   
   if (count($list) <= 0) {
       $out['wordlist'] = "内容がないよう\nここに改行区切りで単語を入れよう。\n";
       $out['regexp'] = '';
       $out['code'] = '';
       $out['bad'] = false;
       $out = hh($out);
       include("index.tpl");       
   }

   //正規表現生成
   $assemble = new Regexp_Assemble();
   foreach($list as $_) {
       $assemble->add( _perl_quotemeta($_) );
   }
   $out['regexp'] = $assemble->as_string();

   //念のためマッチテストする
   $bad = false;
   foreach($list as $_) {
       if (! preg_match('/^'.$out['regexp'].'$/u',$_ ) ) {
           $bad = true;
       }
   }

   mylog("入力単語:" . join("\t",$list) . "	出力正規表現:" . $out['regexp']. "	判定:{$bad}\r\n" );


   $out['code'] = 'require_once(\'Regexp_Assemble.php\');  //正規表現自動生成モジュール' . "\n";
   $out['code'].= "\n";
   $out['code'].= "\n";
   $out['code'].= '$assemble = new Regexp_Assemble();      //オブジェクトを作って・・・' . "\n";
   $out['code'].= "\n";
   $out['code'].= '//単語を add で追加するだけ。 '."\n";
   foreach($list as $_) {
       $out['code'].= '$assemble->add( \'' . _singleqoute(_perl_quotemeta($_)). '\' );'."\n";
   }
   $out['code'].= "\n";
   $out['code'].= '$str = $assemble->as_string(); //これで正規表現を生成します。'."\n";
   $out['code'].= "\n";
   $out['code'].= 'echo $str;'."\n";

   $out['wordlist'] = $_REQUEST['wordlist'];

   $out['bad'].= $bad;

   if($_REQUEST['method'] == "GET-JSON") {
        header("Content-Type: text/javascript; charset=utf-8");
        echo json_encode($out);
        die;
   }


   $out = hh($out);
   include("index.tpl");
   die;
}

if ( isset($_GET['download']) ) {
   if ($_GET['download'] == 'Regexp_Assemble.php') {
      header("Content-Disposition: inline; filename=Regexp_Assemble.php");
      header("Content-Type: text/plan");
      echo file_get_contents("Regexp_Assemble.php");
   }
   else {
      echo "いやーん";
   }
  die;
}

//ディフォルト.
$out['wordlist'] = "who\nwhat\nwhere\nwhy\nwhen\nここに改行区切りで単語を入れよう。\n";
$out['regexp'] = '';
$out['code'] = '';
$out['bad'] = false;
$out = hh($out);
include("index.tpl");