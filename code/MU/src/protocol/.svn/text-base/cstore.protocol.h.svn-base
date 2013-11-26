#ifndef __CSTORE_PROTOCOL_H__
#define __CSTORE_PROTOCOL_H__




/*macro of protocol*/

////////////////////////////////////////////////////////////////
//CA00
////////////////////////////////////////////////////////////////
/********************* SYS - CA ***********************/
const uint32_t MSG_SYS_CA_LOG_IN = 0x00000010;
//uid, pwd
const uint32_t MSG_SYS_CA_LOG_IN_ACK = 0x00000011;
//token(content)

/********************* MPC - CA ***********************/
const uint32_t MSG_MPC_CA_CREATE_USER = 0x00010010;
//uid, pwd, content
const uint32_t MSG_MPC_CA_CREATE_USER_ACK = 0x00010011;

const uint32_t MSG_MPC_CA_DELETE_USER = 0x00010020;
//uid
const uint32_t MSG_MPC_CA_DELETE_USER_ACK = 0x00010021;



////////////////////////////////////////////////////////////////
//RS01
////////////////////////////////////////////////////////////////
/********************* CS - RS ***********************/
const uint32_t MSG_CS_RS_RELOAD_MU_HASH = 0x01000010;
const uint32_t MSG_CS_RS_RELOAD_MU_HASH_ACK = 0x01000011;

const uint32_t MSG_CS_RS_RELOAD_SU_HASH = 0x01000020;
const uint32_t MSG_CS_RS_RELOAD_SU_HASH_ACK = 0x01000021;

/********************* SYS - RS ***********************/
const uint32_t MSG_SYS_RS_UPDATE_MU_HASH = 0x01010030; //ZERO!!!
//version
const uint32_t MSG_SYS_RS_UPDATE_MU_HASH_ACK = 0x01010031;
//(latest_rule, version) or version_list

const uint32_t MSG_SYS_RS_UPDATE_ALL_MU_HASH = 0x01010032;
const uint32_t MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK = 0x01010033;

const uint32_t MSG_SYS_RS_UPDATE_SU_HASH = 0x01010040; //ZERO!!!
//version
const uint32_t MSG_SYS_RS_UPDATE_SU_HASH_ACK = 0x01010041;
//(latest_rule, version) or version_list

const uint32_t MSG_SYS_RS_UPDATE_ALL_SU_HASH = 0x01010042;
const uint32_t MSG_SYS_RS_UPDATE_ALL_SU_HASH_ACK = 0x01010043;

const uint32_t MSG_SYS_RS_GET_MU_BUCKET_ITEM = 0x01010050;
//bucket_nr
const uint32_t MSG_SYS_RS_GET_MU_BUCKET_ITEM_ACK = 0x01010051;
//mu_items

const uint32_t MSG_SYS_RS_GET_SU_BUCKET_ITEM = 0x01010060;
//bucket_nr
const uint32_t MSG_SYS_RS_GET_SU_BUCKET_ITEM_ACK = 0x01010061;
//su_items



////////////////////////////////////////////////////////////////
//CS02
////////////////////////////////////////////////////////////////
/********************* RS - CS ***********************/
const uint32_t MSG_RS_CS_HEARTBEAT = 0x02000010;
const uint32_t MSG_RS_CS_HEARTBEAT_ACK = 0x02000011;

const uint32_t MSG_RS_CS_UPDATE_MU_HASH = 0x02000020; 
//version
const uint32_t MSG_RS_CS_UPDATE_MU_HASH_ACK = 0x02000021;
//(latest_rule, version_list) or version_list

const uint32_t MSG_RS_CS_UPDATE_ALL_MU_HASH = 0x02000022; 
//version
const uint32_t MSG_RS_CS_UPDATE_ALL_MU_HASH_ACK = 0x02000023;

const uint32_t MSG_RS_CS_UPDATE_SU_HASH = 0x02000030; 
//version
const uint32_t MSG_RS_CS_UPDATE_SU_HASH_ACK = 0x02000031;
//(latest_rule, version_list) or version_list

const uint32_t MSG_RS_CS_UPDATE_ALL_SU_HASH = 0x02000032; 
//version
const uint32_t MSG_RS_CS_UPDATE_ALL_SU_HASH_ACK = 0x02000033;

/********************* MU - CS ***********************/
const uint32_t MSG_MU_CS_HEARTBEAT_HANDSHAKE = 0x02010010;
//storage_load_limit
const uint32_t MSG_MU_CS_HEARTBEAT_HANDSHAKE_ACK = 0x02010011;

const uint32_t MSG_MU_CS_HEARTBEAT = 0x02010020;
//storage_load_now, bucket_load_list
const uint32_t MSG_MU_CS_HEARTBEAT_ACK = 0x02010021;

/********************* SU - CS ***********************/
const uint32_t MSG_SU_CS_HEARTBEAT_HANDSHAKE = 0x02020010;
//storage_load_limit
const uint32_t MSG_SU_CS_HEARTBEAT_HANDSHAKE_ACK = 0x02020011;

const uint32_t MSG_SU_CS_HEARTBEAT = 0x02020020;
//storage_load_now, bucket_load_list
const uint32_t MSG_SU_CS_HEARTBEAT_ACK = 0x02020021;

/********************* MPC - CS ***********************/
const uint32_t MSG_MPC_CS_RUBBISH_RECYCLE = 0x02030010;
//scale
const uint32_t MSG_MPC_CS_RUBBISH_RECYCLE_ACK = 0x02030011;

const uint32_t MSG_MPC_CS_MU_MIGRATE_BUCKET = 0x02030020;
//source_mu, destination_mu, bucket_nr
const uint32_t MSG_MPC_CS_MU_MIGRATE_BUCKET_ACK = 0x02030021;

const uint32_t MSG_MPC_CS_MU_EXTENT_BUCKET = 0x02030030;
//new_mod
const uint32_t MSG_MPC_CS_MU_EXTENT_BUCKET_ACK = 0x02030031;

const uint32_t MSG_MPC_CS_SU_MIGRATE_BUCKET = 0x02030040;
//source_su, destination_su, bucket_nr
const uint32_t MSG_MPC_CS_SU_MIGRATE_BUCKET_ACK = 0x02030041;

const uint32_t MSG_MPC_CS_SU_EXTENT_BUCKET = 0x02030050;
//new_mod
const uint32_t MSG_MPC_CS_SU_EXTENT_BUCKET_ACK = 0x02030051;

const uint32_t MSG_MPC_CS_GET_MU_RULE = 0x02030060;
const uint32_t MSG_MPC_CS_GET_MU_RULE_ACK = 0x02030061;
//rule

const uint32_t MSG_MPC_CS_MU_INIT_HASH = 0x02030070;
const uint32_t MSG_MPC_CS_MU_INIT_HASH_ACK = 0x02030071;

const uint32_t MSG_MPC_CS_GET_SU_RULE = 0x02030080;
const uint32_t MSG_MPC_CS_GET_SU_RULE_ACK = 0x02030081;
//rule

const uint32_t MSG_MPC_CS_SU_INIT_HASH = 0x02030090;
const uint32_t MSG_MPC_CS_SU_INIT_HASH_ACK = 0x02030091;

const uint32_t MSG_MPC_CS_MU_BUCKET = 0x02030100;
const uint32_t MSG_MPC_CS_MU_BUCKET_ACK = 0x02030110;

////////////////////////////////////////////////////////////////
//MU03
////////////////////////////////////////////////////////////////
/********************* MPC - MU ***********************/
const uint32_t MSG_MPC_MU_CREATE_USER = 0x03000010;
//uid
const uint32_t MSG_MPC_MU_CREATE_USER_ACK = 0x03000011;

const uint32_t MSG_MPC_MU_DELETE_USER = 0x03000020;
//uid
const uint32_t MSG_MPC_MU_DELETE_USER_ACK = 0x03000021;

/********************* CS - MU ***********************/
const uint32_t MSG_CS_MU_RUBBISH_RECYCLE = 0x03010010;
//scale
const uint32_t MSG_CS_MU_RUBBISH_RECYCLE_ACK = 0x03010011;

const uint32_t MSG_CS_MU_EXTENT_BUCKET_PREPARE = 0x03010020;

const uint32_t MSG_CS_MU_EXTENT_BUCKET_PREPARE_ACK = 0x03010021;

const uint32_t MSG_CS_MU_EXTENT_BUCKET= 0x03010022;

const uint32_t MSG_CS_MU_EXTENT_BUCKET_ACK = 0x03010023;

const uint32_t MSG_CS_MU_MIGRATE_BUCKET = 0x03010030;
//source_mu, bucket_nr
const uint32_t MSG_CS_MU_MIGRATE_BUCKET_ACK = 0x03010031;

const uint32_t MSG_CS_MU_DELETE_BUCKET = 0x03010040;
//bucket_nr
const uint32_t MSG_CS_MU_DELETE_BUCKET_ACK = 0x03010041;

const uint32_t MSG_CS_MU_SET_BUCKET_STATE = 0x03010050;
//bucket_nr, state
const uint32_t MSG_CS_MU_SET_BUCKET_STATE_ACK = 0x03010051;

/********************* MU - MU ***********************/
const uint32_t MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE = 0x03020010;
const uint32_t MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK = 0x03020011;

const uint32_t MSG_MU_MU_MIGRATE_BUCKET_DATA = 0x03020020;
const uint32_t MSG_MU_MU_MIGRATE_BUCKET_DATA_ACK = 0x03020021;

const uint32_t MSG_MU_MU_SYNC_HANDSHAKE = 0x03020030;
const uint32_t MSG_MU_MU_SYNC_HANDSHAKE_ACK = 0x03020031;

const uint32_t MSG_MU_MU_PUSH_LOG = 0x03020040;
const uint32_t MSG_MU_MU_PUSH_LOG_ACK = 0x03020041;

const uint32_t MSG_MU_MU_PULL_LOG = 0x03020050;
const uint32_t MSG_MU_MU_PULL_LOG_ACK = 0x03020051;

/********************* SYS - MU ***********************/
const uint32_t MSG_SYS_MU_GET_USER_INFO = 0x03030010;
//uid, token
const uint32_t MSG_SYS_MU_GET_USER_INFO_ACK = 0x03030011;
//sequence

const uint32_t MSG_SYS_MU_GET_USER_LOG = 0x03030020;
//uid, token, sequence
const uint32_t MSG_SYS_MU_GET_USER_LOG_ACK = 0x03030021;
//log

const uint32_t MSG_SYS_MU_PUT_DIR = 0x03030030;
//uid, token, path
const uint32_t MSG_SYS_MU_PUT_DIR_ACK = 0x03030031;

const uint32_t MSG_SYS_MU_GET_DIR = 0x03030040;
//uid, token, path
const uint32_t MSG_SYS_MU_GET_DIR_ACK = 0x03030041;
//dir_names and file_names 

const uint32_t MSG_SYS_MU_GET_DIR2 = 0x03030050;
//uid, token, path
const uint32_t MSG_SYS_MU_GET_DIR2_ACK = 0x03030051;
//dir_names and file_metas 

const uint32_t MSG_SYS_MU_STAT_DIR = 0x03030152;

const uint32_t MSG_SYS_MU_STAT_DIR_ACK = 0x03030153;

const uint32_t MSG_SYS_MU_DEL_DIR = 0x03030060;
//uid, token, path
const uint32_t MSG_SYS_MU_DEL_DIR_ACK = 0x03030061;

const uint32_t MSG_SYS_MU_MOV_DIR = 0x03030070;
//uid, token, source_path, destination_path
const uint32_t MSG_SYS_MU_MOV_DIR_ACK = 0x03030071;

const uint32_t MSG_SYS_MU_PUT_FILE = 0x03030080;
//uid, token, path, size, auth, time, version, block_list
const uint32_t MSG_SYS_MU_PUT_FILE_ACK = 0x03030081;

const uint32_t MSG_SYS_MU_GET_FILE = 0x03030090;
//uid, token, path
const uint32_t MSG_SYS_MU_GET_FILE_ACK = 0x03030091;
//size, auth, time, version, block_list

const uint32_t MSG_SYS_MU_DEL_FILE = 0x03030100;
//uid, token, path
const uint32_t MSG_SYS_MU_DEL_FILE_ACK = 0x03030101;

const uint32_t MSG_SYS_MU_MOV_FILE = 0x03030110;
//uid, token, source_path, destination_path
const uint32_t MSG_SYS_MU_MOV_FILE_ACK = 0x03030111;



////////////////////////////////////////////////////////////////
//SU04
////////////////////////////////////////////////////////////////
/********************* CS - SU ***********************/
const uint32_t MSG_CS_SU_RUBBISH_RECYCLE = 0x04000010;
//scale
const uint32_t MSG_CS_SU_RUBBISH_RECYCLE_ACK = 0x04000011;

const uint32_t MSG_CS_SU_EXTENT_BUCKET = 0x04000020;
//new_mod
const uint32_t MSG_CS_SU_EXTENT_BUCKET_ACK = 0x04000021;

const uint32_t MSG_CS_SU_MIGRATE_BUCKET = 0x04000030;
//source_su, bucket_nr
const uint32_t MSG_CS_SU_MIGRATE_BUCKET_ACK = 0x04000031;

const uint32_t MSG_CS_SU_DELETE_BUCKET = 0x04000040;
//bucket_nr
const uint32_t MSG_CS_SU_DELETE_BUCKET_ACK = 0x04000041;

const uint32_t MSG_CS_SU_SET_BUCKET_STATE = 0x04000050;
//bucket_nr, state
const uint32_t MSG_CS_SU_SET_BUCKET_STATE_ACK = 0x04000051;

/********************* SYS - SU ***********************/
const uint32_t MSG_SYS_SU_CHK_BLOCK = 0x04010010;
//infohash
const uint32_t MSG_SYS_SU_CHK_BLOCK_ACK = 0x04010011;

const uint32_t MSG_SYS_SU_PUT_BLOCK = 0x04010020;
//infohash, data
const uint32_t MSG_SYS_SU_PUT_BLOCK_ACK = 0x04010021;

const uint32_t MSG_SYS_SU_GET_BLOCK = 0x04010030;
//infohash
const uint32_t MSG_SYS_SU_GET_BLOCK_ACK = 0x04010031;
//data

const uint32_t MSG_SYS_SU_DEL_BLOCK = 0x04010040;
//infohash
const uint32_t MSG_SYS_SU_DEL_BLOCK_ACK = 0x04010041;

const uint32_t MSG_SYS_SU_DEL_BLOCK2 = 0x04010050;
//infohash
const uint32_t MSG_SYS_SU_DEL_BLOCK2_ACK = 0x04010051;

/********************* SU - SU ***********************/
const uint32_t MSG_SU_SU_HEARTBEAT = 0x04020010;
//su_sequence

const uint32_t MSG_SU_SU_PUT_BLOCK = 0x04020020;
//infohash, sequence, data
const uint32_t MSG_SU_SU_PUT_BLOCK_ACK = 0x04020021;

const uint32_t MSG_SU_SU_GET_BLOCK = 0x04020030;
//infohash
const uint32_t MSG_SU_SU_GET_BLOCK_ACK = 0x04020031;
//data

const uint32_t MSG_SU_SU_DEL_BLOCK = 0x04020040;
//infohash, sequence
const uint32_t MSG_SU_SU_DEL_BLOCK_ACK = 0x04020041;

const uint32_t MSG_SU_SU_DEL_BLOCK2 = 0x04020050;
//infohash, sequence
const uint32_t MSG_SU_SU_DEL_BLOCK2_ACK = 0x04020051;

const uint32_t MSG_SU_SU_GET_BLOCK_LIST = 0x04020060;
//bucket_nr, start_sequence, end_sequence
const uint32_t MSG_SU_SU_GET_BLOCK_LIST_ACK = 0x04020061;
//bucket_nr, start_sequence, end_sequence, infohash_list

const uint32_t MSG_SU_SU_GET_CURRENT_SEQ = 0x04020070;
//bucket_nr
const uint32_t MSG_SU_SU_GET_CURRENT_SEQ_ACK = 0x04020071;
//bucket_nr, current_sequence

const uint32_t MSG_SU_SU_HEARTBEAT_BUILD = 0x04020080;
//bucket_nr
const uint32_t MSG_SU_SU_HEARTBEAT_BUILD_ACK = 0x04020081;


////////////////////////////////////////////////////////////////
//CS_ERROR_NUM
////////////////////////////////////////////////////////////////

const uint32_t CS_OK = 0x10000000;
const uint32_t CS_ERROR = 0x10000001;
const uint32_t MU_HAND_ERROR = 0x10000002;
const uint32_t SU_HAND_ERROR = 0x10000003;
const uint32_t MU_HEART_ERROR= 0x10000004;
const uint32_t SU_HEART_ERROR= 0x10000005;
const uint32_t VERSION_ERROR = 0x10000006;
const uint32_t MU_ERROR = 0x10000007;
const uint32_t SU_ERROR = 0x10000008;
const uint32_t SOURCE_ERROR = 0x10000009;
const uint32_t DESTINATION_ERROR = 0x10000010;
const uint32_t SQL_HANDLE_ERROR = 0x10000011;

const uint32_t RULER_OK = 0x10000018;//给客户端返回规则正常
const uint32_t RULER_ERROR= 0x10000019;//
const uint32_t RS_OK = 0x10000012;
const uint32_t RS_ERROR = 0x10000013;

const uint32_t CS_SU_DELETE = 0x10000017;

/////////////////////////////////////////////////////////////////
// SU_ERROR_NUM
/////////////////////////////////////////////////////////////////

const uint32_t SU_OK = 0x02000000;
const uint32_t SU_INTERNAL_ERROR = 0x02000001;
const uint32_t SU_LOCATE_ERROR = 0x02000002;
const uint32_t SU_SERVICE_DENY = 0x02000003;
const uint32_t SU_IDX_EXIST = 0x02000004;
const uint32_t SU_IDX_NOT_EXIST = 0x02000005;
const uint32_t SU_SOURCE_ERROR = 0x02000006;



/////////////////////////////////////////////////////////////////
// CA_ERROR_NUM
/////////////////////////////////////////////////////////////////
const uint32_t CA_OK = 0x03000000;
const uint32_t CA_ERROR = 0x03000001;



#endif  // __CSTORE_PROTOCOL_H__

