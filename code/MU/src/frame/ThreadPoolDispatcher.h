/*
 * @file ThreadPoolDispatcher.h
 * @brief Request dispatcher of thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:36:21 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef ThreadPoolDispatcher_H_
#define ThreadPoolDispatcher_H_

#include <list>

#include "comm/Agent.h"
#include "comm/Epoll.h"
#include "comm/EpollEvent.h"
#include "sys/ThreadPool3.h"

/**
 * @brief Dispatcher������ά��һ���ܵ���
 * �����̳߳��е����񽫽��д���ϲ㣬
 * �˹ܵ��Ķ���ע�ᵽEpoll��
 * ���⣬��ȡ�̳߳عܵ���д�ˣ�����ע�ᵽEpoll��
 * �������Է������ؽ������·����̳߳ء�
 */
class ThreadPoolDispatcher : public Agent
{
public:
    virtual ~ThreadPoolDispatcher();

    ThreadPoolDispatcher(Epoll *pEpoll, ThreadPool3 *pPool);

    /**
     * @brief �·������̳߳�
     *
     * @param pWorkItem
     */
    virtual void postRequest(ThreadPoolWorkItem3 *pWorkItem);

    /**
     * @brief ���¼��������ӹܵ���ȡ������
     *
     * @return
     */
    virtual int recvData();

    /**
     * @brief д�¼��������������·����̳߳�
     *
     * @return
     */
    virtual int sendData();

protected:
    /**
     * @brief ����������Ϊ������
     *
     * @param fd ������
     *
     * @return
     */
    int setNonblock(int fd);

protected:
    ThreadPool3 *m_pThreadPool;
    int m_hThreadPoolHandle;
    int m_hReadHandle;
    int m_hWriteHandle;
    std::list<ThreadPoolWorkItem3 *> m_WorkItemList;

    // for epoll
    Epoll *m_pEpoll;
    EpollEvent m_ReadEpollEvent;
    EpollEvent m_WriteEpollEvent;
};

#endif  // ThreadPoolDispatcher_H_

