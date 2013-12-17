/*
 * @file LogDispatcher.h
 * @brief Agent for write operation logs.
 *
 * @version 1.0
 * @date Mon Jul  9 21:30:49 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef LogDispatcher_H_
#define LogDispatcher_H_

#include "comm/comm.h"

#include "oplog/LogEvent.h"
#include "oplog/LogThread.h"

#include <list>

#include <inttypes.h>

class LogDispatcher : public Agent
{
public:
    virtual ~LogDispatcher();

    LogDispatcher(Epoll *pEpoll);

    int recvData();

    int sendData();

    void sendLog(LogEvent &event);

    void rotateLog(uint64_t bucketId);

    void closeLog(uint64_t bucketId);

    void flushLog(uint64_t bucketId);

protected:
    int setNonblock(int fd);

    int sendRequestToLogThread(LogThreadRequest *pReq);

protected:
    int m_QueueRD;
    int m_QueueWR;
    LogThread *m_pLogThread;
    std::list<LogThreadRequest *> m_ReqList;

    // for epoll
    Epoll *m_pEpoll;
    EpollEvent m_WriteEpollEvent;
};


#endif  // LogDispatcher_H_

