/*
 * @file WriteFileMeta.cpp
 * @brief write a valid metadata file
 * 
 * @version 1.0
 * @date Tue Aug 14 19:55:41 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include <string>
#include <list>

#include "protocol/MUMacros.h"
#include "frame/MUMacros.h"
#include "data/FileMeta.h"
#include "util/util.h"
#include "log/log.h"

#include "test/gtest/DataGen.h"

int
writeFileMeta(const std::string &path)
{
    int rt = 0;

    std::string rpath = path;

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

int 
main(int argc, char *argv[])
{
    if (2 != argc) {
        printf("usage: %s PATH\n", argv[0]);
        exit(1);
    }

    FileMeta meta;

    int rt = 0;

    rt = writeFileMeta(argv[1]);
    if (-1 == rt) {
        ERROR_LOG("write file meta failed");
        exit(1);
    }
    
    return 0;
}


