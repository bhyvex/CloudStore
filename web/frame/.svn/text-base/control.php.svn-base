<?php 

session_start();

if(!$_SESSION["online"])
{
	echo "<script type='text/javascript'>";
	echo "window.alert('please login');";
	echo "top.location.href='../../index.php';";   //需要修改成top.location.href
	echo "</script>";
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="../style/style2.css" />
<script type="text/javascript" src="../scripts/dtree.js"></script>
<title>CStorage管理员控制平台</title>
</head>

<body id="body_navi" background="img/bg.gif" onLoad="startclock()">
<table class="mytable">
<tr><th>管理员控制平台</th></tr>
</table>

<div class="dtree">

<p><a href="javascript: d.openAll();">Open All</a> | <a href="javascript: d.closeAll();">Close All</a></p>

	<script type="text/javascript">
		d = new dTree('d');

		d.add(0,-1,'Options');
		
		d.add(1,0,'系统信息');
        d.add(2,1,'系统启停','../implement/show/show_system_control.php','Module Control','showFrame','../images/24px/system.png');
//		d.add(3,1,'System Status','../implement/show/show_system_status.php','Module status','showFrame','../images/user/sys_info.png');
        d.add(4,1,'查看MU规则','../implement/show/show_rule.php?type=mu','System MU','showFrame','../images/24px/info.png');
        d.add(5,1,'查看SU规则','../implement/show//show_rule.php?type=su','System SU','showFrame','../images/24px/info.png');

        d.add(10,0,"用户控制");
        d.add(11,10,'显示所有用户','../implement/show/show_all_user.php',"Display All User",'showFrame','../images/24px/all_user.png');
		d.add(12,10,'添加用户','../implement/show/add_new_user.php','Add new User','showFrame','../images/24px/add_user.png');
		d.add(13,10,'删除用户','../implement/show/delete_user.php','Delete An User','showFrame','../images/24px/del_user.png');

		d.add(16,0,'系统控制');
		d.add(18,16,'MU集群控制','../implement/show/node_operation.php?type=mu','MU Extent or Migration','showFrame','../images/24px/set.png');
        d.add(19,16,'SU集群控制','../implement/show/node_operation.php?type=su','SU Extent or Migration','showFrame','../images/24px/set.png');
        d.add(22,16,'初始化部署','../implement/show/init_system.php','System Init','showFrame','../images/24px/computer.png');
        d.add(24,16,'系统垃圾回收','../implement/show/rubbish_recycler.php','Globle SU Recycler','showFrame','../images/24px/recycler.png');

		document.write(d);

	</script>

</div>

<div id="userDIV">
<?php
if (isset($_SESSION["UserName"]))
{
	echo "<table class='mytable'>";
	echo "<tr><th>" . "Welcome : " . $_SESSION["UserName"] . "&nbsp;&nbsp;" . "<a href=logout.php>Logout</a>" . "</th></tr>";
?>
<tr>
<th><form name="clock" style="color:#1714C1">   
<input name="thetime" style="background:transparent; border:0; color:#1714C1;" size="30" ReadOnly="true">   
</form>  
</th>
</tr>
<?
	echo "</table>";
} 
?>

<script language="javascript">   
var timerID = null;   
var timerRunning = false;   
function stopclock ()
{   
	if(timerRunning)   
		clearTimeout(timerID);   
	timerRunning = false;
}   
function startclock ()
{   
   stopclock();   
   showtime();   
}   
function showtime () 
{   
    var now = new Date(); 
   // var year = now.getFullYear(); 
  //  var month = now.getUTCMonth();
   // var day = now.getDay();
    var hours = now.getHours();   
    var minutes = now.getMinutes();   
    var seconds = now.getSeconds(); 
    //var timeValue = ((hours >= 12) ? " PM: " : " AM: " )    ;
    var timeValue  = ((hours >12) ? hours -12 :hours) ;
    timeValue += ((minutes < 10) ? ":0" : ":") + minutes  ; 
    timeValue += ((seconds < 10) ? ":0" : ":") + seconds   ;
    timeValue += ((hours >= 12) ? "  PM" : "  AM" );
    document.clock.thetime.value = "Current Time: " + timeValue ;   
    timerID = setTimeout("showtime()",1000);   
    timerRunning = true;   
}   
</script>   
</div>
</body>
</html>
