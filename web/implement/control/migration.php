<?php
    require_once("../tool/check_session.php");
    require_once("../tool/change_ip.php");
    require_once("../tool/check_server.php");
    require_once('../protocol/MessageHeader.class.php');
    require_once('../protocol/msg_define.macro.php');
    require_once('../protocol/pb_proto_cstore.protocol.php');
    require_once('../comm/connect_server.php');
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

<title>
<?php
    echo check_type();
?>桶迁移</title>
</head>
</html>

<?php
    $server_type = check_type();
  
    $src_ip = $_GET["src"];
    $dest_ip = $_GET["dest"];
    $bucket_nr = $_GET["bucket"];
     
    $head = new CHeader();
    if($server_type == "MU")
    {
        $head->set_cmd(MSG_MPC_CS_MU_MIGRATE_BUCKET);
        $mu_migration_pb = new pb_MSG_MPC_CS_MU_MIGRATE_BUCKET;
        $ret_cmd = MSG_MPC_CS_MU_MIGRATE_BUCKET_ACK;
    }
    else
    {
        $head->set_cmd(MSG_MPC_CS_SU_MIGRATE_BUCKET);
        $mu_migration_pb = new pb_MSG_MPC_CS_SU_MIGRATE_BUCKET;
        $ret_cmd = MSG_MPC_CS_SU_MIGRATE_BUCKET_ACK;
    }

    $mu_migration_pb->set_source_ip(change_int_ip($src_ip));
    $mu_migration_pb->set_destination_ip(change_int_ip($dest_ip));
    $mu_migration_pb->set_bucket_num($bucket_nr);

    $pb_data = $mu_migration_pb->SerializeToString();
    
    $head->set_length(strlen($pb_data));
    $head_str = $head->package();

    $all_data = $head_str . $pb_data;
    $migration_ack = send_data_to_cs($all_data);

    $head = $migration_ack->msg_header;
    if($head->cmd() != $ret_cmd)
    {
        die("Undefine CS reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK) 
    {
        echo "<script type='text/javascript'>";
        echo "window.alert('执行桶迁移操作出错,请查看CS获取失败原因!');";
        echo "</script>";

        die("Do Migration " . $server_type . " Bucket error , Please Check CS For Detail..." . $head->error());
    }
    else
    {
        $info = "桶迁移操作执行成功!  详细信息:" . "From " . $src_ip . " To " . $dest_ip . " Bucket Number " . $bucket_nr;
        $dest = "../show/show_rule.php?type=" . strtolower($server_type);
        echo "<script type='text/javascript'>";
        echo "window.alert('" . $info . "');";
        echo "location.href='" . $dest . "';";
        echo "</script>";
    }
?>

