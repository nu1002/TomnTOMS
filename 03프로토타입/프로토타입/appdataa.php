<?php
header("Content-Type: text/html; charset=UTF-8"); //charset 설정
$mysql_hostname = "waytech.kr";
$mysql_username = "waytechtest";
$mysql_password = "dnpdl123";
$mySql_database = "waytechtest";

$connect = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);  //DB연결

//if else문 대신 or die문 추가해도 됨 or die('Error connectiong to MySQL server')
//db 연결 확인

if(mysqli_connect_errno($connect)){
  echo "DB fail";
}
else{
  echo "DB success";
}
<br>

$cm1 = $_GET["cm1"];
$cm2 = $_GET["cm2"];


$query = "SELECT * FROM iotTest"; //db select문
$result = mysqli_query($query, $connect) or die ('Error inserting DB'); 

$total_record=mysql_num_rows($result);
echo "{\"status\":\"OK\",\"num_results\":\"$total_record\",\"results\":[";
if($i<$total_record-1) {
  echo ",";
}
while($row = mysqli_fetch_array($result)){
  echo "cm1 :".$row[cm1].", table2: ".$row[cm2];
  echo "<br>";

}

mysqli_free_result($result);
mysqli_close($connect);     //DB 연결 종료

?>