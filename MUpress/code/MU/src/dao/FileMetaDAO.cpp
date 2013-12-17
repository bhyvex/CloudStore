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
#include "storage/ChannelManager.h"
#include "storage/Channel.h"
#include "storage/NameSpace.h"
#include "storage/FSNameSpace.h"

#include "protocol/MUMacros.h"



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

bool FileMetaDAO::setBucketID(uint64_t Id)
{
	m_BucketId = Id;
	return true;
}
bool FileMetaDAO::setUserID(uint64_t Id)
{
	m_UserId = Id;
	return true;
}


ReturnStatus
FileMetaDAO::putDir(const std::string &path)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;

    rt = DataNS->MkDir(path.c_str(),
                 S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if (0 == rt) {
        return ReturnStatus(MU_SUCCESS);
    }

    // error occurred

    int error = errno;

    ERROR_LOG("path %s, mkdir() error, %s.", path.c_str(), strerror(error));

    if (EEXIST == error) {
        ERROR_LOG("path %s, path exist", path.c_str());
        return ReturnStatus(MU_FAILED, PATH_EXIST);

    } else if (ENOENT == error || ENOTDIR == error) {
        ERROR_LOG("path %s, path invalid", path.c_str());
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
        ERROR_LOG("path %s, rmdirRecursive() error", path.c_str());
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
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;

	FileAttr st;
    rt = DataNS->Stat(prefix(path).c_str(), &st);

    if (0 == rt) {

        if (st.m_Type == MU_DIRECTORY) {//如果path是个存在的目录
            // prefix path is existed and refers to a directory
            return ReturnStatus(MU_FAILED, PATH_NOT_EXIST);

        } else {//如果path是个存在的文件
            return ReturnStatus(MU_FAILED, PATH_INVALID);
        }

    } else {
        if (errno == ENOENT || errno == ENOTDIR) {//如果这个路径不存在
            return ReturnStatus(MU_FAILED, PATH_INVALID);

        } else {//
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

}

ReturnStatus
FileMetaDAO::rmdirRecursive(const std::string &path, uint64_t *pDelta)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;
    ReturnStatus rs;
    int error = 0;
    std::string entryName;
    std::string npath;
    Args st;

	rt = DataNS->OpenDir(path.c_str(), &st);
    if (rt < 0) {
        error = errno;

        ERROR_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            ERROR_LOG("path %s, opendir() error, no directory");
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    // delete its children

    Dirent dirent;

    while(DataNS->ReadDirNext(&st, &dirent)){
        entryName = dirent.filename;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR
            || entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        npath = path + PATH_SEPARATOR_STRING + entryName;

        if (MU_DIRECTORY == dirent.filetype) {
            // directory, call myself to delete it
            rs = rmdirRecursive(npath, pDelta);

            if (!rs.success()) {
                ERROR_LOG("path %s, rmdirRecursive() error", npath.c_str());
                return rs;
            }

        } else {
            // add file size to changed user quota
            addFileSizeToDelta(npath, pDelta);

            // delete it directly
            rt = DataNS->Unlink(npath.c_str());

            if (-1 == rt) {
                ERROR_LOG("path %s, unlink() error, %s.",
                          npath.c_str(), strerror(errno));
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }
        }
    }


    // delete path

    // do not delete user root
    if (path.substr(path.length() - 1) == ROOT_PATH) {
        return ReturnStatus(MU_SUCCESS);
    }

    rt = DataNS->RmDir(path.c_str());

    if (-1 == rt) {
        ERROR_LOG("path %s, rmdir() error, %s.",
                  path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}


void
FileMetaDAO::addFileSizeToDelta(const std::string &path, uint64_t *pDelta)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;
    Args fd;

    // read file size
    
    fd = DataNS->Open(path.c_str(), O_RDONLY);

    if (false == fd.valid) {
        ERROR_LOG("path %s, open() error, %s", path.c_str(), strerror(errno));
        return ;
    }

    //int fd = rt;

    FileAttr attr;

    rt = DataNS->readn(&fd, &attr, sizeof(attr));
    DataNS->Close(&fd);

    if (sizeof(attr) != rt) {
        ERROR_LOG("path %s, readn() error", path.c_str());
        return ;
    }

    *pDelta += attr.m_Size;
}

ReturnStatus
FileMetaDAO::getDir(const std::string &path, std::list<PDEntry> *pEntryList)
{
    assert(pEntryList);

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    int rt = 0;
    int error = 0;
    std::string entryName;
    Args st;

    rt = DataNS->OpenDir(path.c_str(), &st);

    if (false == st.valid) {
        error = errno;

        ERROR_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    Dirent dirent;

    while(DataNS->ReadDirNext(&st, &dirent)){
        entryName = dirent.filename;

        // omit "." and ".."
        // omit user info file in user root dir
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR
            || entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        PDEntry ent;
        ent.m_Name = entryName;

        if (MU_DIRECTORY == dirent.filetype) {
            ent.m_Type = MU_DIRECTORY;

        } else {
            ent.m_Type = MU_REGULAR_FILE;
        }

        pEntryList->push_back(ent);
    }


    return ReturnStatus(MU_SUCCESS);
}


ReturnStatus
FileMetaDAO::statDir(const std::string &path, FileMeta *pMeta)
{
    assert(pMeta);

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    int rt = 0;
    int error = 0;

	FileAttr st;
	rt = DataNS->Stat(path.c_str(), &st);

    if (-1 == rt) {
        error = errno;
        ERROR_LOG("path %s, stat() error, %s.", path.c_str(), strerror(error));

        if (ENOENT == error || ENOTDIR == error) {
            return checkPrefix(path);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    if (st.m_Type != MU_DIRECTORY) {
        ERROR_LOG("path %s, not directory", path.c_str());
        return ReturnStatus(MU_FAILED, NOT_DIRECTORY);
    }

    pMeta->m_Attr.m_Mode = st.m_Mode;
    pMeta->m_Attr.m_CTime = st.m_CTime;
    pMeta->m_Attr.m_MTime = st.m_MTime;
    pMeta->m_Attr.m_Size = st.m_Size;

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::getDir2(const std::string &path,
                     std::list<EDEntry> *pEntryList)
{
    assert(pEntryList);

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    int rt = 0;
    int error = 0;
    std::string entryName;
    std::string npath;
	Args st;

    rt = DataNS->OpenDir(path.c_str(), &st);
    if (false == st.valid) {
        error = errno;

        ERROR_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        if (ENOTDIR == error) {
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);

        } else if (ENOENT == error) {
            return checkPrefix(path);
        }
    }

    FileMeta meta;
    ReturnStatus rs;
    Dirent dirent;

    while(DataNS->ReadDirNext(&st, &dirent)){
        entryName = dirent.filetype;
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

        if (MU_DIRECTORY == dirent.filetype) {
            ent.m_Type = MU_DIRECTORY;

            rs = statDir(npath, &meta);

            if (!rs.success()) {
                ERROR_LOG("path %s, statDir() error", npath.c_str());
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
                ERROR_LOG("path %s, getFile() error", npath.c_str());
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


    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::movDir(const std::string &srcPath,
                    const std::string &destPath)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
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

    rt = DataNS->Move(srcPath.c_str(), destPath.c_str());

    if (-1 == rt) {
        error = errno;
        ERROR_LOG("src path %s, dest path %s, rename() error, %s.",
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
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

	
    int rt = 0;

	FileAttr st;
    rt = DataNS->Stat(path.c_str(), &st);

    if (0 == rt) {
        if (MU_DIRECTORY == st.m_Type) {
            return ReturnStatus(MU_SUCCESS);

        } else {
            ERROR_LOG("path %s, not directory", path.c_str());
            return ReturnStatus(MU_FAILED, NOT_DIRECTORY);
        }
    }

    int error = errno;

    ERROR_LOG("path %s, stat() error, %s", path.c_str(), strerror(error));

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
    	//ERROR_LOG("createFile, %s", path.c_str());
        return createFile(path, meta, pMeta, pDelta);

    } else {
    	//ERROR_LOG("updateFile, %s", path.c_str());
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
    int error = 0;
    Args fd;

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

	fd = DataNS->Open(path.c_str(), O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);

    if (false == fd.valid) {
        error = errno;
        ERROR_LOG("path %s, open() error, %s.", path.c_str(), strerror(error));

        if (EEXIST == error) {
            ERROR_LOG("path %s, file exist", path.c_str());

            // is a regular file?
            ReturnStatus rs = isfile(path);

            if (!rs.success()) {
                return rs;
            }

            fd = DataNS->Open(path.c_str(), O_RDONLY);

            if (false == fd.valid) {
                ERROR_LOG("path %s, open() error, %s",
                          path.c_str(), strerror(errno));
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }


            // check file version

            FileAttr attr;

            rt = DataNS->readn(&fd, &attr, sizeof(attr));

            DataNS->Close(&fd);

            if (sizeof(attr) != rt) {
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }

            pMeta->m_Attr.m_Version = attr.m_Version;

            return ReturnStatus(MU_FAILED, PATH_EXIST);

        } else if (ENOENT == error || ENOTDIR == error) {
            ERROR_LOG("path %s, path invalid", path.c_str());
            return ReturnStatus(MU_FAILED, PATH_INVALID);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }


    ReturnStatus rs = writeFileMeta(&fd, meta);

    DataNS->Close(&fd);

    // return file size delta
    *pDelta = meta.m_Attr.m_Size;

    return rs;
}

ReturnStatus
FileMetaDAO::updateFile(const std::string &path, const FileMeta &meta,
                        FileMeta *pMeta, int *pDelta)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;
    Args fd;

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    fd = DataNS->Open(path.c_str(), O_RDWR);

    if (-1 == rt) {
        ERROR_LOG("path %s, open() error, %s.", path.c_str(), strerror(errno));

        // already check path errors in isfile()
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }


    // check file version

    FileAttr attr;

    rt = DataNS->readn(&fd, &attr, sizeof(attr));

    if (sizeof(attr) != rt) {
        ERROR_LOG("readn() error");
        DataNS->Close(&fd);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (meta.m_Attr.m_Version != attr.m_Version + 1) {
        ERROR_LOG("version outdated, current version %" PRIu64 ", "
                  "received version %" PRIu64,
                  attr.m_Version, meta.m_Attr.m_Version);
        DataNS->Close(&fd);
        pMeta->m_Attr = attr;
        return ReturnStatus(MU_FAILED, VERSION_OUTDATED);
    }

    // write metadata

    rs = writeFileMeta(&fd, meta);

    DataNS->Close(&fd);;

    // return file size delta
    *pDelta = meta.m_Attr.m_Size - attr.m_Size;

    return rs;
}


ReturnStatus
FileMetaDAO::writeFileMeta(Args *fd, const FileMeta &meta)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    int rt = 0;

    rt = DataNS->Lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        ERROR_LOG("lseek() error, %s.", strerror(errno));
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

    rt = DataNS->writen(fd, pBuf, bufIdx);

    delete [] pBuf;
    pBuf = NULL;

    if (bufIdx != rt) {
        ERROR_LOG("writen() error");
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
    //ERROR_LOG("writev() error, %s.", strerror(errno));
    //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    //}

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::isfile(const std::string &path)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;
    FileAttr st;

    rt = DataNS->Stat(path.c_str(), &st);

    if (0 == rt) {
        if (MU_REGULAR_FILE == st.m_Type) {
            return ReturnStatus(MU_SUCCESS);

        } else if (MU_DIRECTORY == st.m_Type) {
            ERROR_LOG("path %s, is directory", path.c_str());
            return ReturnStatus(MU_FAILED, IS_DIRECTORY);

        } else {
            ERROR_LOG("path %s, unexpected file type", path.c_str());
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    int error = errno;

    ERROR_LOG("path %s, stat() error, %s", path.c_str(), strerror(error));

    if (ENOENT == error || ENOTDIR == error) {
        return checkPrefix(path);

    } else {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }
}

ReturnStatus
FileMetaDAO::delFile(const std::string &path, int *pDelta)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;
    Args fd;

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    // read file size
    fd = DataNS->Open(path.c_str(), O_RDONLY);

    if (false == fd.valid) {
        ERROR_LOG("path %s, open() error, %s", path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }


    FileAttr attr;

    rt = DataNS->readn(&fd, &attr, sizeof(attr));
    DataNS->Close(&fd);

    if (sizeof(attr) != rt) {
        ERROR_LOG("path %s, readn() error", path.c_str());
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    *pDelta = attr.m_Size;

    // do delete action

    rt = DataNS->Unlink(path.c_str());

    if (-1 == rt) {
        ERROR_LOG("path %s, unlink() error, %s.",
                  path.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
FileMetaDAO::getFile(const std::string &path, FileMeta *pMeta)
{
    assert(pMeta);

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    // clear possible data
    pMeta->m_BlockList.clear();

    ReturnStatus rs;

    rs = isfile(path);

    if (!rs.success()) {
        return rs;
    }

    int rt = 0;
    Args fd;

    fd = DataNS->Open(path.c_str(), O_RDONLY);

    if (false == fd.valid) {
        ERROR_LOG("path %s, open() error, %s.", path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }


    rs = readFileMeta(&fd, pMeta);

    DataNS->Close(&fd);

    if (!rs.success()) {
        ERROR_LOG("path %s, readFileMeta() error", path.c_str());
    }

    return rs;
}


ReturnStatus
FileMetaDAO::readFileMeta(Args *fd, FileMeta *pMeta)
{
	/*
    assert(pMeta);

    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

    int rt = 0;

    rt = DataNS->readn(fd, &(pMeta->m_Attr), sizeof(pMeta->m_Attr));

    if (sizeof(pMeta->m_Attr) != rt) {
        ERROR_LOG("read attr, readn() error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int blocks = pMeta->m_Attr.m_Size / FIXED_BLOCK_SIZE;

    if (pMeta->m_Attr.m_Size % FIXED_BLOCK_SIZE != 0) {
        ++blocks;
    }

    char *pBlockList = new char[blocks * FIXED_BLOCK_CHECKSUM_LEN];

    rt = DataNS->readn(fd, pBlockList, blocks * FIXED_BLOCK_CHECKSUM_LEN);

    if (blocks * FIXED_BLOCK_CHECKSUM_LEN != rt) {
        ERROR_LOG("read block list, readn() error");
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

	*/
//----------------
    assert(pMeta);
    Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;

	int rt = 0;

	//TODO
	int max_blocks = 512;//max file size is 256MB
	char *buffer = new char[max_blocks * FIXED_BLOCK_CHECKSUM_LEN];
	rt = DataNS->readn(fd, buffer, max_blocks * FIXED_BLOCK_CHECKSUM_LEN);
	memcpy(&(pMeta->m_Attr), buffer, sizeof(pMeta->m_Attr));

	int blocks = pMeta->m_Attr.m_Size / FIXED_BLOCK_SIZE;

	if (pMeta->m_Attr.m_Size % FIXED_BLOCK_SIZE != 0) {
        ++blocks;
    }

	if ((sizeof(pMeta->m_Attr) + blocks * FIXED_BLOCK_CHECKSUM_LEN) != rt) {
        ERROR_LOG("read file meta, readn() error");
        delete [] buffer;
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    BlockMeta bmeta;

    for(int i = 0; i < blocks; ++i){
    	bmeta.m_Checksum = 
    		std::string(buffer + sizeof(pMeta->m_Attr) + i * FIXED_BLOCK_CHECKSUM_LEN,
    					FIXED_BLOCK_CHECKSUM_LEN);
    					
    	pMeta->m_BlockList.push_back(bmeta);
    		
    }
    
	delete [] buffer;

	return ReturnStatus(MU_SUCCESS);
    
}


ReturnStatus
FileMetaDAO::movFile(const std::string &srcPath,
                     const std::string &destPath)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
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

    rt = DataNS->Move(srcPath.c_str(), destPath.c_str());

    if (-1 == rt) {
        error = errno;
        ERROR_LOG("src path %s, dest path %s, rename() error, %s.",
                  srcPath.c_str(), destPath.c_str(), strerror(error));

        if (EISDIR == error) {
            return ReturnStatus(MU_FAILED, DEST_PATH_IS_DIRECTORY);

        } else {
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    return ReturnStatus(MU_SUCCESS);
}





