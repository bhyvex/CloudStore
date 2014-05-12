<?php
	//   message header macro define
	
    //create user
    define("MSG_MPC_CA_CREATE_USER" , 0x00010010);
    define("MSG_MPC_CA_CREATE_USER_ACK" , 0x00010011);

    //delete user
    define("MSG_MPC_CA_DELETE_USER" , 0x00010020);
    define("MSG_MPC_CA_DELETE_USER_ACK" , 0x00010021);

	// rubbish recycler
    define("MSG_MPC_CS_RUBBISH_RECYCLE" , 0x02030010);
    define("MSG_MPC_CS_RUBBISH_RECYCLE_ACK" , 0x02030011);
	
	// mu and su migration
    define("MSG_MPC_CS_MU_MIGRATE_BUCKET" , 0x02030020);
    define("MSG_MPC_CS_MU_MIGRATE_BUCKET_ACK" , 0x02030021);
    define("MSG_MPC_CS_SU_MIGRATE_BUCKET" , 0x02030040);
    define("MSG_MPC_CS_SU_MIGRATE_BUCKET_ACK" , 0x02030041);
	
	//mu and su extention
    define("MSG_MPC_CS_MU_EXTENT_BUCKET" , 0x02030030);
    define("MSG_MPC_CS_MU_EXTENT_BUCKET_ACK" , 0x02030031);
    define("MSG_MPC_CS_SU_EXTENT_BUCKET" , 0x02030050);
    define("MSG_MPC_CS_SU_EXTENT_BUCKET_ACK" , 0x02030051);
	
	// get mu and su hash
    define("MSG_MPC_CS_GET_MU_RULE" , 0x02030060);
    define("MSG_MPC_CS_GET_MU_RULE_ACK" , 0x02030061);
    define("MSG_MPC_CS_GET_SU_RULE" , 0x02030080);
    define("MSG_MPC_CS_GET_SU_RULE_ACK" , 0x02030081);
	
	// init hash
    define("MSG_MPC_CS_MU_INIT_HASH" , 0x02030070);
    define("MSG_MPC_CS_MU_INIT_HASH_ACK" , 0x02030071);
    define("MSG_MPC_CS_SU_INIT_HASH" , 0x02030090);
    define("MSG_MPC_CS_SU_INIT_HASH_ACK" , 0x02030091);

    // get bucket
    define("MSG_MPC_CS_MU_BUCKET" , 0x02030100);
    define("MSG_MPC_CS_MU_BUCKET_ACK" , 0x02030101);
    define("MSG_MPC_CS_SU_BUCKET" , 0x02030110);
    define("MSG_MPC_CS_SU_BUCKET_ACK" , 0x02030111);

    define("MSG_MPC_CS_GLOBAL_BLOCK_INFO" , 0X2030130);
    define("MSG_MPC_CS_GLOBAL_BLOCK_INFO_ACK" , 0X2030131);
	
	//
	define("NO_MODULE", 0);
	define("RS_MODULE", 1);
	define("CA_MODULE", 2);
	define("MS_MODULE", 3);
	define("MU_MODULE", 4);
	define("SU_MODULE", 5);
	
	// create user dir
    define("MSG_MPC_MU_CREATE_USER" , 0x03000010);
    define("MSG_MPC_MU_CREATE_USER_ACK" , 0x03000011);

    // delete user dir
    define("MSG_MPC_MU_DELETE_USER" , 0x03000020);
    define("MSG_MPC_MU_DELETE_USER_ACK" , 0x03000021);
    
    // module status
    define("MSG_MPC_DS_MODULES_INFO" , 0x05000012);
    define("MSG_MPC_DS_MODULES_INFO_ACK" , 0x05000013);
    define("MSG_MPC_DS_START_MODULE" , 0x05000014);
    define("MSG_MPC_DS_START_MODULE_ACK" , 0x05000015);
    define("MSG_MPC_DS_STOP_MODULE" , 0x05000016);
    define("MSG_MPC_DS_STOP_MODULE_ACK" , 0x05000017);


    // error code , in error of header
    define("CS_OK" , 0x10000000);
    define("CS_ERROR" , 0x10000001);
    define("MPC_INPUT_ERROR" , 0x10000012);

    define("DS_OK" , 0x10000020);
    define("DS_ERROR" , 0x10000021);

    // inside error code , in para1 of header
    define("MPC_CS_MIGRATE_SUCCESS" , 0);
    define("MPC_CS_MIGRATE_FAILED" , 1);
    define("MPC_CS_EXTENT_SUCCESS" , 0);
    define("MPC_CS_EXTENT_FAILED" , 1);
    define("HASH_NOT_INITED" , 0);
    define("HASH_HAS_INITED" , 1);

    define("MAX_IP_NUM" , 5);
    define("INVALID_IP" , 0);

?>
