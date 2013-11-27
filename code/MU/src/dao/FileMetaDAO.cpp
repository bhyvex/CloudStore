/*
 * @file FileMetaDAO.cpp
 * @brief File metadata access object.
 *
 * @version 1.0
 * @date Wed Jul  4 14:16:48 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "FileMetaDAO.h"

#include "frame/ReturnStatus.h"
#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "data/FileMeta.h"

#include "log/log.h"
#include "util/util.h"

#include <errno.h>
#include <assert.h>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>

FileMetaDAO::~FileMetaDAO()
{

}

FileMetaDAO::FileMetaDAO()
{

}

ReturnStatus
FileMetaDAO::putDir(const std::string &path)
{
    int rt = 0;

    rt = ::mkdir(path.c_str(),
                 S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if (0 == rt) {
        return ReturnStatus(MU_SUCCESS);
    }

    // error occurred

    int error = errno;

    DEBUG_LOG("path %s, mkdir() error, %s.", path.c_str(), strerror(error));

    if (EEXIST == error) {
        DEBUG_LOG("path %s, path exist", path.c_str());
        return ReturnStatus(MU_FAILED, PATH_EXIST);

    } else if (ENOENT == error || ENOTDIR == error) {
        DEBUG_LOG("path %s, path invalid", path.c_str());
        return ReturnStatus(MU_FAILED, PATH_INVALID);

    } else {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);  // make compiler happy
}

ReturnStatus
FileMetaDAO::delDir(const std::string &path, uint64_t *pDelta)
{
    assert(pDelta);

    ReturnStatus rs;

    uint64_t delta = 0;
    rs = rmdirRecursive(path, pDelta);

    if (!rs.success()) {
        DEBUG_LOG("path %s, rmdirRecursive() error", path.c_str());
    }

    return rs;
}

std::string
FileMetaDAO::prefix(const std::string &path)
{
    size_t pos = path.rfind(PATH_SEPARATOR_CHAR);

    return path.substr(0, pos);
}

ReturnStatus
FileMetaDAO::checkPrefix(const std::string &path)
{
    int rt = 0;

    struct stat st;
    rt = ::stat(prefix(path).c_str(), &st);

    if (0 == rt) {

        if (S_ISDIR(st.st_mode)) {
            // prefix path is existed and refers to a directory
            return ReturnStatus(MU_FAILED, PATH_NOT_EXIST);

        } else {
            return ReturnStatus(MU_FAILED, PATH_INVALID);
        }

    } else {
        if (errno == ENOENT || errno == ENOTDIR) {
            return ReturnStatus(MU_FAILED, PATH_INVALID);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

}

ReturnStatus
FileMetaDAO::rmdirRecursive(const std::string &path, uint64_t *pDelta)
{
    int rt = 0;
    ReturnStatus rs;
    int error = 0;
    std::string entryName;
    std::string npath;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        error = errno;

        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            DEBUG_LOG("path %s, opendir() error, no directory");
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    // delete its children

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR
            || entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        npath = path + PATH_SEPARATOR_STRING + entryName;

        if (DT_DIR == pEnt->d_type) {
            // directory, call myself to delete it
            rs = rmdirRecursive(npath, pDelta);

            if (!rs.success()) {
                DEBUG_LOG("path %s, rmdirRecursive() error", npath.c_str());
                return rs;
            }

        } else {
            // add file size to changed user quota
            addFileSizeToDelta(npath, pDelta);

            // delete it directly
            rt = ::unlink(npath.c_str());

            if (-1 == rt) {
                DEBUG_LOG("path %s, unlink() error, %s.",
                          npath.c_str(), strerror(errno));
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }
        }
    }

    ::closedir(pDir);

    // delete path

    // do not delete user root
    if (path.substr(path.length() - 1) == ROOT_PATH) {
        return ReturnStatus(MU_SUCCESS);
    }

    rt = ::rmdir(path.c_str());

    if (-1 == rt) {
        DEBUG_LOG("path %s, rmdir() error, %s.",
                  path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

void
FileMetaDAO::addFileSizeToDelta(const std::string &path, uint64_t *pDelta)
{
    int rt = 0;

    // read file size
    rt = ::open(path.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s", path.c_str(), strerror(errno));
        return ;
    }

    int fd = rt;

    FileAttr attr;

    rt = util::io::readn(fd, &attr, sizeof(attr));
    ::close(fd);

    if (sizeof(attr) != rt) {
        DEBUG_LOG("path %s, readn() error", path.c_str());
        return ;
    }

    *pDelta += attr.m_Size;
}

ReturnStatus
FileMetaDAO::getDir(const std::string &path, std::list<PDEntry> *pEntryList)
{
    assert(pEntryList);

    int rt = 0;
    int error = 0;
    std::string entryName;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        error = errno;

        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR
            || entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        PDEntry ent;
        ent.m_Name = entryName;

        if (DT_DIR == pEnt->d_type) {
            ent.m_Type = MU_DIRECTORY;

        } else {
            ent.m_Type = MU_REGULAR_FILE;
        }

        pEntryList->push_back(ent);
    }

    ::closedir(pDir);

    return ReturnStatus(MU_SUCCESS);
}


ReturnStatus
FileMetaDAO::statDir(const std::string &path, FileMeta *pMeta)
{
    assert(pMeta);

    int rt = 0;
    int error = 0;

    struct stat st;
    rt = ::stat(path.c_str(), &st);

    if (-1 == rt) {
        error = errno;
        DEBUG_LOG("path %s, stat() error, %s.", path.c_str(), strerror(error));

        if (ENOENT == error || ENOTDIR == error) {
            return checkPrefix(path);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    if (!S_ISDIR(st.st_mode)) {
        DEBUG_LOG("path %s, not directory", path.c_str());
        return ReturnStatus(MU_FAILED, NOT_DIRECTORY);
    }

    pMeta->m_Attr.m_Mode = st.st_mode;
    pMeta->m_Attr.m_CTime = st.st_ctime;
    pMeta->m_Attr.m_MTime = st.st_mtime;
    pMeta->m_Attr.m_Size = st.st_size;

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::getDir2(const std::string &path,
                     std::list<EDEntry> *pEntryList)
{
    assert(pEntryList);

    int rt = 0;
    int error = 0;
    std::string entryName;
    std::string npath;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        error = errno;

        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    struct dirent *pEnt = NULL;

    FileMeta meta;

    ReturnStatus rs;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;
        npath = path + PATH_SEPARATOR_STRING + entryName;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR
            || entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        EDEntry ent;
        ent.m_Name = entryName;

        if (DT_DIR == pEnt->d_type) {
            ent.m_Type = MU_DIRECTORY;

            rs = statDir(npath, &meta);

            if (!rs.success()) {
                DEBUG_LOG("path %s, statDir() error", npath.c_str());
                return rs;
            }

            ent.m_Mode = meta.m_Attr.m_Mode;
            ent.m_CTime = meta.m_Attr.m_CTime;
            ent.m_MTime = meta.m_Attr.m_MTime;
            ent.m_Size = meta.m_Attr.m_Size;

        } else {
            // clear data of last entry
            meta.m_BlockList.clear();

            rs = getFile(npath, &meta);

            if (!rs.success()) {
                DEBUG_LOG("path %s, getFile() error", npath.c_str());
                return rs;
            }

            ent.m_Mode = meta.m_Attr.m_Mode;
            ent.m_CTime = meta.m_Attr.m_CTime;
            ent.m_MTime = meta.m_Attr.m_MTime;
            ent.m_Size = meta.m_Attr.m_Size;
            ent.m_Version = meta.m_Attr.m_Version;
            ent.m_Type = meta.m_Attr.m_Type;

            ent.m_BlockList = meta.m_BlockList;
        }

        pEntryList->push_back(ent);
    }

    ::closedir(pDir);

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::movDir(const std::string &srcPath,
                    const std::string &destPath)
{
    int rt = 0;
    int error = 0;

    ReturnStatus rs;

    // check src path

    rs = isdir(srcPath);

    if (!rs.success()) {
        if (NOT_DIRECTORY == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_NOT_DIRECTORY);

        } else if (PATH_INVALID == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_INVALID);

        } else if (PATH_NOT_EXIST == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_NOT_EXIST);
        }

        return rs;
    }

    // check dest path

    rs = isdir(prefix(destPath));

    if (!rs.success()) {
        if (MU_UNKNOWN_ERROR != rs.errorCode) {
            return ReturnStatus(MU_FAILED, DEST_PATH_INVALID);
        }

        return rs;
    }

    rt = ::rename(srcPath.c_str(), destPath.c_str());

    if (-1 == rt) {
        error = errno;
        DEBUG_LOG("src path %s, dest path %s, rename() error, %s.",
                  srcPath.c_str(), destPath.c_str(), strerror(error));

        if (ENOTDIR == error) {
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOTEMPTY == error || EEXIST == error) {
            return ReturnStatus(MU_FAILED, DIRECTORY_NOT_EMPTY);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::isdir(const std::string &path)
{
    int rt = 0;

    struct stat st;
    rt = ::stat(path.c_str(), &st);

    if (0 == rt) {
        if (S_ISDIR(st.st_mode)) {
            return ReturnStatus(MU_SUCCESS);

        } else {
            DEBUG_LOG("path %s, not directory", path.c_str());
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);
        }
    }

    int error = errno;

    DEBUG_LOG("path %s, stat() error, %s", path.c_str(), strerror(error));

    if (ENOENT == error || ENOTDIR == error) {
        return checkPrefix(path);

    } else {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }
}


ReturnStatus
FileMetaDAO::putFile(const std::string &path, const FileMeta &meta,
                     FileMeta *pMeta, int *pDelta)
{
    assert(pMeta);
    assert(pDelta);

    if (FILE_VERSION_INIT == meta.m_Attr.m_Version) {
        return createFile(path, meta, pMeta, pDelta);

    } else {
        return updateFile(path, meta, pMeta, pDelta);
    }
}



ReturnStatus
FileMetaDAO::createFile(
    const std::string &path,
    const FileMeta &meta,
    FileMeta *pMeta,
    int *pDelta)
{
    int rt = 0;
    int fd = 0;
    int error = 0;

    rt = ::open(path.c_str(), O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        error = errno;
        DEBUG_LOG("path %s, open() error, %s.", path.c_str(), strerror(error));

        if (EEXIST == error) {
            DEBUG_LOG("path %s, file exist", path.c_str());

            // is a regular file?
            ReturnStatus rs = isfile(path);

            if (!rs.success()) {
                return rs;
            }

            rt = ::open(path.c_str(), O_RDONLY);

            if (-1 == rt) {
                DEBUG_LOG("path %s, open() error, %s",
                          path.c_str(), strerror(errno));
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }

            fd = rt;

            // check file version

            FileAttr attr;

            rt = util::io::readn(fd, &attr, sizeof(attr));

            ::close(fd);

            if (sizeof(attr) != rt) {
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }

            pMeta->m_Attr.m_Version = attr.m_Version;

            return ReturnStatus(MU_FAILED, PATH_EXIST);

        } else if (ENOENT == error || ENOTDIR == error) {
            DEBUG_LOG("path %s, path invalid", path.c_str());
            return ReturnStatus(MU_FAILED, PATH_INVALID);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    fd = rt;

    ReturnStatus rs = writeFileMeta(fd, meta);

    ::close(fd);

    // return file size delta
    *pDelta = meta.m_Attr.m_Size;

    return rs;
}

ReturnStatus
FileMetaDAO::updateFile(const std::string &path, const FileMeta &meta,
                        FileMeta *pMeta, int *pDelta)
{
    int rt = 0;
    int fd = 0;

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    rt = ::open(path.c_str(), O_RDWR);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s.", path.c_str(), strerror(errno));

        // already check path errors in isfile()
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    fd = rt;

    // check file version

    FileAttr attr;

    rt = util::io::readn(fd, &attr, sizeof(attr));

    if (sizeof(attr) != rt) {
        DEBUG_LOG("readn() error");
        ::close(fd);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (meta.m_Attr.m_Version != attr.m_Version + 1) {
        DEBUG_LOG("version outdated, current version %" PRIu64 ", "
                  "received version %" PRIu64,
                  attr.m_Version, meta.m_Attr.m_Version);
        ::close(fd);
        pMeta->m_Attr = attr;
        return ReturnStatus(MU_FAILED, VERSION_OUTDATED);
    }

    // write metadata

    rs = writeFileMeta(fd, meta);

    ::close(fd);

    // return file size delta
    *pDelta = meta.m_Attr.m_Size - attr.m_Size;

    return rs;
}


ReturnStatus
FileMetaDAO::writeFileMeta(int fd, const FileMeta &meta)
{
    int rt = 0;

    rt = ::lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        DEBUG_LOG("lseek() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int blocks = meta.m_BlockList.size();

    int attrLen = sizeof(FileAttr);

    char *pBuf = new char[
        attrLen + FIXED_BLOCK_CHECKSUM_LEN * blocks];
    int bufIdx = 0;

    memcpy(pBuf + bufIdx, &(meta.m_Attr), attrLen);
    bufIdx += attrLen;

    for (std::list<BlockMeta>::const_iterator it = meta.m_BlockList.begin();
         it != meta.m_BlockList.end(); ++it) {
        memcpy(pBuf + bufIdx,
               it->m_Checksum.c_str(),
               FIXED_BLOCK_CHECKSUM_LEN);
        bufIdx += FIXED_BLOCK_CHECKSUM_LEN;
    }

    rt = util::io::writen(fd, pBuf, bufIdx);

    delete [] pBuf;
    pBuf = NULL;

    if (bufIdx != rt) {
        DEBUG_LOG("writen() error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //int iovcnt = blocks + 1;
    //struct iovec iov[iovcnt];
    //int iovIdx = 0;

    //iov[iovIdx].iov_base = const_cast<FileAttr *>(&(meta.m_Attr));
    //iov[iovIdx].iov_len = sizeof(meta.m_Attr);
    //++iovIdx;

    //for (std::list<BlockMeta>::const_iterator it = meta.m_BlockList.begin();
    //it != meta.m_BlockList.end(); ++it) {
    //iov[iovIdx].iov_base = const_cast<char *>(it->m_Checksum.c_str());
    //iov[iovIdx].iov_len = FIXED_BLOCK_CHECKSUM_LEN;
    //++iovIdx;
    //}

    //rt = ::writev(fd, iov, iovcnt);

    //if (sizeof(meta.m_Attr) + blocks * FIXED_BLOCK_CHECKSUM_LEN != rt) {
    //DEBUG_LOG("writev() error, %s.", strerror(errno));
    //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    //}

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::isfile(const std::string &path)
{
    int rt = 0;

    struct stat st;
    rt = ::stat(path.c_str(), &st);

    if (0 == rt) {
        if (S_ISREG(st.st_mode)) {
            return ReturnStatus(MU_SUCCESS);

        } else if (S_ISDIR(st.st_mode)) {
            DEBUG_LOG("path %s, is directory", path.c_str());
            return ReturnStatus(MU_FAILED, IS_DIRECTORY);

        } else {
            DEBUG_LOG("path %s, unexpected file type", path.c_str());
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    int error = errno;

    DEBUG_LOG("path %s, stat() error, %s", path.c_str(), strerror(error));

    if (ENOENT == error || ENOTDIR == error) {
        return checkPrefix(path);

    } else {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }
}

ReturnStatus
FileMetaDAO::delFile(const std::string &path, int *pDelta)
{
    int rt = 0;

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    // read file size
    rt = ::open(path.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s", path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int fd = rt;

    FileAttr attr;

    rt = util::io::readn(fd, &attr, sizeof(attr));
    ::close(fd);

    if (sizeof(attr) != rt) {
        DEBUG_LOG("path %s, readn() error", path.c_str());
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    *pDelta = attr.m_Size;

    // do delete action

    rt = ::unlink(path.c_str());

    if (-1 == rt) {
        DEBUG_LOG("path %s, unlink() error, %s.",
                  path.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::getFile(const std::string &path, FileMeta *pMeta)
{
    assert(pMeta);

    // clear possible data
    pMeta->m_BlockList.clear();

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    int rt = 0;

    rt = ::open(path.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s.", path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int fd = rt;

    rs = readFileMeta(fd, pMeta);

    ::close(fd);

    if (!rs.success()) {
        DEBUG_LOG("path %s, readFileMeta() error", path.c_str());
    }

    return rs;
}


ReturnStatus
FileMetaDAO::readFileMeta(int fd, FileMeta *pMeta)
{
    assert(pMeta);

    int rt = 0;

    rt = util::io::readn(fd, &(pMeta->m_Attr), sizeof(pMeta->m_Attr));

    if (sizeof(pMeta->m_Attr) != rt) {
        DEBUG_LOG("read attr, readn() error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int blocks = pMeta->m_Attr.m_Size / FIXED_BLOCK_SIZE;

    if (pMeta->m_Attr.m_Size % FIXED_BLOCK_SIZE != 0) {
        ++blocks;
    }

    char *pBlockList = new char[blocks * FIXED_BLOCK_CHECKSUM_LEN];

    rt = util::io::readn(fd, pBlockList, blocks * FIXED_BLOCK_CHECKSUM_LEN);

    if (blocks * FIXED_BLOCK_CHECKSUM_LEN != rt) {
        DEBUG_LOG("read block list, readn() error");
        delete [] pBlockList;
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    BlockMeta bmeta;

    for (int i = 0; i < blocks; ++i) {
        bmeta.m_Checksum =
            std::string(pBlockList + i * FIXED_BLOCK_CHECKSUM_LEN,
                        FIXED_BLOCK_CHECKSUM_LEN);
        pMeta->m_BlockList.push_back(bmeta);
    }

    delete [] pBlockList;

    return ReturnStatus(MU_SUCCESS);
}


ReturnStatus
FileMetaDAO::movFile(const std::string &srcPath,
                     const std::string &destPath)
{
    int rt = 0;
    int error = 0;

    ReturnStatus rs;

    // check src path

    rs = isfile(srcPath);

    if (!rs.success()) {
        if (IS_DIRECTORY == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_IS_DIRECTORY);

        } else if (PATH_INVALID == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_INVALID);

        } else if (PATH_NOT_EXIST == rs.errorCode) {
            return ReturnStatus(MU_FAILED, SRC_PATH_NOT_EXIST);
        }

        return rs;
    }

    // check dest path

    rs = isdir(prefix(destPath));

    if (!rs.success()) {
        if (MU_UNKNOWN_ERROR != rs.errorCode) {
            return ReturnStatus(MU_FAILED, DEST_PATH_INVALID);
        }

        return rs;
    }

    rt = ::rename(srcPath.c_str(), destPath.c_str());

    if (-1 == rt) {
        error = errno;
        DEBUG_LOG("src path %s, dest path %s, rename() error, %s.",
                  srcPath.c_str(), destPath.c_str(), strerror(error));

        if (EISDIR == error) {
            return ReturnStatus(MU_FAILED, DEST_PATH_IS_DIRECTORY);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    return ReturnStatus(MU_SUCCESS);
}


