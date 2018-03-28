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

$sensor = $_GET["sensor"];
$random = $_GET["random"];

$query = "INSERT INTO iot (table1, table2) VALUES ('주석으로', '남겨둔다')";	//db 삽입문
$result = mysqli_query($connect, $query) or die ('Error inserting DB');		//연결된 DB에 쿼리 실행


/*
if ($result) {
	echo $connect->affected_rows." data inserted into databases.";
	mysqli_free_result($result);
}
else{
	echo "ERROR: data can't inserted into databases";
}
*/
//mysqli_free_result($result);	//메모리 삭제
mysqli_close($connect);			//DB 연결 종료

?>