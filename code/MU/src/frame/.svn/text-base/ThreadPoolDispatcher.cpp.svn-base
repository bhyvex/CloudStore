/*
 * @file ThreadPoolDispatcher.cpp
 * @brief Request dispatcher of thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:38:16 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "ThreadPoolDispatcher.h"

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "log/log.h"

ThreadPoolDispatcher::ThreadPoolDispatcher(
    Epoll *pEpoll,
    ThreadPool *pPool) :
    m_pEpoll(pEpoll),
    m_pThreadPool(pPool)
{
    assert(pEpoll);
    assert(pPool);

    // set state
    m_iConnect = CONNECTED;

    // init EpollEvent
    m_ReadEpollEvent.setEpoll(m_pEpoll);
    m_WriteEpollEvent.setEpoll(m_pEpoll);

    m_ReadEpollEvent.setHandler(this);
    m_WriteEpollEvent.setHandler(this);

    // get thread pool handle
    m_hThreadPoolHandle = m_pThreadPool->getHandle();

    setNonblock(m_hThreadPoolHandle);

    // 设置线程池句柄
    m_WriteEpollEvent.setFd(m_hThreadPoolHandle);
    m_WriteEpollEvent.registerWEvent();
    // 关闭写事件
    m_WriteEpollEvent.closeWevent();

    // init pipe
    int fd[2];
    int rt = pipe(fd);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pipe. %s", strerror(errno));
    }

    m_hReadHandle = fd[0];
    m_hWriteHandle = fd[1];

    setNonblock(m_hReadHandle);

    m_ReadEpollEvent.setFd(m_hReadHandle);

    // 注册管道读事件
    m_ReadEpollEvent.registerREvent();
}

ThreadPoolDispatcher::~ThreadPoolDispatcher()
{
    m_ReadEpollEvent.unregisterRWEvents();
    m_WriteEpollEvent.unregisterRWEvents();

    m_pEpoll = NULL;
    m_pThreadPool = NULL;
}

int
ThreadPoolDispatcher::setNonblock(int fd)
{
    int val = 0;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return val;
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return -1;
    }

    return 0;
}

void
ThreadPoolDispatcher::postRequest(ThreadPoolWorkItem *pWorkItem)
{
    m_WorkItemList.push_back(pWorkItem);

    // 打开线程池句柄写事件
    m_WriteEpollEvent.openWevent();
}

int
ThreadPoolDispatcher::sendData()
{
    int rt = 0;

    while (!m_WorkItemList.empty()) {
        rt = m_pThreadPool->postRequest(m_WorkItemList.front());

        if (0 != rt) {
            if (EAGAIN != rt) {
                DEBUG_LOG("Error occurred while post request to thread pool, "
                          "%s", strerror(errno));
                //return -1;
                break;
            }

            break;

        } else {
            m_WorkItemList.pop_front();
        }
    }

    if (m_WorkItemList.empty()) {
        // 关闭写事件
        m_WriteEpollEvent.closeWevent();
    }

    return 0;
}

int
ThreadPoolDispatcher::recvData()
{
    return 0;
}

