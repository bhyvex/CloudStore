/*
 * @file StopBucketTask.cpp
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 18:27:33 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "StopBucketTask.h"

#include <inttypes.h>
#include <assert.h>

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUMacros.h"
#include "frame/MURegister.h"
#include "frame/MUTaskManager.h"
#include "frame/MUWorkItemManager.h"
#include "oplog/LogDispatcher.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"

StopBucketTask::~StopBucketTask()
{
    if (TASK_FINISH != m_CurrentState) {
        ERROR_LOG("stop bucket %llu failed", m_BucketId);
    }

    if (NULL != m_pTimer) {
        recycle(m_pTimer);
        m_pTimer = NULL;
    }
}

StopBucketTask::StopBucketTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_pTimer = NULL;
}

void
StopBucketTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    ERROR_LOG("stop bucket task, timer error");
    recycle();
}

int
StopBucketTask::start()
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_INIT: {
            INFO_LOG("try to stop bucket %llu", m_BucketId);
            return registerTimer();
            break;
        }

    default: {
            // never reach here
            ERROR_LOG("unexpected state %d", m_CurrentState);
            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}

int
StopBucketTask::registerTimer()
{
    int rt = 0;

    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL == pBucket) {
        ERROR_LOG("no such bucket");
        return -1;
    }

    pBucket->m_BucketState = MU_BUCKET_STOP;

    Epoll *pEpoll = MURegister::getInstance()->getEpoll();

    m_pTimer = new MUTimer(pEpoll, this);

    rt = m_pTimer->create();

    if (-1 == rt) {
        ERROR_LOG("create timer failed");
        return -1;
    }

    m_pTimer->setTime(REQUEST_QUEUE_CHECK_PERIOD, REQUEST_QUEUE_CHECK_PERIOD);

    m_CurrentState = TASK_CHECK_REQUEST_QUEUE;

    return 0;
}

int
StopBucketTask::next(MUTimer *pChannel, uint64_t times)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_CHECK_REQUEST_QUEUE: {
            rt = checkRequestQueue();

            if (-1 == rt) {
                ERROR_LOG("checkRequestQueue() error");
                recycle();
                return -1;
            }

            break;
        }

    case TASK_FINISH: {
            ERROR_LOG("task already finished, no timer should open");
            break;
        }

    default: {
            ERROR_LOG("unexpected state %d.", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
StopBucketTask::checkRequestQueue()
{
    MUWorkItemManager *pItemManager =
        MURegister::getInstance()->getCurrentItemManager();

    if (pItemManager->noItemsInBucket(m_BucketId)) {
        m_CurrentState = TASK_CHECK_REQUEST_QUEUE_COMPLETED;

        // close log connection
        LogDispatcher *pLogDispatcher =
            MURegister::getInstance()->getLogDispatcher();
        pLogDispatcher->closeLog(m_BucketId);

        // call back
        if (NULL != m_pParent) {
            m_pParent->next(this);
            // do not need it any more
            m_pParent = NULL;
        }

        // close timer
        m_pTimer->disarm();

        // finish
        m_CurrentState = TASK_FINISH;
        INFO_LOG("stop bucket %" PRIu64 " successfully", m_BucketId);

        recycle();

        return 0;
    }

    // still have some requests to do

    return 0;
}

