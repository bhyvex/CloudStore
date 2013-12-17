/*
 * @file LogFlushTask.cpp
 * @brief Task to flush logs to local disk.
 *
 * @version 1.0
 * @date Tue Jul 10 09:58:59 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogFlushTask.h"

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "frame/MURegister.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "LogDispatcher.h"

#include <assert.h>
#include <time.h>

#include <list>
#include <map>
#include <memory>

LogFlushTask::~LogFlushTask()
{
    if (NULL != m_pTimer) {
        recycle(m_pTimer);
        m_pTimer = NULL;
    }
}

LogFlushTask::LogFlushTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_pTimer = NULL;
}

void
LogFlushTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    FATAL_LOG("log flush task, timer error");
    recycle();
}

int
LogFlushTask::start()
{
    int rt = 0;

    m_pTimer = new MUTimer(MURegister::getInstance()->getEpoll(), this);

    rt = m_pTimer->create();

    if (-1 == rt) {
        ERROR_LOG("create timer error");
        recycle(m_pTimer);
        m_pTimer = NULL;
        return -1;
    }

    rt = m_pTimer->setTime(LOG_FLUSH_PERIOD,
                           LOG_FLUSH_PERIOD);

    if (-1 == rt) {
        ERROR_LOG("timer set time error");
        recycle(m_pTimer);
        m_pTimer = NULL;
        return -1;
    }

    return 0;
}

int
LogFlushTask::next(MUTimer *pTimer, uint64_t times)
{
    int rt = 0;

    rt = flushLog();

    if (-1 == rt) {
        ERROR_LOG("flush logs failed");
    }

    return 0;
}

int
LogFlushTask::flushLog()
{
    int rt = 0;

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    std::list<uint64_t> bucketList;

    BucketManager::getInstance()->getAllBuckets(&bucketList);

    for (std::list<uint64_t>::iterator it = bucketList.begin();
         it != bucketList.end(); ++it) {
        pLogDispatcher->flushLog(*it);
    }

    return 0;
}



