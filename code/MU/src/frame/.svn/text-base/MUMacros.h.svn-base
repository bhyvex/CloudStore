/*
 * @file MUMacros.h
 * @brief MU错误码和通用宏定义
 *
 * @version 1.0
 * @date Wed Jun 20 10:39:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef _Internal_MUMacros_H_
#define _Internal_MUMacros_H_

// MU internal

#define MU_SUCCESS                  0

#define MU_FAILED                   -1

#define ROOT_PATH                   "/"

#define PATH_SEPARATOR_CHAR         '/'
#define PATH_SEPARATOR_STRING       "/"

#define DENTRY_CURRENT_DIR          "."
#define DENTRY_PARENT_DIR           ".."
#define HIDDEN_FILE_PREFIX          "."

#define FILE_VERSION_INIT           0

#define FIXED_BLOCK_SIZE            (512 * 1024)
#define FIXED_BLOCK_CHECKSUM_LEN    40

#define BUCKET_NAME_PREFIX          "bucket"
#define USER_NAME_PREFIX            "user"

#define USER_INFO_FILE_NAME         ".user_info"
#define BUCKET_LOG_FILE_NAME        "bucket_log"

#define BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX   "migration.tmp."
#define BUCKET_SNAPSHOT_FILE_NAME_PREFIX        "snapshot."

#define LOG_ROTATION_STRATEGY_TIME  "time"
#define LOG_ROTATION_STRATEGY_SIZE  "size"
#define LOG_ROTATION_CHECK_PERIOD   30

#define REQUEST_QUEUE_CHECK_PERIOD  2

#define MIGRATION_DATA_PIECE_SIZE   (1024 * 1024)

#define TASK_RECYCLE_PERIOD         2

#define USER_IDLE_CHECK_PERIOD      60
#define USER_IDLE_TIME_OUT          30

// total log records a user can read once time
#define MAX_LOG_RECORDS_ONCE        100

// least number of archived log files
#define LEAST_ARCHIVED_LOG_FILES    1

// time to flush log to lock disk
#define LOG_FLUSH_PERIOD            1

// internal log op types
// user opertion codes in log record
#define MU_OP_PUT_USER              100

#define MU_OP_DEL_USER              101

#define MU_OP_ROTATE_LOG            102



// MU error codes

//#define MU_OK                       0

//#define MU_UNKNOWN_ERROR            1

//#define MU_LOCATE_ERROR             2

//#define MU_SERVICE_DENY             3

//#define INSUFFICIENT_SPACE          4

// there's some problems with the prefix of specified path
//#define PATH_INVALID                10

//#define PATH_EXIST                  11

//#define PATH_NOT_EXIST              12

//#define NOT_DIRECTORY               13

//#define IS_DIRECTORY                14

//#define DIRECTORY_NOT_EMPTY         15

//#define SRC_PATH_INVALID            16

//#define SRC_PATH_NOT_DIRECTORY      17

//#define SRC_PATH_NOT_EXIST          18

//#define DEST_PATH_INVALID           19

//#define SRC_PATH_IS_DIRECTORY       20

//#define DEST_PATH_IS_DIRECTORY      21

// file version outdated
//#define VERSION_OUTDATED            22

// log sequence outdated
//#define LOG_SEQ_OUTDATED            23

//#define USER_EXIST                  30

//#define MU_BUCKET_BUSY              40

//#define MU_BUCKET_EXIST             41




// MU macros

// data any more?
//#define MU_NO_MORE_DATA             0

//#define MU_MORE_DATA                1

// file types
//#define MU_DIRECTORY                0

//#define MU_REGULAR_FILE             1

//#define MU_SYMBOLINK                2

// bucket states
//#define MU_BUCKET_START             0

//#define MU_BUCKET_MASTER            1

//#define MU_BUCKET_SLAVE             2

//#define MU_BUCKET_STOP              3

// file operation codes in log record
//#define MU_OP_PUT_DIR               0

//#define MU_OP_DEL_DIR               1

//#define MU_OP_MOV_DIR               2

//#define MU_OP_PUT_FILE              3

//#define MU_OP_DEL_FILE              4

//#define MU_OP_MOV_FILE              5



#endif  // _Internal_MUMacros_H_


