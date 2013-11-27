/*
 * @file LogThread.h
 * @brief A thread to write operation logs.
 *
 * @version 1.0
 * @date Mon Jul  9 20:22:48 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _LogThread_H_
#define _LogThread_H_

#include "sys/Thread.h"

#include "LogEvent.h"

struct LogThreadRequest {
    enum _LogThreadRequestOpType {
        ROTATE_LOG,
        WRITE_LOG,
        CLOSE_LOG,
        FLUSH_LOG,
    };

    int m_OpType;
    LogEvent m_LogEvent;
    // for ROTATE_LOG, total buckets in current system
    uint64_t m_TotalBuckets;

    LogThreadRequest() {
        m_OpType = 0;
        m_TotalBuckets = 0;
    }
};

class LogThread : public Thread
{
public:
    virtual ~LogThread();

    LogThread();

    void setQueueFd(int fd);

    void setBackQueueFd(int fd);

protected:
    void run();

    void doRequest(LogThreadRequest *pReq);

    void rotateLog(LogThreadRequest *pReq);

    void writeLog(LogThreadRequest *pReq);

    void closeLog(LogThreadRequest *pReq);

    void flushLog(LogThreadRequest *pReq);

protected:
    int m_QueueFd;
};

#endif  // _LogThread_H_

