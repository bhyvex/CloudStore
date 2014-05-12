<?php
    require_once('../tool/check_session.php');
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
<script type="text/javascript"></script>
<style type="text/css">
body {
    font: normal 11px auto "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
    color: #4f6b72;
    background: #E6EAE9;
}

a {
    color: #c75f3e;
}

#mytable {	
    width: 100%;
    text-align:center;
}

caption {
    padding: 0 0 5px 0;
    font: normal 28px auto "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
    text-align:center;
}

th {
    font: bold 11px "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
    color: #4f6b72;
    border-right: 1px solid #C1DAD7;
    border-bottom: 1px solid #C1DAD7;
    border-top: 1px solid #C1DAD7;
    letter-spacing: 2px;
    text-transform: uppercase;
    text-align: left;
    padding: 6px 6px 6px 12px;
    background: #CAE8EA url(images/bg_header.jpg) no-repeat;
    text-align:center;
}

th.nobg {	
    border-top: 0;
    border-left: 0;
    border-right: 1px solid #C1DAD7;
    background: none;
}

th.tit
{
    padding: 0 0 5px 0;
    font: normal 28px auto "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
    text-align:center;
}

td {
    border-right: 1px solid #C1DAD7;
    border-bottom: 1px solid #C1DAD7;
    background: #fff;
    padding: 6px 6px 6px 12px;
    color: #4f6b72;
}


</style>
</script>

<title>查看<?php echo check_type(); ?>规则</title>
</head>

<body class="mu_background">
<div class="mu_tips">
<table width=100% class="mu_mytable" style="border:none">
<tr>
<?php
    $server = check_type();
    if($server == "MU")
    {
        echo "<th class='tit'>元数据服务器集群全局桶规则表</th>";
    }
    else
    {
        echo "<th class='tit'>数据服务器集群全局桶规则表</th>";
    }
?>
<td><a href="../rule_about.php" target="_blank">关于规则</a></td>
</tr>
</table>
</div>

<?php
    $server = check_type();

    $head = new CHeader();
    if($server == "MU")
    {
        $head->set_cmd(MSG_MPC_CS_GET_MU_RULE);
        $ret_cmd = MSG_MPC_CS_GET_MU_RULE_ACK;
    }
    else
    {
        $head->set_cmd(MSG_MPC_CS_GET_SU_RULE);
        $ret_cmd = MSG_MPC_CS_GET_SU_RULE_ACK;
    }
    $head->set_length(0);

    $head_str = $head->package();
    $all_mu_rule = send_data_to_cs($head_str);

    $head = $all_mu_rule->msg_header;
    if($head->cmd() != $ret_cmd)
    {
        die("Undefined cs reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK)
    {
        echo "<script type='text/javascript'>";
        echo "window.alert('获取全部规则信息出错,请查看CS运行状态!');";
        echo "</sciript>";
        
        die("Get All " . $server . " Rule error , Please Check CS status...");
    }

    $load_string = $all_mu_rule->load_data;
    if($server == "MU")
    {
        $mu_rule_pb = new pb_MSG_MPC_CS_GET_MU_RULE_ACK();
    }
    else
    {
        $mu_rule_pb = new pb_MSG_MPC_CS_GET_SU_RULE_ACK();
    }
    $mu_rule_pb->ParseFromString($load_string);

    $size = $mu_rule_pb->map_bucket_size();

    //接下来开始布置表格，显示在网页中
    echo "<table width=100% class='mu_mytable'>";
    echo "<tr>";
    echo "<th>桶号</th>";

    $addr = "IP地址";
    for($pos = 1 ; $pos <= MAX_IP_NUM ; $pos ++)
    {
        $addr_str = $addr . $pos;
        echo "<th>" . $addr_str . "</th>";
    }
    echo "</tr>";

    for($pos = 0 ; $pos < $size ; $pos ++)
    {
        $item = $mu_rule_pb->map_bucket($pos);

        $bucket_nr = $item->bucket_num();
        $ip_number = $item->module_ip_size();
        echo "<tr class='mu_alt'>";
        echo "<td>" . $bucket_nr . "</td>";
        for($ano = 0 ; $ano < MAX_IP_NUM ; $ano ++)
        {
            if($ano < $ip_number)
            {
                $ip_num = $item->module_ip($ano);
                $ip_str = change_string_ip(long2ip($ip_num));
                echo "<td>" . $ip_str . "</td>";
            }
            else 
            {
                echo "<td>0</td>";
            }
        }
        echo "</tr>";
    }

    echo "</table>";
?>

<?php
    /*
$username = $_SESSION['UserName'];
$info = "这个是查看系统" . $server . "集群规则的界面，暂时还不知道怎么实现，先空着吧... " . $username;
echo "<p><h2>" . $info . "</h2></p>";
     */
?>
</body>
</html>

