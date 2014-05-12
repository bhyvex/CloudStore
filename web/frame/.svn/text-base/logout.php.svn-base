<?php
header("Content-Type: text/html; charset=UTF-8");
session_start();

$username = $_SESSION['UserName'];
$info = $username . " 退出成功!";

$_SESSION = array();
if(isset($_COKKIE[session_name()]))
{
    setCookie(session_name() , '' , time() - 60 * 60 , '/');
}

session_destroy();

echo "<script type='text/javascript'>window.alert('$info')</script>";
echo "<script type='text/javascript'>window.top.location.href='../index.php'</script>";

?>
