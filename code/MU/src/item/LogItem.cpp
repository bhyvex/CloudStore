/*
 * @file LogItem.cpp
 * @brief Thread pool request to read user logs.
 *
 * @version 1.0
 * @date Mon Jul  9 23:29:41 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogItem.h"

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"
#include "frame/MUMacros.h"
#include "frame/MUConfiguration.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"

#include "log/log.h"

#include "dao/LogDAO.h"

#include <inttypes.h>

LogItem::~LogItem()
{

}

LogItem::LogItem(MUTask *pTask) :
    MUWorkItem(pTask)
{
    m_RequestId = 0;
    m_UserId = 0;
    m_BucketId = 0;
    m_UserLogSeq = 0;
    m_MoreLogs = false;
}

int
LogItem::process()
{
    switch (m_WorkType) {

    case GET_USER_LOG: {
            queryUserLog();
            break;
        }

    default: {
            DEBUG_LOG("unknown work type %" PRIi32 , m_WorkType);
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }

    }

    return 0;
}

void
LogItem::queryUserLog()
{
    TRACE_LOG("get user log, bucket %" PRIu64 ", user %" PRIu64 ", "
              "log seq nr %" PRIu64,
              m_BucketId, m_UserId, m_UserLogSeq);

    LogDAO *pLogDAO = new LogDAO();

    m_ReturnStatus = pLogDAO->queryUserLog(
                         m_BucketId,
                         m_UserId,
                         m_UserLogSeq,
                         &m_LogList,
                         &m_MoreLogs
                     );

    delete pLogDAO;
    pLogDAO = NULL;

    if (!m_ReturnStatus.success()) {
        if (LOG_SEQ_OUTDATED == m_ReturnStatus.errorCode) {
            DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                      "log seq %" PRIu64 ", log seq outdated",
                      m_BucketId, m_UserId, m_UserLogSeq);

        } else {
            DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                      "log seq %" PRIu64 ", get user log failed",
                      m_BucketId, m_UserId, m_UserLogSeq);
        }

    } else {
        TRACE_LOG("get user log success, bucket %" PRIu64
                  ", user %" PRIu64 ", log seq nr %" PRIu64
                  ", size %" PRIi32,
                  m_BucketId, m_UserId, m_UserLogSeq, m_LogList.size());
    }
}


void
LogItem::delay()
{
    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL != pBucket) {
        pBucket->m_pItemManager->put(m_ItemId, this);

    } else {
        MUWorkItem::delay();
    }
}

