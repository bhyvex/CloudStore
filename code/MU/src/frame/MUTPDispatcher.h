/*
 * @file MUTPDispatcher.h
 * @brief Request dispatcher of MU thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:42:17 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _MUTPDISPATCHER_H_
#define _MUTPDISPATCHER_H_

#include "ThreadPoolDispatcher.h"

#include "MUWorkItem.h"

class MUTPDispatcher : public ThreadPoolDispatcher
{
public:
    MUTPDispatcher(Epoll *pEpoll, ThreadPool *pPool);

    /**
     * @brief called by Epoll when there's some results in the write-back pipe.
     *
     * @return 0 if a success, or -1
     */
    virtual int recvData();

    /**
     * @brief send result to its owner
     *
     * @param pWorkItem thread pool request
     *
     * @return 0 if a success, or -1
     */
    virtual int postResult(MUWorkItem *pWorkItem);

    /**
     * @brief send request to thread pool
     *
     * @param pWorkItem Thread pool work item.
     */
    void postRequest(MUWorkItem *pWorkItem);
};

#endif  // _MUTPDISPATCHER_H_

