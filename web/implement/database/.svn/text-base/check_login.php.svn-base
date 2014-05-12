<?php
function check_user_login($user , $pwd)
{
    $user_ini = parse_ini_file('user_config.ini' , true);
    $username1 = $user_ini['admin']['username'];
    $password1 = $user_ini['admin']['password'];

    $username2 = $user_ini['terry']['username'];
    $password2 = $user_ini['terry']['password'];

    /*
    echo $username1 . "<br>";
    echo $username2 . "<br>";
     */

    //用户名不存在
    if((strcmp($user , $username1) != 0) &&
        (strcmp($user , $username2) != 0))
    {
        return 1;
    }
    else
    {
        if((strcmp($user , $username1) == 0) && 
            strcmp($pwd , $password1) == 0)
        {
            return 0;
        }
        if((strcmp($user , $username2) == 0) && 
            strcmp($pwd , $password2) == 0)
        {
            return 0;
        }
    }

    return -1;
}
?>
