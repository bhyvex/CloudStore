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
<!-- <link rel="stylesheet" type="text/css" href="styles.css" /> -->
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
    font: italic 28px "Trebuchet MS", Verdana, Arial, Helvetica, sans-serif;
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

<title>MU集群控制</title>
</head>

<body>
<div id="box">

<table id='mytable' cellspacing='0'>
<caption>Storage Server Cluster Migration And Extension</caption>
<tr>
<th width="20%" scope='col'><a href="../mu_about.php" target="_blank">关于MU集群</a></th>
<th width="20%" scope="col" abbr="Dual 1.8"><a href="./about_cstore.php" target="_blank">关于CSTORE系统</a></th>
<th width="40%" scope="col" abbr="Dual 2"><a href="../control/extension.php?action=su" style="color: #8B008B;font-size: 16px" target='_blank'>MU集群全局桶扩展</a></th>
<th width="20%" scope="col" abbr="Dual 2.5"><a href="../operation_abouit.php" target="_blank">关于桶操作</a></th>
</tr>
</table>

<table id="mytable" cellspacing="0">
<tr>
<th width="15%">MU节点编号</th>
<th width="20%">MU节点IP地址</th>
<th width="20%">MU节点磁盘负载</th>
<th width="15%">桶号</th>
<th width="15%">目的节点</th>
<th width="15%">MU桶迁移</th>

</table>
</div>

<?php
    $head = new CHeader();
    $head->set_cmd(MSG_MPC_CS_MU_BUCKET);
    $head->set_length(0);

    $head_str = $head->package();
    $all_mu_bucket = send_data_to_cs($head_str);

    $head = $all_mu_bucket->msg_header;
    if($head->cmd() != MSG_MPC_CS_MU_BUCKET_ACK)
    {
        die("Undefine CS reply ack : " . $head->cmd());
    }
    else if($head->error() != CS_OK)
    {
        echo "<script type='text/javascript'>";
        echo "window.alert('获取MU全部通信息出错，请查看CS运行状态!');";
        echo "</script>";

        die("Get All MU Bucket error , Please Check CS Status...");
    }

    $load_string = $all_mu_bucket->load_data;
    $mu_bucket_pb = new pb_MSG_MPC_CS_MU_BUCKET_ACK();
    $mu_bucket_pb->ParseFromString($load_string);

    $size = $mu_bucket_pb->bucket_list_size();

    //接下来开始布置表格，显示在网页中 
    
    echo "<table id='mytable' cellspacing='0'>";
    echo "<caption> </caption>";

    for($pos = 0 ; $pos < $size ; $pos ++)
    {
        $item = $mu_bucket_pb->bucket_list($pos);
        $disk_load = 0;
        $bucket_num = $item->bucket_item_size();
        for($ano = 0 ; $ano < $bucket_num ; $ano ++)
        {
            $disk_load += $item->bucket_item($ano)->bucket_load();
        }
        echo "<tr>";
        echo "<td width='15%'>" . $pos . "</td>";
        $string_ip = change_string_ip(long2ip($item->mu_ip()));
        echo "<td width='20%' name='ip_td'>" . $string_ip . "</td>";
        echo "<td width='20%'>" . number_format($disk_load / (1024 * 1024) , 3) . " MB</td>";
        echo "<td width='15%'";
        echo "<select id='slt' disable='disable' alt='Select Migration Bucket' title='Select Migration Bucket' > ";
        echo "<select onchange='selectBucket(this)'>";

        for($ano = 0 ; $ano < $bucket_num ; $ano ++)
        {
            $bucket_n = $item->bucket_item($ano)->bucket_num();
            $load = $item->bucket_item($ano)->bucket_load();
            if(isset($_GET['line']) && isset($_GET['bucket']) && $_GET['line'] == $pos && $_GET['bucket'] == $bucket_n)
            {
            echo "<option value='" . $bucket_n . "' selected='selected'>" . $bucket_n . " (" . number_format($load / (1024 * 1024) , 3) . "MB )</option>";
            }
            else
            {
                echo "<option value='" . $bucket_n . "'>" . $bucket_n . " (" . number_format($load / (1024 * 1024) , 3) . "MB )</option>";
            }
            
        }
        echo "</select>";
        echo "</td>";

        echo "<td width='15%'>";
        echo "<select disable='disable' id='ip_slt' alt='Select Destination Node' title='Migration To...'>";

        if(isset($_GET['line']) && isset($_GET['bucket']) && $_GET['line'] == $pos)
        {
            echo "ano count : " . $ano;
            $selectbucket = $_GET['bucket'];
            for($ano = 0 ; $ano < $size ; $ano ++)
            {
                $ano_item = $mu_bucket_pb->bucket_list($ano);
                $item_bucket_size = $ano_item->bucket_item_size();
                for($bno = 0 ; $bno < $item_bucket_size ; $bno ++)
                {
                    $item_bucket_nr = $ano_item->bucket_item($bno)->bucket_num();
                    if($item_bucket_nr == $selectbucket)
                        break;
                }
                if($bno == $item_bucket_size)
                {
                    $dest_ip = change_string_ip(long2ip($ano_item->mu_ip()));
                    echo "<option value='" . $dest_ip . "'>" . $dest_ip . "</option>";
                }
            }
        }
        else
        {
            for($ano = 0 ; $ano < $size ; $ano ++)
            {
                $ano_item = $mu_bucket_pb->bucket_list($ano);
                if($ano_item->mu_ip() != $item->mu_ip())
                {
                    $dest_ip = change_string_ip(long2ip($ano_item->mu_ip()));
                    echo "<option value='" . $dest_ip . "'>" . $dest_ip . "</option>";
                }
            }
        }

        echo "</select>";
        echo "</td>";
        
        echo "<td width='15%'>";
//        echo "<td width='15%'><a href='../control/migrate_bucket?action=" . $item->mu_ip() . "'>Migrate Bucket</a></td>";
        echo "<button disable='disable' onclick='MigrationCheck(this)'>Migrate Bucket</button>";
        echo "</td>";
        echo "</tr>";
    }

    echo "</table>";

?>

<?php
$username = $_SESSION['UserName'];
$info = "这个是对MU集群的控制页面，实现桶的迁移和扩展，暂时还不知道怎么实现，先空着吧... " . $username;
echo "<p><h2>" . $info . "</h2></p>";
?>

<script type="text/javascript">

function GetRowObj(ele){
    while (ele.tagName.toLowerCase() != "tr"){
        ele = ele.parentNode;
        if(ele.tagName.toLowerCase() == "table")
            return null;
    }

    return ele;
}

function MigrationCheck(obj)
{
    var trobj = GetRowObj(obj);

    var objbucket = trobj.getElementsByTagName("select")[0];
    var objip = trobj.getElementsByTagName("select")[1];

    var srcip = trobj.children[1].innerHTML;
    
    var outinfo = "源MU: " + srcip + " 目的MU: " + objip.value + " 桶号: " + objbucket.value + "\n确定执行迁移操作?";

    if(confirm(outinfo))
    {
        location.href="../control/migration.php?" + "src=" + srcip + "&dest=" + objip.value + "&bucket=" + objbucket.value;
    }
}

function selectBucket(obj)
{
    var trobj = GetRowObj(obj);
    var bucket = obj.value;
    var no = trobj.children[0].innerHTML;

    window.alert('select bucket ' + bucket + ' on line ' + no);
    location.href="su_operation.php?" + "line=" + no + "&bucket=" + bucket;
}

</script>
</body>
</html>

