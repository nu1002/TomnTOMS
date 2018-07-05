<?php

header("Content-Type: text/html; charset=UTF-8");
$mysql_hostname = "waytech.kr";
$mysql_username = "waytechtest";
$mysql_password = "dnpdl123";
$mySql_database = "waytechtest";

$con = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mySql_database);  //DB연결

mysqli_set_charset($con,"utf8");

if (mysqli_connect_errno($con))

{
   echo "Failed to connect to MySQL: " . mysqli_connect_error();
}
$userid = $_POST['Id'];
$userpw = $_POST['Pw'];

$result = mysqli_query($con,"insert into meminfo(u_id,u_pw) values('$userid','$userpw')");

  if($result){
    echo 'success';
  }
  else{
    echo 'failure';
  }

mysqli_close($con);

?>