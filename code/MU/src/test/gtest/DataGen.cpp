/*
 * @file DataGen.cpp
 * @brief Generate sample data for test.
 *
 * @version 1.0
 * @date Sat Aug  4 17:38:37 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "DataGen.h"

#include <stdlib.h>

#include "log/log.h"
#include "data/FileMeta.h"
#include "frame/MUMacros.h"
#include "protocol/MUMacros.h"
#include "storage/ChannelManager.h"


DataGen::~DataGen()
{

}

DataGen::DataGen(const std::string &dataRoot)
{
    _dataRoot = dataRoot;
    _bucketRoot = _dataRoot + PATH_SEPARATOR + BUCKET_NAME(VALID_BUCKET_ID);
    _userRoot = _bucketRoot + PATH_SEPARATOR + USER_NAME(VALID_USER_ID);
}

int
DataGen::clean()
{
    int rt = 0;

    std::string cmd = "rm -rf " + _userRoot;

    rt = ::system(cmd.c_str());

    if (-1 == rt) {
        ERROR_LOG("clean data failed");
        return -1;
    }

    return 0;
}

int
DataGen::gen()
{
    int rt = 0;

    rt = createUser();

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/empty_dir_00");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/empty_dir_01");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/empty_dir_02");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/existed_file_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/existed_file_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/existed_file_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/not_empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_10/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_10/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_10/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/not_empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_11/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_11/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_11/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_00/not_empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_12/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_12/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_00/not_empty_dir_12/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/existed_file_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/existed_file_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/existed_file_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/not_empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_10/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_10/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_10/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/not_empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_11/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_11/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_11/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_01/not_empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_12/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_12/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_01/not_empty_dir_12/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/existed_file_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/existed_file_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/existed_file_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/not_empty_dir_10");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_10/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_10/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_10/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/not_empty_dir_11");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_11/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_11/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_11/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createDir("/not_empty_dir_02/not_empty_dir_12");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_12/existed_file_20");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_12/existed_file_21");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/not_empty_dir_02/not_empty_dir_12/existed_file_22");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/existed_file_00");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/existed_file_01");

    if (-1 == rt) {
        return -1;
    }

    rt = createFile("/existed_file_02");

    if (-1 == rt) {
        return -1;
    }

    return 0;
}

int
DataGen::readUserInfo(UserInfo *pInfo)
{
    int rt = 0;

    std::string path = _userRoot + PATH_SEPARATOR_STRING + USER_INFO_NAME;

	
    rt = ::open(path.c_str(), O_RDONLY);

    if (-1 == rt) {
        ERROR_LOG("path %s, open() error, %s",
                  path.c_str(), strerror(errno));
        return -1;
    }

    int fd = rt;

    rt = ::read(fd, pInfo, sizeof(UserInfo));

    if (sizeof(UserInfo) != rt) {
        if (-1 == rt) {
            ERROR_LOG("path %s, read() error, %s", path.c_str(), strerror(errno));

        } else {
            ERROR_LOG("path %s, read user info, expected %d bytes, "
                      "but read %d bytes", path.c_str(), sizeof(UserInfo), rt);
        }

        ::close(fd);
        return -1;
    }

    ::close(fd);

    return 0;
}

int
DataGen::writeUserInfo(const UserInfo &info)
{
    int rt = 0;

    std::string path = _userRoot + PATH_SEPARATOR_STRING + USER_INFO_NAME;

    rt = ::open(path.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        ERROR_LOG("path %s, open() error, %s",
                  path.c_str(), strerror(errno));
        return -1;
    }

    int fd = rt;

    rt = ::write(fd, &info, sizeof(UserInfo));

    if (sizeof(UserInfo) != rt) {
        if (-1 == rt) {
            ERROR_LOG("path %s, write() error, %s",
                      path.c_str(), strerror(errno));

        } else {
            ERROR_LOG("path %s, write user info, expected %d bytes, "
                      "but write %d bytes", path.c_str(), sizeof(UserInfo), rt);
        }

        ::close(fd);
        return -1;
    }

    ::close(fd);

    return 0;
}


int
DataGen::createUser()
{
    int rt = 0;

    rt = ::mkdir(_userRoot.c_str(), S_IRWXU);

    if (-1 == rt) {
        ERROR_LOG("path %s, mkdir() error, %s",
                  _userRoot.c_str(), strerror(errno));
        return -1;
    }

    UserInfo info;
    info.m_TotalQuota = USER_QUOTA_TOTAL;
    info.m_UsedQuota = USER_QUOTA_USED;

    rt = writeUserInfo(info);

    if (-1 == rt) {
        ERROR_LOG("write user info failed");
        return -1;
    }

    return 0;
}

int
DataGen::createDir(const std::string &path)
{
    int rt = 0;

    std::string rpath = _userRoot + path;

    rt = ::mkdir(rpath.c_str(), S_IRWXU);

    if (-1 == rt) {
        ERROR_LOG("path %s, mkdir() error, %s",
                  rpath.c_str(), strerror(errno));
        return -1;
    }

    return 0;
}

int
DataGen::createFile(const std::string &path)
{
    int rt = 0;

    std::string rpath = _userRoot + path;

    rt = ::open(rpath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        ERROR_LOG("path %s, open() error, %s",
                  rpath.c_str(), strerror(errno));
        return -1;
    }

    int fd = rt;

    FileAttr attr;
    attr.m_Version = REGULAR_FILE_VERSION;
    attr.m_Mode = REGULAR_FILE_MODE;
    attr.m_Size = REGULAR_FILE_SIZE;
    attr.m_Type = MU_REGULAR_FILE;
    attr.m_CTime = REGULAR_FILE_CTIME;
    attr.m_MTime = REGULAR_FILE_MTIME;

    rt = ::write(fd, &attr, sizeof(FileAttr));

    if (-1 == rt) {
        ERROR_LOG("path %s, write() error, %s",
                  rpath.c_str(), strerror(errno));
        ::close(fd);
        return -1;
    }

    char key[FIXED_BLOCK_CHECKSUM_LEN];

    memcpy(key, REGULAR_FILE_BLOCK_1, 40);

    rt = ::write(fd, key, 40);

    if (-1 == rt) {
        ERROR_LOG("path %s, write() error, %s",
                  rpath.c_str(), strerror(errno));
        ::close(fd);
        return -1;
    }

    memcpy(key, REGULAR_FILE_BLOCK_2, 40);

    rt = ::write(fd, key, 40);

    if (-1 == rt) {
        ERROR_LOG("path %s, write() error, %s",
                  rpath.c_str(), strerror(errno));
        ::close(fd);
        return -1;
    }

    memcpy(key, REGULAR_FILE_BLOCK_3, 40);

    rt = ::write(fd, key, 40);

    if (-1 == rt) {
        ERROR_LOG("path %s, write() error, %s",
                  rpath.c_str(), strerror(errno));
        ::close(fd);
        return -1;
    }

    ::close(fd);

    return 0;
}




