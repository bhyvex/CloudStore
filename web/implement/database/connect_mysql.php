<?
    error_reporting(E_ALL);

class Migration_Info
{
    var $src_ip;
    var $dest_ip;
    var $bucket_nr;
    var $state;
    var $start_time;
    var $end_time;
    var $result;
    var $reason;
}

function get_all_table_info($ip , $port , $user , $pwd , $db , $table , $type)
{
    $mysql_handler = new mysqli($ip , $user , $pwd , $db , $port);
    if(mysqli_connect_errno())
    {
        echo "连接数据库IP : " . $ip . " 出错: " . mysqli_connect_error();
        return null;
    }

    $mysql_handler->select_db($table);
    if(mysqli_connect_errno())
    {
        echo "选择数据库表 : " . $table . " 出错: " . mysqli_connect_error();
        return null;
    }

    $query_info = "select * from " . $table . " where type='" . $type . "';";
    $result = $mysql_handler->query($query_info) ;
    if(mysqli_connect_errno())
    {
        echo "获取数据库全部的信息,类型为: " . $type . "出错 : " . mysqli_connect_error();
        return null;
    }
    
    $info_array = array();
    $i = 0;
    while($row = $result->fetch_assoc())
    {
        $mig_info = new Migration_Info;
        $mig_info->src_ip = $row["srcIP"];
        $mig_info->dest_ip = $row["destIP"];
        $mig_info->bucket_nr = $row["bucket"];
        $mig_info->state = $row["state"];
        $mig_info->start_time = $row["start"];
        $mig_info->end_time = $row["end"];
        $mig_info->result = $row["result"];
        $mig_info->reason = $row["reason"];
        $info_array[$i] = $mig_info;
        $i ++;
    }

    $result->close();
    $mysql_handler->close();

    return $info_array;
}

function get_all_migration_history($type)
{
    $config = parse_ini_file("database_config.ini" , true);
    if(! $config)
    {
        echo "Parse ini file database_config.ini error !";
        return null;
    }
    
    $ip = $config["cs_db"]["db_ip"];
    $port = $config["cs_db"]["db_port"];
    $db_name = $config["cs_db"]["db_name"];
    $user = $config["cs_db"]["username"];
    $pwd = $config["cs_db"]["passwd"];
    $table = $config["cs_db"]["migration_table"];

    return get_all_table_info($ip , $port , $user , $pwd , $db_name , $table , $type);

}

function get_all_su_migration_history()
{
    return get_all_migration_history("SU");
}

function get_all_mu_migration_history()
{
    return get_all_migration_history("MU");
}

?>
