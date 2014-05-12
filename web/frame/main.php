<?php
session_start();

require_once("../implement/database/check_login.php");

if(empty($_POST["UserName"]))
{
    echo "<script type='text/javascript'>";
    echo "window.alert('用户名不能为空!');";
    echo "location.href='../index.php';";
    echo "</script>";
}

if(empty($_POST["UserPassword"]))
{
    echo "<script type='text/javascript'>";
    echo "window.alert('密码不能为空!');";
    echo "location.href='../index.php';";
    echo "</script>";
}


$username = $_POST["UserName"];
$password = $_POST["UserPassword"];

$ret = check_user_login($username , $password);
if(0 == $ret)
{
    $_SESSION["UserName"] = $username;
    $_SESSION["online"] = true;
    $info = $username . " 欢迎过来...";
    echo "<script type='text/javascript'>";
    echo "window.alert($info);";
    echo "</script>";
}
else if($ret < 0)
{
    echo "<script type='text/javascript'>";
    echo "window.alert('用户名或密码错误！');";
    echo "location.href='../index.php';";
    echo "</script>";
}
else
{
    echo "<script type='text/javascript'>";
    echo "window.alert('用户名不存在!');";
    echo "location.href='../index.php';";
    echo "</script>";
}


?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<link rel="stylesheet" type="text/css" herf="../style/style.css" />
<script type="text/javascript" src="../scripts/jQuery.js"></script>
<title>CStorage 用户管理平台</title>
</head>

<frameset cols="20%,80%">
<frame src="control.php" noresize="noresize">
<frame name="showFrame">
</frameset>
</html>
