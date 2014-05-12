<?php
    function check_type()
    {
        if(isset($_GET['type']))
        {
            if($_GET['type'] == 'su')
                $server_name = "SU";
            else if($_GET['type'] == 'mu')
                $server_name = "MU";
            else
            {
                die("Undefined Bucket Operation Type : " . $_GET['type'] . " Please Connect Administrator!");
            }
        }
        else
        {
            die("Undefined Bucket Operation Type ! Please Connect Administrator !");
        }
        return $server_name;
    }
?>
