/*
 * @file RecycleTask.cpp
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 18:27:33 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "RecycleTask.h"

#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUMacros.h"
#include "frame/MUTaskManager.h"
#include "frame/MURegister.h"

#include "log/log.h"
#include "util/util.h"

#ifdef DEBUG

#include <time.h>

uint64_t g_processed = 0;
uint64_t g_processedPerSec = 0;

uint64_t g_received = 0;
uint64_t g_receivedPerSec = 0;

TimeCounter g_tc_req;

#endif


RecycleTask::~RecycleTask()
{
    if (NULL != m_pTimer) {
        m_pTimer->setTask(NULL);
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

RecycleTask::RecycleTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_pTimer = NULL;
}

void
RecycleTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    FATAL_LOG("recycle task, timer error, program will exit");
    exit(1);
}

int
RecycleTask::start()
{
    switch (m_CurrentState) {
    case TASK_INIT: {
            return registerTimer();
            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %d", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
RecycleTask::next(MUTimer *pChannel, uint64_t times)
{
    switch (m_CurrentState) {

    case TASK_RECYCLE: {
            doRecycle();
            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
RecycleTask::registerTimer()
{
    int rt = 0;

    m_pTimer = new MUTimer(MURegister::getInstance()->getEpoll());

    rt = m_pTimer->create();

    if (-1 == rt) {
        DEBUG_LOG("create timer failed");

        m_pTimer->setTask(NULL);
        delete m_pTimer;
        m_pTimer = NULL;

        return -1;
    }

    m_pTimer->setTask(this);
    m_pTimer->setTime(
        TASK_RECYCLE_PERIOD,
        TASK_RECYCLE_PERIOD
    );

    m_CurrentState = TASK_RECYCLE;

    return 0;
}

void
RecycleTask::doRecycle()
{
    MUTaskManager::getInstance()->doRecycle();

#ifdef DEBUG

    int msecs = 0;

    g_tc_req.end();

    msecs = g_tc_req.diff();

    if (msecs < 2000) {
        return ;
    }

    WARN_LOG("processed per second: %" PRIu64,
             g_processedPerSec * 1000 / msecs);

    g_processed += g_processedPerSec;
    WARN_LOG("processed total: %" PRIu64,
             g_processed);

    g_processedPerSec = 0;

    WARN_LOG("received per second: %" PRIu64,
             g_receivedPerSec * 1000 / msecs);

    g_received += g_receivedPerSec;
    WARN_LOG("received total: %" PRIu64,
             g_received);

    g_receivedPerSec = 0;

    g_tc_req.begin();

#endif

}

