/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.cpp
 *
 *    Description:  线程池实现
 *
 *        Version:  1.0
 *        Created:  04/27/2011 09:46:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "ThreadPool.h"

#include <list>

#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "Thread.h"

#include "log/log.h"
#include "util/util.h"

ThreadPool::EventHandlerThread::
EventHandlerThread(ThreadPool *pool) :
    m_pThreadPool(pool)
{
    if (NULL == pool) {
        DEBUG_LOG("NULL pointer argument.");
    }
}

void ThreadPool::EventHandlerThread::
run()
{
    ThreadPoolWorkItem *pWorkItem = NULL;

    for (; ;) {
        //ERROR_LOG("I'm 0x%x.", m_ThreadID.getRawID());
        // 取任务
        int rt = util::io::readn(m_pThreadPool->m_hReadHandle,
                                 static_cast<void *>(&pWorkItem),
                                 sizeof(void *));

        if (-1 == rt) {
            if (EINTR != errno) {
                DEBUG_LOG("Syscall Error: read. %s", strerror(errno));
            }
        }

        // 处理任务
        pWorkItem->preProcess();
        pWorkItem->process();
        pWorkItem->postProcess();
    }

}

ThreadPool::ThreadPool(int maxIdle, int maxTotal) :
    m_nMaxIdle(maxIdle),
    m_nMaxTotal(maxTotal),
    m_hReadHandle(-1),
    m_hWriteHandle(-1)
{
    if (maxIdle <= 0 ||
        maxTotal <= 0 ||
        maxIdle < maxTotal) {
        m_nMaxIdle = THREAD_MAX_IDLE_DEFAULT;
        m_nMaxTotal = THREAD_MAX_TOTAL_DEFAULT;
    }
}

int ThreadPool::getHandle() const
{
    return m_hWriteHandle;
}

int ThreadPool::postRequest(ThreadPoolWorkItem *pWorkItem)
{
    int rt = util::io::writen(m_hWriteHandle, &pWorkItem, sizeof(void *));

    if (sizeof(void *) != rt) {
        if (EAGAIN != errno) {
            DEBUG_LOG("writen error");
        }

        return errno;
    }

    return 0;
}

int ThreadPool::start()
{
    // init pipe
    int fd[2];

    int rt = pipe(fd);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pipe. %s", strerror(errno));
        return -1;

    } else {
        m_hReadHandle = fd[0];
        m_hWriteHandle = fd[1];
    }

    // create threads
    for (int i = 0; i < m_nMaxIdle; ++i) {
        m_ThreadList.push_back(new EventHandlerThread(this));
    }

    for (std::list<Thread *>::iterator it = m_ThreadList.begin();
         it != m_ThreadList.end();
         ++it) {
        rt = (*it)->start();

        if (-1 == rt) {
            DEBUG_LOG("Error occurred whilst starting a thread.");
            return -1;
        }
    }

    return 0;
}

int ThreadPool::stop()
{
    return 0;
}

ThreadPool::~ThreadPool()
{
    for (std::list<Thread *>::iterator it = m_ThreadList.begin();
         it != m_ThreadList.end();
         ++it) {
        delete(*it);
    }
}

int ThreadPool::ThreadExitWorkItem::
process()
{
    return 0;
}

