/*
 * @file LogItem.h
 * @brief Thread pool request to read user logs.
 *
 * @version 1.0
 * @date Mon Jul  9 23:23:07 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef ClientPM_LogItem_H_
#define ClientPM_LogItem_H_

#include "frame/MUWorkItem.h"
#include "frame/ReturnStatus.h"
#include "oplog/LogEvent.h"

#include <list>

class LogItem : public MUWorkItem
{
public:
    enum _LogItemWorkType {
        GET_USER_LOG
    };

public:
    virtual ~LogItem();

    explicit LogItem(MUTask *pTask);

    int process();

    void delay();

    inline uint32_t getRequestId();

    inline void setRequestId(uint32_t requestId);

    inline uint64_t getUserId();

    inline void setUserId(uint64_t userId);

    inline uint64_t getBucketId();

    inline void setBucketId(uint64_t bucketId);

    inline uint64_t getUserLogSeq();

    inline void setUserLogSeq(uint64_t seq);

    inline const std::list<LogEvent> &getLogList();

    inline bool moreLogs();

protected:
    void queryUserLog();

private:
    // client request id
    uint32_t m_RequestId;

    // general
    uint64_t m_UserId;
    uint64_t m_BucketId;

    // current user log seq
    uint64_t m_UserLogSeq;

    std::list<LogEvent> m_LogList;

    // more logs to read?
    bool m_MoreLogs;
};


uint32_t
LogItem::getRequestId()
{
    return m_RequestId;
}

void
LogItem::setRequestId(uint32_t requestId)
{
    m_RequestId = requestId;
}

uint64_t
LogItem::getUserId()
{
    return m_UserId;
}

void
LogItem::setUserId(uint64_t userId)
{
    m_UserId = userId;
}

uint64_t
LogItem::getBucketId()
{
    return m_BucketId;
}

void
LogItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

uint64_t
LogItem::getUserLogSeq()
{
    return m_UserLogSeq;
}

void
LogItem::setUserLogSeq(uint64_t seq)
{
    m_UserLogSeq = seq;
}

const std::list<LogEvent> &
LogItem::getLogList()
{
    return m_LogList;
}

bool
LogItem::moreLogs()
{
    return m_MoreLogs;
}

#endif  // ClientPM_LogItem_H_

