/*
 * @file FileOpItem.h
 * @brief Thread pool work item for file operations.
 *
 * @version 1.0
 * @date Tue Jul  3 09:24:40 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef FileOpItem_H_
#define FileOpItem_H_

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

#include "data/FileMeta.h"

#include <list>

class FileOpItem : public MUWorkItem
{
public:
    enum _FileOpItemWorkType {
        PUT_DIR,
        DEL_DIR,
        GET_DIR,
        STAT_DIR,
        GET_DIR2,
        MOV_DIR,
        PUT_FILE,
        DEL_FILE,
        GET_FILE,
        MOV_FILE
    };

public:
    virtual ~FileOpItem();

    explicit FileOpItem(MUTask *pTask);

    int preProcess();

    int process();

    void delay();

    inline uint32_t getRequestId();

    inline void setRequestId(uint32_t requestId);

    inline uint64_t getUserId();

    inline void setUserId(uint64_t userId);

    inline uint64_t getBucketId();

    inline void setBucketId(uint64_t bucketId);

    inline std::string getToken();

    inline void setToken(const std::string &token);

    inline void setPath(const std::string &path);

    inline std::string getPath();

    inline void setSrcPath(const std::string &srcPath);

    inline std::string getSrcPath();

    inline void setDestPath(const std::string &destPath);

    inline std::string getDestPath();

    inline void setFileMetaIn(const FileMeta &meta);

    inline const FileMeta &getFileMetaIn();

    inline const FileMeta &getFileMetaOut();

    inline const std::list<PDEntry> &getPDEntryList();

    inline const std::list<EDEntry> &getEDEntryList();

protected:
    /**
     * @brief User already exists?
     *
     * @return
     */
    ReturnStatus userExists();

    /**
     * @brief convert 'path' to absolute path using m_UserId and m_BucketId
     *
     * @param path user path
     *
     * @return absolute path on local filesystem
     */
    std::string absPath(const std::string &path);

    /**
     * @brief Get user root dir path.
     *
     * @return
     */
    std::string userRoot();

    void putDir();

    void delDir();

    void getDir();

    void statDir();

    void getDir2();

    void movDir();

    void putFile();

    void delFile();

    void getFile();

    void movFile();

private:
    // client request id
    uint32_t m_RequestId;

    // general
    uint64_t m_UserId;
    uint64_t m_BucketId;
    std::string m_Token;
    std::string m_Path;
    std::string m_SrcPath;
    std::string m_DestPath;

    // file
    FileMeta m_FileMetaIn;
    FileMeta m_FileMetaOut;

    // directory entry
    std::list<PDEntry> m_PDEntryList;
    std::list<EDEntry> m_EDEntryList;
};

uint32_t
FileOpItem::getRequestId()
{
    return m_RequestId;
}

void
FileOpItem::setRequestId(uint32_t requestId)
{
    m_RequestId = requestId;
}

uint64_t
FileOpItem::getUserId()
{
    return m_UserId;
}

void
FileOpItem::setUserId(uint64_t userId)
{
    m_UserId = userId;
}

uint64_t
FileOpItem::getBucketId()
{
    return m_BucketId;
}

void
FileOpItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

std::string
FileOpItem::getToken()
{
    return m_Token;
}

void
FileOpItem::setToken(const std::string &token)
{
    m_Token = token;
}

void
FileOpItem::setPath(const std::string &path)
{
    m_Path = path;
}

std::string
FileOpItem::getPath()
{
    return m_Path;
}

void
FileOpItem::setSrcPath(const std::string &srcPath)
{
    m_SrcPath = srcPath;
}

std::string
FileOpItem::getSrcPath()
{
    return m_SrcPath;
}

void
FileOpItem::setDestPath(const std::string &destPath)
{
    m_DestPath = destPath;
}

std::string
FileOpItem::getDestPath()
{
    return m_DestPath;
}

void
FileOpItem::setFileMetaIn(const FileMeta &meta)
{
    m_FileMetaIn = meta;
}

const FileMeta &
FileOpItem::getFileMetaIn()
{
    return m_FileMetaIn;
}

const FileMeta &
FileOpItem::getFileMetaOut()
{
    return m_FileMetaOut;
}

const std::list<PDEntry> &
FileOpItem::getPDEntryList()
{
    return m_PDEntryList;
}

const std::list<EDEntry> &
FileOpItem::getEDEntryList()
{
    return m_EDEntryList;
}

#endif  // FileOpItem_H_

