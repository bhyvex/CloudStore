/*
 * @file LogDAO.cpp
 * @brief Access user logs.
 *
 * @version 1.0
 * @date Mon Jul  9 23:16:01 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogDAO.h"

#include "frame/ReturnStatus.h"
#include "frame/MUMacros.h"
#include "oplog/LogEvent.h"
#include "oplog/LogAccessEngine.h"

#include "log/log.h"

#include <list>

#include <assert.h>
#include <inttypes.h>

LogDAO::~LogDAO()
{

}

LogDAO::LogDAO()
{

}


ReturnStatus
LogDAO::queryUserLog(
    uint64_t bucketId, uint64_t userId,
    uint64_t seq, std::list<LogEvent> *pLogList, bool *pMore)
{
    assert(pLogList);

    ReturnStatus rs;
    rs = LogAccessEngine::getInstance()->readUserLog(
             bucketId, userId, seq, pLogList, pMore);

    if (!rs.success()) {
        if (LOG_SEQ_OUTDATED == rs.errorCode) {
            DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                      "log seq %" PRIu64 ", log seq outdated",
                      bucketId, userId, seq);

        } else {
            DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64 ", "
                      "log seq %" PRIu64 ", readUserLog() error",
                      bucketId, userId, seq);
        }

    }

    return rs;
}

ReturnStatus
LogDAO::queryCurrentBucketLogSeqNr(uint64_t bucketId, uint64_t *pSeqNr)
{
    assert(pSeqNr);

    ReturnStatus rs;

    rs = LogAccessEngine::getInstance()->queryCurrentBucketLogSeqNr(
             bucketId, pSeqNr);

    if (!rs.success()) {
        DEBUG_LOG("bucket id %llu, query bucket log seq failed", bucketId);
    }

    return rs;
}

ReturnStatus
LogDAO::queryCurrentUserLogSeqNr(
    uint64_t bucketId, uint64_t userId, uint64_t *pSeqNr)
{
    assert(pSeqNr);

    ReturnStatus rs;
    rs = LogAccessEngine::getInstance()->queryCurrentUserLogSeqNr(
             bucketId, userId, pSeqNr);

    if (!rs.success()) {
        DEBUG_LOG("bucket id %llu, user id %llu, query user log seq failed",
                  bucketId, userId);
    }

    return rs;
}



