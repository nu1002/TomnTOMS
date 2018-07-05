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

$review_title = $_POST["review_title"];
$review_content = $_POST["review_content"];


$result = mysqli_query($con,"insert into review(review_title,review_content) values('$review_title','$review_content')");

  if($result){
    echo 'success';
  }
  else{
    echo 'failure';
  }

mysqli_close($con);

?>