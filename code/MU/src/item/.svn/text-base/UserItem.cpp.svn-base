/*
 * @file UserItem.cpp
 * @brief Thread pool request for user-related operations.
 *
 * @version 1.0
 * @date Thu Jul  5 17:08:16 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "UserItem.h"

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"
#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "data/UserInfo.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "dao/UserDAO.h"
#include "dao/LogDAO.h"
#include "oplog/LogAccessEngine.h"

#include "log/log.h"


#include <inttypes.h>

UserItem::~UserItem()
{

}

UserItem::UserItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_RequestId = 0;
    m_UserId = 0;
    m_BucketId = 0;
    m_UserLogSeq = 0;
}

int
UserItem::process()
{
    switch (m_WorkType) {

    case CREATE_USER: {
            createUser();
            break;
        }

    case DELETE_USER: {
            deleteUser();
            break;
        }

    case GET_USER_INFO: {
            readUserInfo();
            break;
        }

    default: {
            DEBUG_LOG("unknown work type %" PRIi32, m_WorkType);
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }

    }

    return 0;
}

void
UserItem::createUser()
{
    TRACE_LOG("create user, bucket %" PRIu64 ", user %" PRIu64,
              m_BucketId, m_UserId);

    UserDAO *pUserDAO = new UserDAO();

    m_ReturnStatus = pUserDAO->createUser(m_BucketId, m_UserId, m_UserQuota);

    if (!m_ReturnStatus.success()) {
        DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                  "create user files failed",
                  m_BucketId, m_UserId);

        delete pUserDAO;
        pUserDAO = NULL;

        return ;
    }

    int rt = 0;

    rt = LogAccessEngine::getInstance()->createUser(m_BucketId, m_UserId);

    if (-1 == rt) {
        DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                  "create user log records failed", m_BucketId, m_UserId);

        // rollback
        m_ReturnStatus = pUserDAO->deleteUser(m_BucketId, m_UserId);

        if (!m_ReturnStatus.success()) {
            DEBUG_LOG("error occurred whilst rollback user-create operation");
        }

        m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);

        delete pUserDAO;
        pUserDAO = NULL;

        return ;
    }

    m_ReturnStatus = ReturnStatus(MU_SUCCESS);

    delete pUserDAO;
    pUserDAO = NULL;
}

void
UserItem::deleteUser()
{
    TRACE_LOG("delete user, bucket %" PRIu64 ", user %" PRIu64,
              m_BucketId, m_UserId);

    UserDAO *pUserDAO = new UserDAO();

    m_ReturnStatus = pUserDAO->deleteUser(m_BucketId, m_UserId);

    delete pUserDAO;
    pUserDAO = NULL;

    if (!m_ReturnStatus.success()) {
        DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                  "delete user files failed",
                  m_BucketId, m_UserId);
        return ;
    }

    int rt = 0;

    rt = LogAccessEngine::getInstance()->deleteUser(m_BucketId, m_UserId);

    if (-1 == rt) {
        DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                  "delete user log records failed",
                  m_BucketId, m_UserId);
        // ignore this error
    }

    m_ReturnStatus = ReturnStatus(MU_SUCCESS);
}

void
UserItem::readUserInfo()
{
    TRACE_LOG("get user info, bucket %" PRIu64, ", user %" PRIu64,
              m_BucketId, m_UserId);

    // check user
    ReturnStatus rs;

    rs = userExists();

    if (!rs.success()) {
        m_ReturnStatus = rs;
        return ;
    }

    UserDAO *pUDAO = new UserDAO();

    m_ReturnStatus = pUDAO->readUserInfo(m_BucketId, m_UserId, &m_UserInfo);

    delete pUDAO;
    pUDAO = NULL;

    if (!m_ReturnStatus.success()) {
        DEBUG_LOG("readUserInfo() failed, bucket id %" PRIu64 ", "
                  "user id %" PRIu64,
                  m_BucketId, m_UserId);
        return ;
    }

    LogDAO *pLDAO = new LogDAO();

    uint64_t seqNr = 0;
    m_ReturnStatus = pLDAO->queryCurrentUserLogSeqNr(
                         m_BucketId, m_UserId, &seqNr);

    delete pLDAO;
    pLDAO = NULL;

    if (!m_ReturnStatus.success()) {
        DEBUG_LOG("queryCurrentUserLogSeqNr() failed, bucket id %" PRIu64
                  ", user id %" PRIu64 ,
                  m_BucketId, m_UserId);
        return ;
    }

    m_UserLogSeq = seqNr;
}


void
UserItem::delay()
{
    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL != pBucket) {
        pBucket->m_pItemManager->put(m_ItemId, this);

    } else {
        MUWorkItem::delay();
    }
}


ReturnStatus
UserItem::userExists()
{
    int rt = 0;

    std::string userRootPath = userRoot();

    struct stat st;
    rt = ::stat(userRootPath.c_str(), &st);

    if (0 == rt) {

        if (S_ISDIR(st.st_mode)) {
            return ReturnStatus(MU_SUCCESS);

        } else {
            DEBUG_LOG("Invalid user root directory, user id %" PRIu64,
                      m_UserId);
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }
    }

    if (ENOENT == errno || ENOTDIR == errno) {
        DEBUG_LOG("no such user, user id %" PRIu64, m_UserId);
        return ReturnStatus(MU_FAILED, MU_LOCATE_ERROR);

    } else {
        DEBUG_LOG("path %s, stat() error, %s.",
                  userRootPath.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }
}

std::string
UserItem::userRoot()
{
    return (
               MUConfiguration::getInstance()->m_FileSystemRoot +
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(m_BucketId) +
               PATH_SEPARATOR_STRING +
               USER_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(m_UserId)
           );
}


