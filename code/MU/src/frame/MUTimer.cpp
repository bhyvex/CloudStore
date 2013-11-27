/*
 * @file MUTimer.cpp
 * @brief Timer for MU.
 *
 * @version 1.0
 * @date Thu Jun 28 20:59:32 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTimer.h"

#include "timer/timer.h"
#include "log/log.h"

#include "MUTask.h"

#include <assert.h>

MUTimer::~MUTimer()
{
    if (NULL != m_pTask) {
        m_pTask->destroy(this);
        m_pTask = NULL;
    }
}

MUTimer::MUTimer(Epoll *pEpoll) :
    Timer(pEpoll)
{
    m_pTask = NULL;
}

MUTimer::MUTimer(Epoll *pEpoll, MUTask *pTask) :
    Timer(pEpoll)
{
    m_pTask = pTask;
}

void
MUTimer::setTask(MUTask *pTask)
{
    m_pTask = pTask;
}

int
MUTimer::expirationCallback(uint64_t times)
{
    assert(m_pTask);

    int rt = 0;

    rt = m_pTask->next(this, times);

    if (-1 == rt) {
        DEBUG_LOG("m_pTask->next() error");
    }

    return 0;
}

