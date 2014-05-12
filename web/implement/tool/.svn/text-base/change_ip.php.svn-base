<?php

function change_string_ip($ip_str)
{
    $str_array = explode('.' , $ip_str);

    $new_ip_str = "";
    $size = count($str_array);

    for($pos = $size - 1 ; $pos >= 0 ; $pos --)
    {
        if(empty($new_ip_str))
            $new_ip_str = $str_array[$pos];
        else
            $new_ip_str = $new_ip_str . "." . $str_array[$pos];
    }

    return $new_ip_str;
}

function change_int_ip($ip_str)
{
    $new_ip = change_string_ip($ip_str);
    $ip_int = ip2long($new_ip);

    return $ip_int;
}
?>
