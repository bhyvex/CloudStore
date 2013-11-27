/*
 * @file MUMacros.h
 * @brief MU错误码和通用宏定义
 *
 * @version 1.0
 * @date Wed Jun 20 10:39:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */  


// MU error codes

#define MU_OK                       0

#define MU_UNKNOWN_ERROR            1

#define MU_LOCATE_ERROR             2

#define MU_SERVICE_DENY             3

//#define INSUFFICIENT_SPACE          4

// there's some problems with the prefix of specified path
#define PATH_INVALID                10

#define PATH_EXIST                  11

#define PATH_NOT_EXIST              12

#define NOT_DIRECTORY               13

#define IS_DIRECTORY                14

#define DIRECTORY_NOT_EMPTY         15

#define SRC_PATH_INVALID            16

#define SRC_PATH_NOT_DIRECTORY      17

#define SRC_PATH_NOT_EXIST          18

#define DEST_PATH_INVALID           19

#define SRC_PATH_IS_DIRECTORY       20

#define DEST_PATH_IS_DIRECTORY      21

// file version outdated
#define VERSION_OUTDATED            22

// log sequence outdated
#define LOG_SEQ_OUTDATED            23

#define USER_EXIST                  30

#define MU_BUCKET_BUSY              40

#define MU_BUCKET_EXIST             41




// MU macros

// data any more?
#define MU_NO_MORE_DATA             0

#define MU_MORE_DATA                1

// file types
#define MU_DIRECTORY                0

#define MU_REGULAR_FILE             1

#define MU_SYMBOLINK                2

// bucket states
#define MU_BUCKET_START             0

#define MU_BUCKET_MASTER            1

#define MU_BUCKET_SLAVE             2

#define MU_BUCKET_STOP              3

// file operation codes in log record
#define MU_OP_PUT_DIR               0

#define MU_OP_DEL_DIR               1

#define MU_OP_MOV_DIR               2

#define MU_OP_PUT_FILE              3

#define MU_OP_DEL_FILE              4

#define MU_OP_MOV_FILE              5



