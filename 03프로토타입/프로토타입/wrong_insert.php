<?php

header("Content-Type: text/html; charset=UTF-8");
$mysql_hostname = "waytech.kr";
$mysql_username = "waytechtest";
$mysql_password = "dnpdl123";
$mySql_database = "waytechtest";

$connect = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);  //DB연결

mysqli_set_charset($connect,"utf8");

if (mysqli_connect_errno($connect)){
   echo "Failed to connect to MySQL: " . mysqli_connect_error();
}


$w_UID = $_POST["w_UID"];
$w_title = $_POST["w_title"];
$w_content = $_POST["w_content"];
$w_userNick = $_POST["w_userNick"];
$w_toiletUID = $_POST["w_toiletUID"];
$w_view = $_POST["w_view"];


$query = "INSERT INTO wrong VALUES ('$w_UID', '$w_title', '$w_content', '$w_userNick', '$r_toiletName', CURDATE(), '$r_view')"
$result = mysqli_query($connect, $query) or die('insert failed');

mysqli_close($connect);

?>