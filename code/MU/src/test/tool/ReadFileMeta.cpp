/*
 * @file ReadFileMeta.cpp
 * @brief tool to read file metadata
 *
 * @version 1.0
 * @date Tue Aug 14 09:59:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include <string>
#include <list>

#include "frame/MUMacros.h"
#include "data/FileMeta.h"
#include "util/util.h"
#include "log/log.h"


int 
readFileMeta(const std::string &path, FileMeta *pMeta)
{
    int rt = 0;

    rt = ::open(path.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s", path.c_str(), strerror(errno));
        return -1;
    }

    int fd = rt;

    rt = util::io::readn(fd, &(pMeta->m_Attr), sizeof(pMeta->m_Attr));

    if (sizeof(pMeta->m_Attr) != rt) {
        DEBUG_LOG("read attr, readn() error");
        ::close(fd);
        return -1;
    }

    int blocks = pMeta->m_Attr.m_Size / FIXED_BLOCK_SIZE;

    if (pMeta->m_Attr.m_Size % FIXED_BLOCK_SIZE != 0) {
        ++blocks;
    }

    printf("total %" PRIi32 " blocks\n", blocks);

    char *pBlockList = new char[blocks * FIXED_BLOCK_CHECKSUM_LEN];

    rt = util::io::readn(fd, pBlockList, blocks * FIXED_BLOCK_CHECKSUM_LEN);

    if (blocks * FIXED_BLOCK_CHECKSUM_LEN != rt) {
        DEBUG_LOG("read block list, readn() error");
        delete [] pBlockList;
        ::close(fd);
        return -1;
    }

    BlockMeta bmeta;

    for (int i = 0; i < blocks; ++i) {
        bmeta.m_Checksum =
            std::string(pBlockList + i * FIXED_BLOCK_CHECKSUM_LEN,
                        FIXED_BLOCK_CHECKSUM_LEN);
        pMeta->m_BlockList.push_back(bmeta);
    }

    delete [] pBlockList;

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

    rt = readFileMeta(argv[1], &meta);
    if (-1 == rt) {
        DEBUG_LOG("read file meta failed");
        exit(1);
    }
    
    printf("version: %" PRIu64 "\n", meta.m_Attr.m_Version);
    printf("type: %" PRIu32 "\n", meta.m_Attr.m_Type);
    printf("mode: %" PRIu32 "\n", meta.m_Attr.m_Mode);
    printf("ctime: %" PRIu64 "\n", meta.m_Attr.m_CTime);
    printf("mtime: %" PRIu64 "\n", meta.m_Attr.m_MTime);
    printf("size: %" PRIu64 "\n", meta.m_Attr.m_Size);

    for (std::list<BlockMeta>::iterator it = meta.m_BlockList.begin();
            it != meta.m_BlockList.end(); ++it) {
        printf("%s\n", it->m_Checksum.c_str());
    }

    return 0;
}



