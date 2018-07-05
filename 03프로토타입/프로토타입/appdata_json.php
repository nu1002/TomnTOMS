<?php
header("Content-Type: text/html; charset=UTF-8"); //charset 설정
$mysql_hostname = "waytech.kr";
$mysql_username = "waytechtest";
$mysql_password = "dnpdl123";
$mySql_database = "waytechtest";

$con = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);  //DB연결

if(mysqli_connect_errno($con)){
  echo "DB fail";
}

mysqli_set_charset($con, "utf8");
$res = mysqli.query($con, "select * from iotTest");
$result = array();


while($row = mysqli_fetch_array($res)){
	array_push($res, array('cm1'=>$row[0], 'cm2'=>$row[1], 'seat_state1'=>$row[2]));
}

echo json_encode(array("result"=>$result));
?>