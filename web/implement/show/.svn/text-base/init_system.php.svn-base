<?php
    require_once '../tool/check_session.php';
    require_once('../protocol/MessageHeader.class.php');
    require_once('../protocol/msg_define.macro.php');
    require_once('../protocol/pb_proto_cstore.protocol.php');
    require_once('../comm/connect_server.php');
    require_once('../tool/change_ip.php');

    class IP_NUM
    {
        var $ip;
        var $num;
    }
    
    function check_ack($head , $ret_cmd)
    {
        if(($head->cmd() != $ret_cmd) || ($head->error() != CS_OK))
        {
            echo "<script type='text/javascript'>";
            echo "window.alert('查看系统初始化信息出错，请查看CS运行状态!');";
            echo "window.location.href='./init_system.php';";
            echo "</script>";
        }
        else if($head->para1() == HASH_HAS_INITED)
        {
            echo "<script type='text/javascript'>";
            echo "window.alert('系统已经初始化!');";
            echo "window.location.href='./init_system.php';";
            echo "</script>";
        }
        else if(($head->para1() != HASH_NOT_INITED))
        {
            echo "<script type='text/javascript'>";
            echo "window.alert('未知错误，请查看CS运行状态!');";
            echo "window.location.href='./init_system.php';";
            echo "</script>";
        }
        
        return 0;
    }
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

<title>初始化部署系统</title>
</head>

<body>
<div>

<table width=100%>
<caption>系统初始化部署</caption>
<tr>
<th>节点数目:&nbsp;
<input type="text" id="number" class="input" />
</th>

<th>&nbsp;&nbsp;模值:&nbsp;
<input type="text" id="mod" class="input" />
</th>

<th>&nbsp;&nbsp;&nbsp;节点类型:&nbsp;
<select id="choise">
<option value="mu">MU</option>
<option value="su">SU</option>
</select>
</th>

<th>&nbsp;&nbsp;副本数:&nbsp;
<select id="dup">
<option value="1">1</option>
<option value="2">2</option>
<option value="3">3</option>
<option value="4">4</option>
<option value="5">5</option>
</select>
</th>

<th>
&nbsp;
<button onclick=checkInput(this) alt='Init System' title="Init System">确定</button>
</th>
</tr>
</table>

<?php
    if(isset($_GET["number"]) && isset($_GET["type"]) && !isset($_GET["sum"]))
    {
        $number = $_GET["number"];
        $server = $_GET["type"];
            
        $head = new CHeader();
        if($server == "mu")
        {
            $head->set_cmd(MSG_MPC_CS_MU_INIT_HASH);
            $ret_cmd = MSG_MPC_CS_MU_INIT_HASH_ACK;
            $init_pb = new pb_MSG_MPC_CS_MU_INIT_HASH;
            $init_pb->set_mod(10);
            $init_pb->set_duplicate_num(3);
        }
        else if($server == "su")
        {
            $head->set_cmd(MSG_MPC_CS_SU_INIT_HASH);
            $ret_cmd = MSG_MPC_CS_SU_INIT_HASH_ACK;
            $init_pb = new pb_MSG_MPC_CS_SU_INIT_HASH;
            $init_pb->set_mod(10);
            $init_pb->set_duplicate_num(4);
        }
        else
        {
            die("Undefined Initlized Type,Please Check Input!");
        }
        
        $pb_str = $init_pb->SerializeToString();
        $head->set_length(strlen($pb_str));
        $head_str = $head->package();
        $all_data = $head_str . $pb_str;

        $init_ack = send_data_to_cs($all_data);

        $head = $init_ack->msg_header;

        check_ack($head , $ret_cmd);

        echo "<table 'id=initTable'>";
        $info = "初始化部署 " . strtoupper($server) . " 集群";
        echo "<caption>" . $info . "</caption>";
        echo "<tr>";
        echo "<th>起始IP</th>";
        echo "<th>连续节点数目</th>";
        echo "</tr>";
        
        for($pos = 0 ; $pos < $number ; $pos ++)
        {
            echo "<tr id='intr'>";
            echo "<td>";
            echo "<input id='ip' />";
            echo "</td>";
            echo "<td>";
            echo "<input id='num' />";
            echo "</td>";
            echo "</tr>";
        }

        echo "<tr>";
        echo "<th><button onclick='checkSubmit(this)' alt='Execurate' title='Init System'>确定</button></th>";
        echo "</tr>";

        echo "</table>";
    }

    if(isset($_GET["sum"]) && (isset($_GET["mod"])) && (isset($_GET["dup"])) && isset($_GET["type"]))
    {
        $num = $_GET["sum"];
        $ip_array = array();
        $count = 0;
        for($i = 0 ; $i < $num ; $i ++)
        {
            $ipstr = "ip" . $i;
            $numstr = "n" . $i;
            if(!(isset($_GET[$ipstr]) && isset($_GET[$numstr])))
                continue ;

            $ip_num = new IP_NUM();
            $ip_num->ip = $_GET[$ipstr];
            $ip_num->num = $_GET[$numstr];
            $ip_array[$count] = $ip_num;
            $count ++;
        }
        $server = $_GET["type"] ;
        $head = new CHeader();
        
        if($server == "mu")
        {
            $head->set_cmd(MSG_MPC_CS_MU_INIT_HASH);
            $ret_cmd = MSG_MPC_CS_MU_INIT_HASH_ACK;
            $init_pb = new pb_MSG_MPC_CS_MU_INIT_HASH;
            $info = "MU";
        }
        else if($server == "su")
        {
            $head->set_cmd(MSG_MPC_CS_SU_INIT_HASH);
            $ret_cmd = MSG_MPC_CS_SU_INIT_HASH_ACK;
            $init_pb = new pb_MSG_MPC_CS_SU_INIT_HASH;
            $info = "SU";
        }
        else
        {
            die("Undefined Initlized Type,Please Check Input! type : " . $server);
        }
        $init_pb->set_mod($_GET["mod"]);
        $init_pb->set_duplicate_num($_GET["dup"]);
        for($i = 0 ; $i < $count ; $i ++)
        {
            $ip_num = $ip_array[$i];
            $ip_scope = $init_pb->add_module_ip();
            $ip_scope->set_Start_IP(ip2long(change_string_ip($ip_num->ip)));
            $ip_scope->set_Serial_num($ip_num->num);
//            echo "start ip : " . $ip_num->ip . " and int is " . ip2long(change_string_ip($ip_num->ip)) . " and count " . $ip_num->num . "<br>";
        }
        
        $proto_str = $init_pb->SerializeToString();
        $head->set_length(strlen($proto_str));
        $head_str = $head->package();

        $all_data = $head_str . $proto_str;

        $init_ack = send_data_to_cs($all_data);
        
        $head = $init_ack->msg_header;
        check_ack($head , $ret_cmd);
        
        echo "<script type='text/javascript'>";
        echo "window.alert('初始化部署成功,点击查看" . $info . "规则信息');";
        echo "window.location.href='./show_rule.php?type=" . $server . "';";
        echo "</script>";
    }
?>


</div>


<?php
    /*
$username = $_SESSION['UserName'];
$info = "这个是初始化部署系统的界面，暂时还不知道怎么实现，先空着吧... " . $username;
echo "<p><h2>" . $info . "</h2></p>";
     */
?>
</body>

<script type="text/javascript">

function checkInput(obj)
{
    var inobj = document.getElementById("number").value;
    var selobj = document.getElementById("choise").value;
    var modobj = document.getElementById("mod").value;
    var dupobj = document.getElementById("dup").value;
    if(!inobj)
        alert("请输入节点的数目!");
    else if(!modobj)
        alert("请输入初始化模值!")
    else if(!parseIsInt(inobj) || inobj == "0")
        alert("请输入正确的节点数目!");
    else if(!parseIsInt(modobj))
        alert("请输入正确的初始化模值!");
    else
    {
        var info = "初始化部署 " + selobj.toUpperCase() + " 服务器" + inobj + "个节点!" + "共" + Math.pow(2 , modobj) + "个桶,每个桶有" + dupobj + "个副本!";
        if(confirm(info))
        {
            var cur_url = window.location.href;
            location.href = cur_url + "?type=" + selobj + "&number=" + inobj + "&mod=" + modobj + "&dup=" + dupobj;
        }
    }
}
function GetTrObj(obj)
{
    while(obj.tagName.toLowerCase() != "tr")
    {
        obj = obj.parentNode;
        if(obj.tagName.toLowerCase() == "html")
            return null;
    }

    return obj;
}

function GetTableObj(obj)
{
    while(obj.tagName.toLowerCase() != "table")
    {
        obj = obj.parentNode;
        if(obj.tagName.toLowerCase() == "html")
            return null;
    }

    return obj;
}

function parseIsInt(str)
{
    for(var i = 0 ; i < str.length ; i ++)
    {
        if(! ((str.charAt(i) <= 9) && (str.charAt(i) >= 0)))
        {
            break;
        }
    }

    if(i != str.length)
        return false;
    else
        return true;
}

function IsIpValue(str)
{
    arr = str.split(".");
    if(arr.length != 4)
    {
        return false;
    }

    if(arr[0] == 0)
    {
        return false;
    }
    var err = false;
    for(var i = 0 ; i < arr.length ; i ++)
    {
        if(!parseIsInt(arr[i]))
        {
            err = true;
            break;
        }
        else if(arr[i] > 255 || arr[i] < 0)
        {
            err = true;
            break;
        }
    }

    return !err;
}


function checkSubmit(obj)
{
    var tableobj = GetTableObj(obj);
    
    var allIP = window.location.href;
    var count = 0;
    for(var i = 1 ; i < tableobj.rows.length - 1 ; i ++)
    {
        var trobj = tableobj.getElementsByTagName("tr")[i];
        var ipobj = trobj.getElementsByTagName("input")[0].value;
        var numobj = trobj.getElementsByTagName("input")[1].value;
        if((ipobj.length == 0) && (numobj.length == 0))
            continue;
        else if(ipobj.length == 0)
        {
            alert("第" + i + "行 : 请输入IP地址!");
            return null;
        }
        else if(numobj.length == 0)
        {
            alert("第" + i + "行 : 请输入IP个数!");
            return null;
        }
        if(!IsIpValue(ipobj))
        {
            alert("第" + i + "行 : 请输入正确的IP!");
            return null;
        }
        else if(!parseIsInt(numobj))
        {
            alert("第" + i + "行 : 请输入正确的IP个数!");
            return null;
        }
        ipobj = adjustIP(ipobj);
        var arr = ipobj.split(".");;
        var last = parseInt(arr[3]) + parseInt(numobj);
        var newIp = arr[0] + "." + arr[1] + "." + arr[2] + "." + last;
        if(!IsIpValue(newIp))
        {
            alert("第" + i + "行 : 请输入正确的IP地址和IP个数!");
            return null;
        }

        allIP += "&ip" + count + "=" + ipobj + "&n" + count + "=" + numobj;
        count ++;
    }
    allIP += "&sum=" + count;
    if(confirm(allIP))
    {
        location.href=allIP;
    }
    return obj;
}

function adjustIP(ip)
{
    var arr = ip.split(".");
    var newIP = parseInt(arr[0]) + "." + parseInt(arr[1]) + "." + parseInt(arr[2]) + "." + parseInt(arr[3]);

    return newIP;
}

</script>


</html>

