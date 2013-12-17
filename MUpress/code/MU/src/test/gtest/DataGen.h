/*
 * @file DataGen.h
 * @brief Generate sample data for test.
 *
 * @version 1.0
 * @date Sat Aug  4 16:36:35 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef MU_TEST_GTEST_DATAGEN_H_
#define MU_TEST_GTEST_DATAGEN_H_

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <string>

#include "util/util.h"

#include "data/UserInfo.h"

#define BUCKET_ID_STR(x) (util::conv::conv<std::string, uint64_t>(x))

#define BUCKET_NAME_PREFIX "bucket"

#define BUCKET_NAME(x) (BUCKET_NAME_PREFIX + BUCKET_ID_STR(x))

#define USER_ID_STR(x) (util::conv::conv<std::string, uint64_t>(x))

#define USER_NAME_PREFIX "user"

#define USER_NAME(x) (USER_NAME_PREFIX + USER_ID_STR(x))

#define PATH_SEPARATOR "/"

#define VALID_BUCKET_ID 0

#define VALID_USER_ID 10000

#define BUCKET_LOG_NAME "bucket_log"

#define USER_INFO_NAME ".user_info"

#define REGULAR_FILE_SIZE (2 * 512 * 1024 + 45)

#define REGULAR_FILE_BLOCKS 3

#define REGULAR_FILE_MODE (420)

#define REGULAR_FILE_VERSION (0)

#define REGULAR_FILE_CTIME (1343962423ULL)

#define REGULAR_FILE_MTIME (1343962423ULL)

#define REGULAR_FILE_BLOCK_1  \
    "1111111111111111111111111111111111111111"

#define REGULAR_FILE_BLOCK_2  \
    "2222222222222222222222222222222222222222"

#define REGULAR_FILE_BLOCK_3  \
    "3333333333333333333333333333333333333333"

#define USER_QUOTA_TOTAL (2 * 1024 * 1024 * 1024ULL)

#define USER_QUOTA_USED (39 * REGULAR_FILE_SIZE)

#define USER_LOG_SEQ_NR (0)

class DataGen
{
public:
    virtual ~DataGen();

    explicit DataGen(const std::string &dataRoot);

    int gen();

    int clean();

private:
    int readUserInfo(UserInfo *pInfo);

    int writeUserInfo(const UserInfo &info);

    int createUser();

    int createFile(const std::string &path);

    int createDir(const std::string &path);

private:
    std::string _dataRoot;
    std::string _bucketRoot;
    std::string _userRoot;
};


#endif  // MU_TEST_GTEST_DATAGEN_H_

