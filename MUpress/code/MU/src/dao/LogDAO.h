/*
 * @file LogDAO.h
 * @brief Access user logs.
 *
 * @version 1.0
 * @date Mon Jul  9 23:10:39 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef ClientPM_LogDAO_H_
#define ClientPM_LogDAO_H_

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"
#include "oplog/LogAccessEngine.h"
#include "oplog/LogEvent.h"

#include <list>

#include <inttypes.h>

class LogDAO : public MUDAO
{
public:
    virtual ~LogDAO();

    LogDAO();

    ReturnStatus queryUserLog(
        uint64_t bucketId, uint64_t userId,
        uint64_t seq, std::list<LogEvent> *pLogList, bool *pMore);

    ReturnStatus queryCurrentUserLogSeqNr(
        uint64_t bucketId, uint64_t userId, uint64_t *pSeqNr);

    ReturnStatus queryCurrentBucketLogSeqNr(
        uint64_t bucketId, uint64_t *pSeqNr);

};

#endif  // ClientPM_LogDAO_H_

