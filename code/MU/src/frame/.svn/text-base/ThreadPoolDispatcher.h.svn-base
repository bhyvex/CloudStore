/*
 * @file ThreadPoolDispatcher.h
 * @brief Request dispatcher of thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:36:21 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef ThreadPoolDispatcher_H_
#define ThreadPoolDispatcher_H_

#include <list>

#include "comm/Agent.h"
#include "comm/Epoll.h"
#include "comm/EpollEvent.h"
#include "sys/ThreadPool.h"

/**
 * @brief Dispatcher类自身维护一根管道，
 * 用于线程池中的任务将结果写回上层，
 * 此管道的读端注册到Epoll。
 * 另外，获取线程池管道的写端，将其注册到Epoll，
 * 这样可以非阻塞地将任务下发到线程池。
 */
class ThreadPoolDispatcher : public Agent
{
public:
    virtual ~ThreadPoolDispatcher();

    ThreadPoolDispatcher(Epoll *pEpoll, ThreadPool *pPool);

    /**
     * @brief 下发任务到线程池
     *
     * @param pWorkItem
     */
    virtual void postRequest(ThreadPoolWorkItem *pWorkItem);

    /**
     * @brief 读事件到来，从管道读取处理结果
     *
     * @return
     */
    virtual int recvData();

    /**
     * @brief 写事件到来，将任务下发到线程池
     *
     * @return
     */
    virtual int sendData();

protected:
    /**
     * @brief 将描述符设为非阻塞
     *
     * @param fd 描述符
     *
     * @return
     */
    int setNonblock(int fd);

protected:
    ThreadPool *m_pThreadPool;
    int m_hThreadPoolHandle;
    int m_hReadHandle;
    int m_hWriteHandle;
    std::list<ThreadPoolWorkItem *> m_WorkItemList;

    // for epoll
    Epoll *m_pEpoll;
    EpollEvent m_ReadEpollEvent;
    EpollEvent m_WriteEpollEvent;
};

#endif  // ThreadPoolDispatcher_H_

