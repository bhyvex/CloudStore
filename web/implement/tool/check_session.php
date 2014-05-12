<?php

@session_start();

if(!$_SESSION['online'])
{
    echo "<script type='text/javascript'>";
    echo "window.alert('请重新登录!');";
    echo "location.href='../../index.php';";
    echo "</script>";
}

?>
