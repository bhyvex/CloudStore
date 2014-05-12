<?php
    require_once '../tool/check_session.php';
    require_once('../protocol/MessageHeader.class.php');
    require_once('../protocol/msg_define.macro.php');
    require_once('../protocol/pb_proto_cstore.protocol.php');
    require_once('../comm/connect_server.php');
    require_once('../tool/change_ip.php');
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="../../style/style2.css" />

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
    font: bold 28px "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
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

td {
    border-right: 1px solid #C1DAD7;
    border-bottom: 1px solid #C1DAD7;
    background: #fff;
    padding: 6px 6px 6px 12px;
    color: #4f6b72;
}
</style>

<title>SU集群垃圾回收</title>
</head>

<body>
<div>

<table id='alltable' cellspacing='0' width=100%>
<tr>
<caption>数据服务器集群垃圾数据回收</caption>
<a href='./about_recycle.php' target='_blank'>关于垃圾回收</a>
</tr>

<tr>
<td width=50%>
<table id='suload' cellspacing='0' width=100%>

<tr>
<caption>SU集群磁盘负载状况</caption>
</tr>

<tr>
<th>序号</th>
<th>IP地址</th>
<th>桶数目</th>
<th>块总数</th>
<th>存储上限(MB)</th>
</tr>

<?php

    $head = new CHeader();
    $head->set_cmd(MSG_MPC_CS_GLOBAL_BLOCK_INFO);
    $head->set_length(0);
    $head_str = $head->package();

    $all_bucket_ack = send_data_to_cs($head_str);

    $head = $all_bucket_ack->msg_header;
    if($head->cmd() != MSG_MPC_CS_GLOBAL_BLOCK_INFO_ACK)
    {
        die("Undefined CS Reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK)
    {
        echo "<script type=text/javascript>";
        echo "window.alert('获取系统负载信息出错，请查看CS运行状态!')";
        echo "</script>";

        die("Get Global Load Infomation error ! Check CS Status...");
    }
    
    $load_info = $all_bucket_ack->load_data;
    
    $bk_info = new pb_MSG_MPC_CS_GET_BLOCK_NUM_ACK();
    $bk_info->ParseFromString($load_info);
    $su_info_size = $bk_info->su_node_list_size();
    $mu_info_size = $bk_info->mu_node_list_size();
    $gap = $su_info_size - $mu_info_size;

    for($i = 0 ; $i < $su_info_size ; $i ++)
    {
        $item = $bk_info->su_node_list($i);
        echo "<tr>";
        echo "<td>" . $i . "</td>";
        echo "<td>" . change_string_ip(long2ip($item->node_ip())) . "</td>";
        echo "<td>" . $item->bucket_number() . "</td>";
        echo "<td>" . $item->block_number() . "</td>";
        echo "<td>" . $item->storage_limit() . "</td>";
        echo "</tr>";
    }

    if($gap < 0)
    {
        for($i = 0 ; $i < $gap ; $i ++)
        {
            echo "<tr>";
            echo "<td>-1</td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "</tr>";
        }
    }

    echo "
    </table>
    </td>";

    echo "
    <td width=50%>
    <table id='muload' cellspacing='0'width=100%>

    <tr>
    <caption>MU集群磁盘负载状况</caption>
    </tr>

    <tr>
    <th width=20%>序号</th>
    <th width=20%>IP地址</th>
    <th width=20%>桶数目</th>
    <th width=20%>块总数</th>
    <th width=20%>存储上限(MB)</th>
    </tr>";

    for($i = 0 ; $i < $mu_info_size ; $i ++)
    {
        $item = $bk_info->mu_node_list($i);
        $limit = $item->storage_limit();
        echo "<tr>";
        echo "<td width=20%>" . $i . "</td>";
        echo "<td width=20%>" . change_string_ip(long2ip($item->node_ip())) . "</td>";
        echo "<td width=20%>" . $item->bucket_number() . "</td>";
        echo "<td width=20%>" . $item->block_number() . "</td>";
        if($limit < 0)
        echo "<td width=20%>" . $limit . "</td>";
        echo "</tr>";
    }
    if($gap > 0)
    {
        for($i = 0 ; $i < $gap ; $i ++)
        {
            echo "<tr>";
            echo "<td>-1</td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "<td> </td>";
            echo "</tr>";
        }
    }

    echo "
    </table>
    </td>";

?>

</tr>
</table>
</div>
<?php
    /*
$username = $_SESSION['UserName'];
$info = "这个是对SU集群进行全局回收的控制界面，暂时还不知道怎么实现，先空着吧... " . $username;
echo "<p><h2>" . $info . "</h2></p>";
     */
?>
</body>
</html>

