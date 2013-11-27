/*
 * @file MURegister.h
 * @brief Register of MU, maintain some global object.
 *
 * @version 1.0
 * @date Mon Jul  2 16:26:09 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef MURegister_H_
#define MURegister_H_

#include "sys/sys.h"
#include "comm/comm.h"

#include "MUWorkItemManager.h"
#include "MUTPDispatcher.h"
#include "MUWorkItemManager.h"
#include "oplog/LogDispatcher.h"

#include <string>

class MURegister : public Singleton<MURegister>
{
    friend class Singleton<MURegister>;

public:
    virtual ~MURegister();

    inline Epoll *getEpoll();

    inline void setEpoll(Epoll *pEpoll);

    inline ThreadPool *getThreadPool();

    inline void setThreadPool(ThreadPool *pPool);

    inline MUTPDispatcher *getThreadPoolDispatcher();

    inline void setThreadPoolDispatcher(MUTPDispatcher *pTPDispatcher);

    inline LogDispatcher *getLogDispatcher();

    inline void setLogDispatcher(LogDispatcher *pLogDispatcher);

    inline MUWorkItemManager *getCurrentItemManager();

    inline MUWorkItemManager *getDelayItemManager();

    inline void setCurrentWorkItemManager(MUWorkItemManager *pWorkItemManager);

    inline void setDelayWorkItemManager(MUWorkItemManager *pWorkItemManager);

    inline bool bigLock();

    inline void setBigLock(bool val);

protected:
    // can't new
    MURegister();

private:
    Epoll *m_pEpoll;
    ThreadPool *m_pThreadPool;
    MUTPDispatcher *m_pTPDispatcher;
    MUWorkItemManager *m_pCurrentItemManager;
    MUWorkItemManager *m_pDelayItemManager;
    LogDispatcher *m_pLogDiapatcher;

    // when the lock is set true, all user requests will be blocked
    bool m_BigLock;
};



Epoll *
MURegister::getEpoll()
{
    return m_pEpoll;
}

void
MURegister::setEpoll(Epoll *pEpoll)
{
    m_pEpoll = pEpoll;
}

ThreadPool *
MURegister::getThreadPool()
{
    return m_pThreadPool;
}

void
MURegister::setThreadPool(ThreadPool *pPool)
{
    m_pThreadPool = pPool;
}

MUTPDispatcher *
MURegister::getThreadPoolDispatcher()
{
    return m_pTPDispatcher;
}

void
MURegister::setThreadPoolDispatcher(MUTPDispatcher *pTPDispatcher)
{
    m_pTPDispatcher = pTPDispatcher;
}

LogDispatcher *
MURegister::getLogDispatcher()
{
    return m_pLogDiapatcher;
}

void
MURegister::setLogDispatcher(LogDispatcher *pLogDispatcher)
{
    m_pLogDiapatcher = pLogDispatcher;
}

MUWorkItemManager *
MURegister::getCurrentItemManager()
{
    return m_pCurrentItemManager;
}

MUWorkItemManager *
MURegister::getDelayItemManager()
{
    return m_pDelayItemManager;
}

void
MURegister::setCurrentWorkItemManager(MUWorkItemManager *pWorkItemManager)
{
    m_pCurrentItemManager = pWorkItemManager;
}

void
MURegister::setDelayWorkItemManager(MUWorkItemManager *pWorkItemManager)
{
    m_pDelayItemManager = pWorkItemManager;
}

bool
MURegister::bigLock()
{
    return m_BigLock;
}

void
MURegister::setBigLock(bool val)
{
    m_BigLock = val;
}

#endif  // MURegister_H_


