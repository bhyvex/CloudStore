<?php

require_once(dirname(__FILE__). '/' . 'message/pb_message.php');

class Module_Name extends PBEnum
{
  const NO_module  = 0;
  const RS_module  = 1;
  const CA_module  = 2;
  const CS_module  = 3;
  const MU_module  = 4;
  const SU_module  = 5;
}
class Module_Item extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "Module_Name";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
    $this->fields["4"] = "PBInt";
    $this->values["4"] = "";
    $this->fields["5"] = "PBInt";
    $this->values["5"] = "";
  }
  function Module_IP()
  {
    return $this->_get_value("1");
  }
  function set_Module_IP($value)
  {
    return $this->_set_value("1", $value);
  }
  function module()
  {
    return $this->_get_value("2");
  }
  function set_module($value)
  {
    return $this->_set_value("2", $value);
  }
  function mem()
  {
    return $this->_get_value("3");
  }
  function set_mem($value)
  {
    return $this->_set_value("3", $value);
  }
  function disk()
  {
    return $this->_get_value("4");
  }
  function set_disk($value)
  {
    return $this->_set_value("4", $value);
  }
  function net()
  {
    return $this->_get_value("5");
  }
  function set_net($value)
  {
    return $this->_set_value("5", $value);
  }
}
class IP_scope extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function Start_IP()
  {
    return $this->_get_value("1");
  }
  function set_Start_IP($value)
  {
    return $this->_set_value("1", $value);
  }
  function Serial_num()
  {
    return $this->_get_value("2");
  }
  function set_Serial_num($value)
  {
    return $this->_set_value("2", $value);
  }
}
class Map_Item extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = array();
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function Module_IP($offset)
  {
    $v = $this->_get_arr_value("1", $offset);
    return $v->get_value();
  }
  function append_Module_IP($value)
  {
    $v = $this->_add_arr_value("1");
    $v->set_value($value);
  }
  function set_Module_IP($index, $value)
  {
    $v = new $this->fields["1"]();
    $v->set_value($value);
    $this->_set_arr_value("1", $index, $v);
  }
  function remove_last_Module_IP()
  {
    $this->_remove_last_arr_value("1");
  }
  function Module_IP_size()
  {
    return $this->_get_arr_size("1");
  }
  function bucket_num()
  {
    return $this->_get_value("2");
  }
  function set_bucket_num($value)
  {
    return $this->_set_value("2", $value);
  }
}
class Bucket_Item extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function bucket_num()
  {
    return $this->_get_value("1");
  }
  function set_bucket_num($value)
  {
    return $this->_set_value("1", $value);
  }
  function bucket_load()
  {
    return $this->_get_value("2");
  }
  function set_bucket_load($value)
  {
    return $this->_set_value("2", $value);
  }
}
class Bucket_List extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Bucket_Item";
    $this->values["1"] = array();
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function bucket_item($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bucket_item()
  {
    return $this->_add_arr_value("1");
  }
  function set_bucket_item($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_bucket_item()
  {
    $this->_remove_last_arr_value("1");
  }
  function bucket_item_size()
  {
    return $this->_get_arr_size("1");
  }
  function mu_ip()
  {
    return $this->_get_value("2");
  }
  function set_mu_ip($value)
  {
    return $this->_set_value("2", $value);
  }
}
class pb_MSG_MPC_MU_CREATE_USER extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function uid()
  {
    return $this->_get_value("1");
  }
  function set_uid($value)
  {
    return $this->_set_value("1", $value);
  }
  function quota()
  {
    return $this->_get_value("2");
  }
  function set_quota($value)
  {
    return $this->_set_value("2", $value);
  }
}
class pb_MSG_MPC_MU_DELETE_USER extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function uid()
  {
    return $this->_get_value("1");
  }
  function set_uid($value)
  {
    return $this->_set_value("1", $value);
  }
}
class pb_MSG_MPC_CS_RUBBISH_RECYCLE extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function scale()
  {
    return $this->_get_value("1");
  }
  function set_scale($value)
  {
    return $this->_set_value("1", $value);
  }
}
class pb_MSG_MPC_CS_SU_MIGRATE_BUCKET extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
  }
  function source_ip()
  {
    return $this->_get_value("1");
  }
  function set_source_ip($value)
  {
    return $this->_set_value("1", $value);
  }
  function destination_ip()
  {
    return $this->_get_value("2");
  }
  function set_destination_ip($value)
  {
    return $this->_set_value("2", $value);
  }
  function bucket_num()
  {
    return $this->_get_value("3");
  }
  function set_bucket_num($value)
  {
    return $this->_set_value("3", $value);
  }
}
class pb_MSG_MPC_CS_MU_MIGRATE_BUCKET extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
  }
  function source_ip()
  {
    return $this->_get_value("1");
  }
  function set_source_ip($value)
  {
    return $this->_set_value("1", $value);
  }
  function destination_ip()
  {
    return $this->_get_value("2");
  }
  function set_destination_ip($value)
  {
    return $this->_set_value("2", $value);
  }
  function bucket_num()
  {
    return $this->_get_value("3");
  }
  function set_bucket_num($value)
  {
    return $this->_set_value("3", $value);
  }
}
class pb_MSG_MPC_CS_MU_EXTENT_BUCKET extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function new_mod()
  {
    return $this->_get_value("1");
  }
  function set_new_mod($value)
  {
    return $this->_set_value("1", $value);
  }
}
class pb_MSG_MPC_CS_SU_EXTENT_BUCKET extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function new_mod()
  {
    return $this->_get_value("1");
  }
  function set_new_mod($value)
  {
    return $this->_set_value("1", $value);
  }
}
class pb_MSG_MPC_CS_GET_SU_RULE_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Map_Item";
    $this->values["1"] = array();
  }
  function map_bucket($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_map_bucket()
  {
    return $this->_add_arr_value("1");
  }
  function set_map_bucket($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_map_bucket()
  {
    $this->_remove_last_arr_value("1");
  }
  function map_bucket_size()
  {
    return $this->_get_arr_size("1");
  }
}
class pb_MSG_MPC_CS_GET_MU_RULE_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Map_Item";
    $this->values["1"] = array();
  }
  function map_bucket($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_map_bucket()
  {
    return $this->_add_arr_value("1");
  }
  function set_map_bucket($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_map_bucket()
  {
    $this->_remove_last_arr_value("1");
  }
  function map_bucket_size()
  {
    return $this->_get_arr_size("1");
  }
}
class IP_bucket extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = array();
  }
  function ip()
  {
    return $this->_get_value("1");
  }
  function set_ip($value)
  {
    return $this->_set_value("1", $value);
  }
  function buckets($offset)
  {
    $v = $this->_get_arr_value("2", $offset);
    return $v->get_value();
  }
  function append_buckets($value)
  {
    $v = $this->_add_arr_value("2");
    $v->set_value($value);
  }
  function set_buckets($index, $value)
  {
    $v = new $this->fields["2"]();
    $v->set_value($value);
    $this->_set_arr_value("2", $index, $v);
  }
  function remove_last_buckets()
  {
    $this->_remove_last_arr_value("2");
  }
  function buckets_size()
  {
    return $this->_get_arr_size("2");
  }
}
class pb_MSG_MPC_CS_SU_INIT_HASH extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "IP_scope";
    $this->values["1"] = array();
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
  }
  function module_ip($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_module_ip()
  {
    return $this->_add_arr_value("1");
  }
  function set_module_ip($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_module_ip()
  {
    $this->_remove_last_arr_value("1");
  }
  function module_ip_size()
  {
    return $this->_get_arr_size("1");
  }
  function mod()
  {
    return $this->_get_value("2");
  }
  function set_mod($value)
  {
    return $this->_set_value("2", $value);
  }
  function duplicate_num()
  {
    return $this->_get_value("3");
  }
  function set_duplicate_num($value)
  {
    return $this->_set_value("3", $value);
  }
}
class pb_MSG_MPC_CS_MU_INIT_HASH extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "IP_scope";
    $this->values["1"] = array();
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
  }
  function module_ip($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_module_ip()
  {
    return $this->_add_arr_value("1");
  }
  function set_module_ip($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_module_ip()
  {
    $this->_remove_last_arr_value("1");
  }
  function module_ip_size()
  {
    return $this->_get_arr_size("1");
  }
  function mod()
  {
    return $this->_get_value("2");
  }
  function set_mod($value)
  {
    return $this->_set_value("2", $value);
  }
  function duplicate_num()
  {
    return $this->_get_value("3");
  }
  function set_duplicate_num($value)
  {
    return $this->_set_value("3", $value);
  }
}
class pb_MSG_MPC_CS_MU_BUCKET_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Bucket_List";
    $this->values["1"] = array();
  }
  function bucket_list($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bucket_list()
  {
    return $this->_add_arr_value("1");
  }
  function set_bucket_list($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_bucket_list()
  {
    $this->_remove_last_arr_value("1");
  }
  function bucket_list_size()
  {
    return $this->_get_arr_size("1");
  }
}
class pb_MSG_MPC_CS_SU_BUCKET_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Bucket_List";
    $this->values["1"] = array();
  }
  function bucket_list($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_bucket_list()
  {
    return $this->_add_arr_value("1");
  }
  function set_bucket_list($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_bucket_list()
  {
    $this->_remove_last_arr_value("1");
  }
  function bucket_list_size()
  {
    return $this->_get_arr_size("1");
  }
}
class Node_Block extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
    $this->fields["4"] = "PBInt";
    $this->values["4"] = "";
  }
  function node_ip()
  {
    return $this->_get_value("1");
  }
  function set_node_ip($value)
  {
    return $this->_set_value("1", $value);
  }
  function bucket_number()
  {
    return $this->_get_value("2");
  }
  function set_bucket_number($value)
  {
    return $this->_set_value("2", $value);
  }
  function block_number()
  {
    return $this->_get_value("3");
  }
  function set_block_number($value)
  {
    return $this->_set_value("3", $value);
  }
  function storage_limit()
  {
    return $this->_get_value("4");
  }
  function set_storage_limit($value)
  {
    return $this->_set_value("4", $value);
  }
}
class pb_MSG_MPC_CS_GET_BLOCK_NUM_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Node_Block";
    $this->values["1"] = array();
    $this->fields["2"] = "Node_Block";
    $this->values["2"] = array();
    $this->fields["3"] = "PBInt";
    $this->values["3"] = "";
    $this->fields["4"] = "PBInt";
    $this->values["4"] = "";
  }
  function su_node_list($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_su_node_list()
  {
    return $this->_add_arr_value("1");
  }
  function set_su_node_list($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_su_node_list()
  {
    $this->_remove_last_arr_value("1");
  }
  function su_node_list_size()
  {
    return $this->_get_arr_size("1");
  }
  function mu_node_list($offset)
  {
    return $this->_get_arr_value("2", $offset);
  }
  function add_mu_node_list()
  {
    return $this->_add_arr_value("2");
  }
  function set_mu_node_list($index, $value)
  {
    $this->_set_arr_value("2", $index, $value);
  }
  function remove_last_mu_node_list()
  {
    $this->_remove_last_arr_value("2");
  }
  function mu_node_list_size()
  {
    return $this->_get_arr_size("2");
  }
  function su_dup()
  {
    return $this->_get_value("3");
  }
  function set_su_dup($value)
  {
    return $this->_set_value("3", $value);
  }
  function mu_dup()
  {
    return $this->_get_value("4");
  }
  function set_mu_dup($value)
  {
    return $this->_set_value("4", $value);
  }
}
class Report_Bucket_List extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Bucket_List";
    $this->values["1"] = "";
    $this->fields["2"] = "PBInt";
    $this->values["2"] = "";
  }
  function node_bucket_list()
  {
    return $this->_get_value("1");
  }
  function set_node_bucket_list($value)
  {
    return $this->_set_value("1", $value);
  }
  function node_storage_limit()
  {
    return $this->_get_value("2");
  }
  function set_node_storage_limit($value)
  {
    return $this->_set_value("2", $value);
  }
}
class pb_MSG_CS_MPC_REPORT_BUCKET_LIST extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Report_Bucket_List";
    $this->values["1"] = array();
    $this->fields["3"] = "Report_Bucket_List";
    $this->values["3"] = array();
  }
  function mu_bucket_list($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_mu_bucket_list()
  {
    return $this->_add_arr_value("1");
  }
  function set_mu_bucket_list($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_mu_bucket_list()
  {
    $this->_remove_last_arr_value("1");
  }
  function mu_bucket_list_size()
  {
    return $this->_get_arr_size("1");
  }
  function su_bucket_list($offset)
  {
    return $this->_get_arr_value("3", $offset);
  }
  function add_su_bucket_list()
  {
    return $this->_add_arr_value("3");
  }
  function set_su_bucket_list($index, $value)
  {
    $this->_set_arr_value("3", $index, $value);
  }
  function remove_last_su_bucket_list()
  {
    $this->_remove_last_arr_value("3");
  }
  function su_bucket_list_size()
  {
    return $this->_get_arr_size("3");
  }
}
class pb_MSG_MPC_DS_MODULES_INFO_ACK extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "Module_Item";
    $this->values["1"] = array();
  }
  function module_list($offset)
  {
    return $this->_get_arr_value("1", $offset);
  }
  function add_module_list()
  {
    return $this->_add_arr_value("1");
  }
  function set_module_list($index, $value)
  {
    $this->_set_arr_value("1", $index, $value);
  }
  function remove_last_module_list()
  {
    $this->_remove_last_arr_value("1");
  }
  function module_list_size()
  {
    return $this->_get_arr_size("1");
  }
}
class pb_MSG_MPC_DS_START_MODULE extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
    $this->fields["2"] = "Module_Name";
    $this->values["2"] = "";
    $this->fields["3"] = "PBString";
    $this->values["3"] = "";
  }
  function Module_IP()
  {
    return $this->_get_value("1");
  }
  function set_Module_IP($value)
  {
    return $this->_set_value("1", $value);
  }
  function module_name()
  {
    return $this->_get_value("2");
  }
  function set_module_name($value)
  {
    return $this->_set_value("2", $value);
  }
  function args()
  {
    return $this->_get_value("3");
  }
  function set_args($value)
  {
    return $this->_set_value("3", $value);
  }
}
class pb_MSG_MPC_DS_STOP_MODULE extends PBMessage
{
  var $wired_type = PBMessage::WIRED_LENGTH_DELIMITED;
  public function __construct($reader=null)
  {
    parent::__construct($reader);
    $this->fields["1"] = "PBInt";
    $this->values["1"] = "";
  }
  function Module_IP()
  {
    return $this->_get_value("1");
  }
  function set_Module_IP($value)
  {
    return $this->_set_value("1", $value);
  }
}
?>
