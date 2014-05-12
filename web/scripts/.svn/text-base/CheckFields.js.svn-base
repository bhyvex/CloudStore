/**************************登录页面检查**************************/
function CheckFields(){
	if (("" == document.login_form.user_name.value) || (null == document.login_form.user_name.value))
		{
		window.alert("用户名不能为空");
		document.login_form.user_name.focus();
		return false;
		}
	
	if (("" == document.login_form.user_passwd.value) || (null == document.login_form.user_passwd.value))
		{
		window.alert("密码不能为空");
		document.login_form.user_passwd.focus();
		return false;
		}
	return true;
}

/**************************上传项目页面检查**************************/
function CheckProject(){
	if (("" == document.upload_project_form.project_name.value) || (null == document.upload_project_form.project_name.value))
		{
		window.alert("项目名称不能为空");
		document.upload_project_form.project_name.focus();
		return false;
		}
	
	var xml = document.getElementById("project_xml_file");
	if (("" == xml.value) || (null == xml.value))
	{
		window.alert("项目XML文件不能为空");
		xml.focus();
		return false;
	}
	return true;
}

function CheckModulesName(){
	if (("" == document.upload_modules_form.project_module_name.value) || (null == document.upload_modules_form.project_module_name.value))
		{
		window.alert("模块名称不能为空");
		document.upload_modules_form.project_module_name.focus();
		return false;
		}
	return true;
}

function CheckInstallProject(){
	if (confirm("确认已经上传所有文件并安装系统？" + '\n' + "点击取消继续上传文件"))
		{
		location.href = "install_sys_to_dbc.php";
		}
	else
		{
		location.href = "upload_modules.php";
		}
}

function CheckProjectName(){
	var xmlhttp;
	if (window.XMLHttpRequest)
		{
		xmlhttp = new XMLHttpRequest;
		}
	else
		{
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
	
	var showurl = "CompareProjectName.php?project_name=" + document.getElementById("project_name").value;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
		document.getElementById("project_name_tips").innerHTML=xmlhttp.responseText;
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
}

function GetRowObj(ele){
	while (ele.tagName.toLowerCase() != "tr"){
		ele = ele.parentNode;
		if (ele.tagName.toLowerCase() == "table")
			return null;
	}
	return ele;
}

function GetFirstTdText(obj){
	var RowObj = GetRowObj(obj);
	return (RowObj.children[0].innerHTML);
}

function GetSecondTdText(obj){
	var RowObj = GetRowObj(obj);
	return (RowObj.children[1].innerHTML);
}

function ChangeSessionProjectName(obj){
	var NewProjectName = GetFirstTdText(obj);
	var xmlhttp;
	if (window.XMLHttpRequest)
		{
		xmlhttp = new XMLHttpRequest;
		}
	else
		{
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
	var showurl = "ChangeSessionProjectName.php?project_name=" + NewProjectName;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			location.href = "upload_modules.php";
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
}

function DeleteThisTempProject(obj){
	var DelProjectName = GetFirstTdText(obj);
	var xmlhttp;
	if (window.XMLHttpRequest)
		{
		xmlhttp = new XMLHttpRequest;
		}
	else
		{
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
	var showurl = "DeleteTempProject.php?project_name=" + DelProjectName;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			location.reload();
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
}

/**************************启动系统实例**************************/
function StartService(obj){
	var SystemType = GetFirstTdText(obj);
	if (window.XMLHttpRequest)
	{
	xmlhttp = new XMLHttpRequest;
	}
	else
	{
	xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	var showurl = "StartService.php?SystemType=" + SystemType;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			alert(xmlhttp.responseText);
			location.reload();
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
	
}

/**************************停止运行系统实例**************************/

function StopService(obj){
	var SystemType = GetFirstTdText(obj);
	var SystemInsId = GetSecondTdText(obj);
	if (window.XMLHttpRequest)
	{
	xmlhttp = new XMLHttpRequest;
	}
	else
	{
	xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	var showurl = "StopService.php?SystemType=" + SystemType + "&SystemInsId=" + SystemInsId;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			alert(xmlhttp.responseText);
			location.reload();
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
}

/**************************卸载系统**************************/

function UninstallService(obj){
	var SystemType = GetFirstTdText(obj);
	if (window.XMLHttpRequest)
	{
	xmlhttp = new XMLHttpRequest;
	}
	else
	{
	xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	var showurl = "UninstallService.php?SystemType=" + SystemType;
	xmlhttp.onreadystatechange=function()
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			alert(xmlhttp.responseText);
			location.reload();
		}
	}
	xmlhttp.open("GET", showurl, true);
	xmlhttp.send();
	
	
}

