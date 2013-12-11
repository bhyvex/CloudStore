/*
 * @file FileOpItem.cpp
 * @brief Thread pool work item for file operations.
 *
 * @version 1.0
 * @date Tue Jul  3 15:27:57 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "FileOpItem.h"

#include "log/log.h"
#include "util/util.h"

#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "data/FileMeta.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "state/User.h"
#include "state/UserManager.h"
#include "storage/ChannelManager.h"
#include "storage/Channel.h"
#include "storage/NameSpace.h"
#include "storage/FSNameSpace.h"

#include "protocol/MUMacros.h"


#include "dao/FileMetaDAO.h"
#include "dao/UserDAO.h"

#include <string>

#include <inttypes.h>

FileOpItem::~FileOpItem()
{

}

FileOpItem::FileOpItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_RequestId = 0;
    m_UserId = 0;
    m_BucketId = 0;
}

int
FileOpItem::preProcess()
{
    return 0;
}

int
FileOpItem::process()
{
    // check user
    ReturnStatus rs;

    rs = userExists();

    if (!rs.success()) {
        m_ReturnStatus = rs;
        return 0;
    }

    // lock user
    User *pUser = NULL;

    if (MUConfiguration::getInstance()->m_UserSerializable) {
        pUser = UserManager::getInstance()->get(m_UserId);

        if (NULL == pUser) {
            ERROR_LOG("pUser == NULL");
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            return 0;
        }
    }

    // do work
    switch (m_WorkType) {

    case PUT_DIR: {
            putDir();
            break;
        }

    case DEL_DIR: {
            delDir();
            break;
        }

    case GET_DIR: {
            getDir();
            break;
        }

    case STAT_DIR: {
            statDir();
            break;
        }

    case GET_DIR2: {
            getDir2();
            break;
        }

    case MOV_DIR: {
            movDir();
            break;
        }

    case PUT_FILE: {
            putFile();
            break;
        }

    case DEL_FILE: {
            delFile();
            break;
        }

    case GET_FILE: {
            getFile();
            break;
        }

    case MOV_FILE: {
            movFile();
            break;
        }

    default: {
            ERROR_LOG("unknown work type %d.", m_WorkType);
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }
    }

    // release user

    if (MUConfiguration::getInstance()->m_UserSerializable) {
        UserManager::getInstance()->release(pUser);
    }

    return 0;
}


std::string
FileOpItem::absPath(const std::string &path)
{
    return (userRoot() + path);
}

void
FileOpItem::putDir()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("put dir, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->putDir(path);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, put dir failed", path.c_str());
    }
}

void
FileOpItem::delDir()
{
    std::string path = absPath(m_Path);//"bucket1/user1/hehe"

    TRACE_LOG("del dir, path %s", path.c_str());

    uint64_t delta = 0;

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->delDir(path, &delta);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, delete directory failed", path.c_str());
        return ;
    }

    // change user quota
	if(MUConfiguration::getInstance()->m_UserDeltaKeep) {
	
	    UserDAO *pUserDAO = new UserDAO();

	    struct UserInfo info;

	    m_ReturnStatus = pUserDAO->readUserInfo(m_BucketId, m_UserId, &info);

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, readUserInfo() failed", path.c_str());
	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);

	        delete pUserDAO;
	        pUserDAO = NULL;

	        return ;
	    }

	    info.m_UsedQuota -= delta;

	    m_ReturnStatus = pUserDAO->writeUserInfo(m_BucketId, m_UserId, info);

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, writeUserInfo() failed", path.c_str());

	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);
	    }

	    delete pUserDAO;
	    pUserDAO = NULL;
    }
}

void
FileOpItem::getDir()
{
    std::string path = absPath(m_Path);//"bucket1/user1/hehe"

    TRACE_LOG("get dir, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->getDir(path, &m_PDEntryList);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, get dir failed", path.c_str());
    }
}

void
FileOpItem::statDir()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("stat dir, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->statDir(path, &m_FileMetaOut);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, stat dir failed", path.c_str());
    }
}

void
FileOpItem::getDir2()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("get dir2, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->getDir2(path, &m_EDEntryList);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, getdir2 failed", path.c_str());
    }
}

void
FileOpItem::movDir()
{
    std::string srcPath = absPath(m_SrcPath);
    std::string destPath = absPath(m_DestPath);

    TRACE_LOG("mov dir, src path %s, dest path %s",
              srcPath.c_str(), destPath.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->movDir(srcPath, destPath);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("src path %s, dest path %s, mov dir failed",
                  srcPath.c_str(), destPath.c_str());
    }
}

void
FileOpItem::putFile()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("put file, path %s", path.c_str());

    INFO_LOG("put file1, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    int delta = 0;
    INFO_LOG("put file2, path %s", path.c_str());
    m_ReturnStatus = pMetaDAO->putFile(
                         absPath(m_Path), m_FileMetaIn,
                         &m_FileMetaOut, &delta);

    delete pMetaDAO;
    pMetaDAO = NULL;
    

	INFO_LOG("put file3, path %s", path.c_str());
	
    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, put file failed", path.c_str());
        return ;
    }

    // change user quota
	if(MUConfiguration::getInstance()->m_UserDeltaKeep) {
	    UserDAO *pUserDAO = new UserDAO();

	    INFO_LOG("put file4, path %s", path.c_str());

	    struct UserInfo info;

	    m_ReturnStatus = pUserDAO->readUserInfo(m_BucketId, m_UserId, &info);

	    INFO_LOG("put file5, path %s", path.c_str());

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, readUserInfo() failed", path.c_str());
	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);

	        delete pUserDAO;
	        pUserDAO = NULL;

	        return ;
	    }

	    INFO_LOG("put file6, path %s", path.c_str());

	    if (delta > 0) {
	        info.m_UsedQuota += delta;

	    } else if (delta < 0) {
	        info.m_UsedQuota -= -delta;
	    }

	    m_ReturnStatus = pUserDAO->writeUserInfo(m_BucketId, m_UserId, info);

	    INFO_LOG("put file7, path %s", path.c_str());

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, writeUserInfo() failed", path.c_str());

	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);
	    }
	    INFO_LOG("put file8, path %s", path.c_str());

	    delete pUserDAO;
	    pUserDAO = NULL;
	}
}

void
FileOpItem::delFile()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("del file, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    int delta = 0;
    m_ReturnStatus = pMetaDAO->delFile(path, &delta);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, delete file failed", path.c_str());
        return ;
    }

    // change user quota
	if(MUConfiguration::getInstance()->m_UserDeltaKeep) {
	    UserDAO *pUserDAO = new UserDAO();

	    struct UserInfo info;

	    m_ReturnStatus = pUserDAO->readUserInfo(m_BucketId, m_UserId, &info);

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, readUserInfo() failed", path.c_str());
	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);

	        delete pUserDAO;
	        pUserDAO = NULL;

	        return ;
	    }

	    info.m_UsedQuota -= delta;

	    m_ReturnStatus = pUserDAO->writeUserInfo(m_BucketId, m_UserId, info);

	    if (!m_ReturnStatus.success()) {
	        ERROR_LOG("path %s, writeUserInfo() failed", path.c_str());

	        // ignore this error
	        m_ReturnStatus = ReturnStatus(MU_SUCCESS);
	    }

	    delete pUserDAO;
	    pUserDAO = NULL;
	}
}

void
FileOpItem::getFile()
{
    std::string path = absPath(m_Path);

    TRACE_LOG("get file, path %s", path.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->getFile(absPath(m_Path), &m_FileMetaOut);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("path %s, get file failed", path.c_str());
    }

}

void
FileOpItem::movFile()
{
    std::string srcPath = absPath(m_SrcPath);
    std::string destPath = absPath(m_DestPath);

    TRACE_LOG("mov file, src path %s, dest path %s",
              srcPath.c_str(), destPath.c_str());

    FileMetaDAO *pMetaDAO = new FileMetaDAO();
    pMetaDAO->setBucketID(m_BucketId);
    pMetaDAO->setUserID(m_UserId);

    m_ReturnStatus = pMetaDAO->movFile(srcPath, destPath);

    delete pMetaDAO;
    pMetaDAO = NULL;

    if (!m_ReturnStatus.success()) {
        ERROR_LOG("src path %s, dest path %s, mov file failed",
                  srcPath.c_str(), destPath.c_str());
    }
}

void
FileOpItem::delay()
{
    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL != pBucket) {
        pBucket->m_pItemManager->put(m_ItemId, this);

    } else {
        MUWorkItem::delay();
    }
}


ReturnStatus
FileOpItem::userExists()
{
	cout <<"FileOpItem::userExists()"<<endl;
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(m_BucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	
    int rt = 0;

    std::string userRootPath = userRoot();
    cout <<"userRootPath="<<userRootPath<<endl;

	FileAttr st;
    rt = DataNS->Stat(userRootPath.c_str(), &st);

    if (0 == rt) {

        if (st.m_Type == MU_DIRECTORY) {
        	cout <<"	user exists!!!!!"<<endl;
            return ReturnStatus(MU_SUCCESS);

        } else {
            ERROR_LOG("invalid user root directory, "
                      "bucket id %llu, user id %llu",
                      m_BucketId, m_UserId);
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    if (ENOENT == errno || ENOTDIR == errno) {
        ERROR_LOG("no such user, user id %llu", m_UserId);
        return ReturnStatus(MU_FAILED, MU_LOCATE_ERROR);

    } else {
        ERROR_LOG("path %s, stat() error, %s.",
                  userRootPath.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }
}

std::string
FileOpItem::userRoot()
{
    return (
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(m_BucketId) +
               PATH_SEPARATOR_STRING +
               USER_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(m_UserId)
           );
}



