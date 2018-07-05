<?php
header("Content-Type: text/html; charset=UTF-8");
$mysql_hostname = "waytech.kr";
$mysql_username = "waytechtest";
$mysql_password = "dnpdl123";
$mySql_database = "waytechtest";

$connect = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);  //DB연결

mysqli_set_charset($con,"utf8");

if (mysqli_connect_errno($con))

{
   echo "Failed to connect to MySQL: " . mysqli_connect_error();
}


$userid = $_POST['Id'];
$userpw = $_POST['Pw'];
$query_search = "select * from meminfo where u_id = '".$userid."' AND u_pw = '".$userpw. "'";


if($rows == 0) {
  echo "No Such User Found";
}else  {    }

?>