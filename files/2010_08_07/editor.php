<?
	$ret = array();
	$ret[] = array('id' => 1234 , 'name' => 'テスト' , 'message1' => 'message1'  , 'message2' => 'message2');
	$ret[] = array('id' => 2345 , 'name' => 'ととろ' , 'message1' => '!!message1'  , 'message2' => '!!message2');
	$ret[] = array('id' => 3456 , 'name' => 'ばなな' , 'message1' => '!!message1'  , 'message2' => '!!message2');
	$ret[] = array('id' => 3456 , 'name' => 'みるく' , 'message1' => '!!message1'  , 'message2' => '!!message2');

	header('Content-Type: application/json; charset=UTF-8');
	echo json_encode($ret);

?>