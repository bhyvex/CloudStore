/*
 * @file ThreadPoolDispatcher.cpp
 * @brief Request dispatcher of thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:38:16 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "ThreadPoolDispatcher.h"

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "log/log.h"
#include "MURegister.h"

ThreadPoolDispatcher::ThreadPoolDispatcher(
    Epoll *pEpoll,
    ThreadPool3 *pPool) :
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


    setNonblock(m_hThreadPoolHandle);

    // �����̳߳ؾ��
    m_WriteEpollEvent.setFd(m_hThreadPoolHandle);
    m_WriteEpollEvent.registerWEvent();
    // �ر�д�¼�
    m_WriteEpollEvent.closeWevent();

    // init pipe
    int fd[2];
    int rt = pipe(fd);

    if (0 != rt) {
        ERROR_LOG("Syscall Error: pipe. %s", strerror(errno));
    }

    m_hReadHandle = fd[0];
    m_hWriteHandle = fd[1];

    setNonblock(m_hReadHandle);

    m_ReadEpollEvent.setFd(m_hReadHandle);

    // ע��ܵ����¼�
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
        ERROR_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return val;
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        ERROR_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return -1;
    }

    return 0;
}

void
ThreadPoolDispatcher::postRequest(ThreadPoolWorkItem3 *pWorkItem)
{
	ThreadPool3 *pThreadPool3 = MURegister::getInstance()->getThreadPool();
    pThreadPool3->postRequest(pWorkItem);
}

int
ThreadPoolDispatcher::sendData()
{
    int rt = 0;

    while (!m_WorkItemList.empty()) {
        rt = m_pThreadPool->postRequest(m_WorkItemList.front());

        if (0 != rt) {
            if (EAGAIN != rt) {
                ERROR_LOG("Error occurred while post request to thread pool, "
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
        // �ر�д�¼�
        m_WriteEpollEvent.closeWevent();
    }

    return 0;
}

int
ThreadPoolDispatcher::recvData()
{
    return 0;
}

