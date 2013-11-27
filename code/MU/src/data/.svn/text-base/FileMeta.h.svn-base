/*
 * @file FileMeta.h
 * @brief Structures of file metadata.
 *
 * @version 1.0
 * @date Tue Jul  3 09:48:03 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef FileMeta_H_
#define FileMeta_H_

#include <inttypes.h>

#include <string>
#include <list>

/**
 * @brief Attributes of file.
 */
struct FileAttr {
    uint64_t m_Version;
    uint32_t m_Mode;
    uint64_t m_CTime;
    uint64_t m_MTime;
    uint32_t m_Type;
    uint64_t m_Size;

    FileAttr() {
        m_Version = 0;
        m_Mode = 0;
        m_CTime = 0;
        m_MTime = 0;
        m_Type = 0;
        m_Size = 0;
    }
};

/**
 * @brief Metadata of file block.
 */
struct BlockMeta {
    std::string m_Checksum;
};

/**
 * @brief Metadata of file.
 */
struct FileMeta {
    FileAttr m_Attr;
    std::list<BlockMeta> m_BlockList;
};

/**
 * @brief Plain old directory entry.
 */
struct PDEntry {
    std::string m_Name;
    int m_Type;

    PDEntry() {
        m_Type = 0;
    }
};

/**
 * @brief Extended directory entry.
 */
struct EDEntry {
    int m_Type;
    std::string m_Name;
    uint32_t m_Mode;
    uint64_t m_CTime;
    uint64_t m_MTime;
    uint64_t m_Size;
    uint64_t m_Version;
    std::list<BlockMeta> m_BlockList;

    EDEntry() {
        m_Type = 0;
        m_Mode = 0;
        m_CTime = 0;
        m_MTime = 0;
        m_Size = 0;
        m_Version = 0;
    }
};

#endif  // FileMeta_H_

