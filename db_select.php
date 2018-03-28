<?php
header("Content-Type: text/html; charset=UTF-8");	//charset 설정

$mysql_hostname = "localhost";
$mysql_username = "root";
$mysql_password = "passwd";
$mySql_database = "iot";

$connect = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);	//DB연결

//if else문 대신 or die문 추가해도 됨 or die('Error connectiong to MySQL server')
//db 연결 확인
if(mysqli_connect_errno($connect)){
	echo "DB fail";
}
else{
	echo "DB success";
}
echo'<br>';

$query = "SELECT * FROM iot";	//db select문
$result = mysqli_query($connect, $query) or die ('Error querying database');	

//db 출력
while($row = mysqli_fetch_array($result)){
  echo "talbe1: ".$row[table1].", table2: ".$row[table2];
  echo "<br>";

}

mysqli_free_result($result);
mysqli_close($connect);
?>