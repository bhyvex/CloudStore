<?php
    require_once '../tool/check_session.php';
    require_once('../protocol/MessageHeader.class.php');
    require_once('../protocol/msg_define.macro.php');
    require_once('../protocol/pb_proto_cstore.protocol.php');
    require_once('../comm/connect_server.php');
    require_once('../tool/change_ip.php');
    require_once('../tool/check_server.php');
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

<title>
<?php
    echo check_type();
?>桶扩展</title>
</head>
</html>

<?php
    $server = check_type();
    
    $head = new CHeader();
    if($server == "MU")
    {
        $head->set_cmd(MSG_MPC_CS_MU_EXTENT_BUCKET);
        $extent_pb = new pb_MSG_MPC_CS_MU_EXTENT_BUCKET;
        $ret_cmd = MSG_MPC_CS_MU_EXTENT_BUCKET_ACK;
    }
    else
    {
        $head->set_cmd(MSG_MPC_CS_SU_EXTENT_BUCKET);
        $extent_pb = new pb_MSG_MPC_CS_SU_EXTENT_BUCKET;
        $ret_cmd = MSG_MPC_CS_SU_EXTENT_BUCKET_ACK;
    }

    $extent_pb->set_new_mod(0);
    $pb_data = $extent_pb->SerializeToString();
    $head->set_length(strlen($pb_data));
    $head_str = $head->package();

    $all_data = $head_str . $pb_data;
    $extent_ack = send_data_to_cs($all_data);

    $head = $extent_ack->msg_header;
    if($head->cmd() != $ret_cmd)
    {
        die("Undefined CS reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK)
    {
        echo "<script type='text/javascript'>";
        echo "window.alert('执行桶扩展操作出错,请查看CS获取失败原因!');";
        echo "</script>";
        die("Do Extension " . $server_type . " Bucket error , Please Check CS For Detail...
        " . $head->error());
    }
    else
    {
        $info = "执行 " . $server . " 桶扩展操作成功!,请查看" . $server . "规则表";
        $dest = "../show/show_rule.php?type=" . strtolower($server);
        echo "<script type='text/javascript'>";
        echo "window.alert('" . $info . "');";
        echo "location.href='" . $dest . "';";
        echo "</script>";
    }
?>
