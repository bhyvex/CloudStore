<?php

require_once("../protocol/MessageHeader.class.php");

error_reporting(E_ALL);

class IoReq
{
    var $msg_header;
    var $load_data;
}

function socket_err_string()
{
    return socket_strerror(socket_last_error());
}

function send_data_to_server($ip , $port , $data)
{
    $socket = socket_create(AF_INET , SOCK_STREAM , 0);
    if(! $socket)
    {
        die("socket create error : " . socket_err_string());
    }
    
    $conn = socket_connect($socket , $ip , $port);
    if(! $conn)
    {
        die("socket connect error : " . socket_err_string());
    }
    if(! socket_set_block($socket))
    {
        die("socket set block error : " . socket_err_string());
    }

    $write_ret = socket_write($socket , $data);
    if(! $write_ret)
    {
        die("socket write data error : " . socket_err_string());
    }

    //header must be 20 byte length...
    $read_ret = socket_read($socket , HEADER_SIZE);
    if(! $read_ret)
    {
        die("socket read header data error : " . socket_err_string());
    }
    else if(strlen($read_ret) == 0)
    {
        return null;
    }
    else if(strlen($read_ret) != HEADER_SIZE)
    {
        die("socket read header data error : can not read all header !");
    }
    
    $head = new CHeader();
    $head->unpackage($read_ret);

    $load_length = $head->length();
    
    $io_msg = new IoReq();
    $io_msg->msg_header = $head;
    if($head->length() == 0)
    {
        return $io_msg;
    }
    $read_ret = socket_read($socket , $load_length);
    if(! $read_ret)
    {
        die("socket read load data error : " . socket_err_string());
    }
    else if(strlen($read_ret) == 0)
    {
        return null;
    }
    else if(strlen($read_ret) != $load_length)
    {
        die("socket read load data error : can not read all header !");
    }

    socket_close($socket);
    
    $io_msg->load_data = $read_ret;

    return $io_msg;
}

function send_data_to_cs($data)
{
    $config = parse_ini_file("config.ini" , true);
    if(! $config)
    {
        die("parse ini file " . "config.ini" . "error !");
    }

    $cs_ip = $config['server_ip']['cs_ip'];
    $cs_port = $config['server_ip']['cs_port'];

    return send_data_to_server($cs_ip , $cs_port , $data);
}

function send_data_to_ca($data)
{
    $config = parse_ini_file("config.ini" , true);
    if(! $config)
    {
        die("parse ini file " . "config.ini" . "error !");
    }

    $ca_ip = $config['server_ip']['ca_ip'];
    $ca_port = $config['server_ip']['ca_port'];

    return send_data_to_server($ca_ip , $ca_port , $data);
}

function send_data_to_rs($data)
{
    $config = parse_ini_file("config.ini" , true);
    if(! $config)
    {
        die("parse ini file " . "config.ini" . "error !");
    }

    $rs_ip = $config['server_ip']['rs_ip'];
    $rs_port = $config['server_ip']['rs_port'];

    return send_data_to_server($rs_ip , $rs_port , $data);
}

?>
