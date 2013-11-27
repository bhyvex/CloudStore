/*
 * @file LogThread.cpp
 * @brief A thread to write operation logs.
 *
 * @version 1.0
 * @date Mon Jul  9 20:35:50 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "LogThread.h"

#include "LogAccessEngine.h"
#include "LogEvent.h"

#include "util/util.h"
#include "log/log.h"

#ifdef DEBUG

#include <time.h>

uint64_t g_logs = 0;
struct timeval g_log_begin;
struct timeval g_log_end;

#endif

LogThread::~LogThread()
{

}

LogThread::LogThread()
{

}

void
LogThread::setQueueFd(int fd)
{
    m_QueueFd = fd;
}

void
LogThread::run()
{
    int rt = 0;

    LogThreadRequest *pReq = NULL;

#ifdef DEBUG

    gettimeofday(&g_log_begin, NULL);

#endif

    for (; ;) {
        rt = util::io::readn(m_QueueFd, &pReq, sizeof(pReq));

        if (sizeof(pReq) != rt) {
            ERROR_LOG("In LogThread::run(), readn() error.");
            continue;
        }

        doRequest(pReq);

        delete pReq;
        pReq = NULL;

#ifdef DEBUG

        if (++g_logs == 5000) {
            gettimeofday(&g_log_end, NULL);

            uint32_t msec =
                (g_log_end.tv_sec * 1000 + g_log_end.tv_usec / 1000) -
                (g_log_begin.tv_sec * 1000 - g_log_begin.tv_usec / 1000);

            WARN_LOG("logs per second: %" PRIu32, g_logs * 1000 / msec);

            g_logs = 0;

            gettimeofday(&g_log_begin, NULL);
        }

#endif

    }
}

void
LogThread::doRequest(LogThreadRequest *pReq)
{
    switch (pReq->m_OpType) {

    case LogThreadRequest::ROTATE_LOG: {
            rotateLog(pReq);
            break;
        }

    case LogThreadRequest::WRITE_LOG: {
            writeLog(pReq);
            break;
        }

    case LogThreadRequest::CLOSE_LOG: {
            closeLog(pReq);
            break;
        }

    case LogThreadRequest::FLUSH_LOG: {
            flushLog(pReq);
            break;
        }

    default: {
            DEBUG_LOG("unknown log request type %d.", pReq->m_OpType);
            break;
        }

    }
}

void
LogThread::rotateLog(LogThreadRequest *pReq)
{
    int rt = 0;

    rt = LogAccessEngine::getInstance()->rotate(
             pReq->m_LogEvent.m_BucketId,
             pReq->m_TotalBuckets,
             pReq->m_LogEvent
         );

    if (-1 == rt) {
        ERROR_LOG("Try to rotate log for bucket %" PRIu64 ", but failed.",
                  pReq->m_LogEvent.m_BucketId);
    }
}

void
LogThread::writeLog(LogThreadRequest *pReq)
{
    int rt = 0;

    rt = LogAccessEngine::getInstance()->write(pReq->m_LogEvent);

    if (-1 == rt) {
        FATAL_LOG("Write log for bucket %llu failed.",
                  pReq->m_LogEvent.m_BucketId);
    }
}

void
LogThread::closeLog(LogThreadRequest *pReq)
{
    int rt = 0;

    rt = LogAccessEngine::getInstance()->close(pReq->m_LogEvent.m_BucketId);

    if (-1 == rt) {
        ERROR_LOG("Try to close log for bucket %llu, but failed.",
                  pReq->m_LogEvent.m_BucketId);
    }
}

void
LogThread::flushLog(LogThreadRequest *pReq)
{
    int rt = 0;

    rt = LogAccessEngine::getInstance()->flush(pReq->m_LogEvent.m_BucketId);

    if (-1 == rt) {
        ERROR_LOG("Try to flush log for bucket %" PRIu64 ", but failed.",
                  pReq->m_LogEvent.m_BucketId);
    }
}


