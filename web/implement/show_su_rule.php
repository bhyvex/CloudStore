
<?php
    require_once('../tool/check_session.php');
    require_once('../protocol/MessageHeader.class.php');
    require_once('../protocol/msg_define.macro.php');
    require_once('../protocol/pb_proto_cstore.protocol.php');
    require_once('../comm/connect_server.php');
    require_once('../tool/change_ip.php');
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="../../style/rule_style.css" />
<script type="text/javascript"></script>

<style>
.su_background{
    background-color: #FFE4B5;
    font-size: 16px;
    font-weight: bold;
}

.su_tips{
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    font-size: 1em;
    font-weight: bold;
    color: #FF3030;
    background-color: #FFFACD;
}

.su_mytable{
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    width: 100%;
    border-collapse: collapse;
}

.su_mytable td, .su_mytable th{
    font-size: 1em;
    text-align:center;
    border: 1px solid #98bf21;
    padding: 3px 7px 2px 7px;
}

.su_mytable th{
    font-size:1.1em;
    text-align:center;
    padding-top:5px;
    padding-bottom:4px;
    background-color:#A7C942;
    color:#FFFFFF;
}

.su_mytable tr.su_alt td{
    color: #000000;
    background-color: #EAF2D3;
}
</style>

<title>查看SU规则</title>
</head>

<body class="su_background">
<div class="su_tips">
<table class="su_mytable" style="border:none">
<tr>
<th>Rule Of CStorage System Storage Server Cluster</th>
<td><a href="../rule_about.php" target="_blank">about rule</a></td>
</tr>
</table>
</div>

<?php
    $head = new CHeader();
    $head->set_cmd(MSG_MPC_CS_GET_SU_RULE);
    $head->set_length(0);

    $head_str = $head->package();
    $all_su_rule = send_data_to_cs($head_str);

    $head = $all_su_rule->msg_header;
    if($head->cmd() != MSG_MPC_CS_GET_SU_RULE_ACK)
    {
        die("Undefined cs reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK)
    {
        echo "<script type='text/javascript'>";
        echo "window.alert('获取SU全部规则信息出错,请查看CS运行状态!');";
        echo "</sciript>";
        
        die("Get All SU Rule error , Please Check CS status...");
    }

    $load_string = $all_su_rule->load_data;
    $su_rule_pb = new pb_MSG_MPC_CS_GET_SU_RULE_ACK();
    $su_rule_pb->ParseFromString($load_string);

    $size = $su_rule_pb->map_bucket_size();

    //接下来开始布置表格，显示在网页中
    echo "<table class='su_mytable'>";
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
        $item = $su_rule_pb->map_bucket($pos);

        $bucket_nr = $item->bucket_num();
        $ip_number = $item->module_ip_size();
        echo "<tr class='su_alt'>";
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
$username = $_SESSION['UserName'];
$info = "这个是查看系统SU集群规则的界面，暂时还不知道怎么实现，先空着吧... " . $username;
echo "<p><h2>" . $info . "</h2></p>";
?>

</body>
</html>
