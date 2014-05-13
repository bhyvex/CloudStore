/*
 * @file LogRotationTask.cpp
 * @brief Task to do operation log rotation.
 *
 * @version 1.0
 * @date Tue Jul 10 09:58:59 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogRotationTask.h"

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "frame/MURegister.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "item/LogRotationCheckItem.h"

#include <assert.h>
#include <time.h>

#include <list>
#include <map>
#include <memory>

LogRotationTask::~LogRotationTask()
{
    if (NULL != m_pTimer) {
        recycle(m_pTimer);
        m_pTimer = NULL;
    }
}

LogRotationTask::LogRotationTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_pTimer = NULL;

    m_RotateStrategy =
        MUConfiguration::getInstance()->m_LogRotateStrategy;
    m_RotateSize =
        MUConfiguration::getInstance()->m_LogRotateSize;
    m_RotateTime =
        MUConfiguration::getInstance()->m_LogRotateTime;

    m_LastRotateTime = ::time(NULL);
}

void
LogRotationTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    FATAL_LOG("log rotation task, timer error");
    recycle();
}

int
LogRotationTask::start()
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

    rt = m_pTimer->setTime(LOG_ROTATION_CHECK_PERIOD,
                           LOG_ROTATION_CHECK_PERIOD);

    if (-1 == rt) {
        ERROR_LOG("timer set time error");
        recycle(m_pTimer);
        m_pTimer = NULL;
        return -1;
    }

    return 0;
}

int
LogRotationTask::next(MUTimer *pTimer, uint64_t times)
{
    if (m_RotateStrategy == LOG_ROTATION_STRATEGY_TIME) {
        return doRotationByTime();

    } else if (m_RotateStrategy == LOG_ROTATION_STRATEGY_SIZE) {
        return doRotationBySize();

    } else {
        ERROR_LOG("unknown log rotation strategy %s.",
                  m_RotateStrategy.c_str());
        assert(0);
        return -1;
    }

    return 0;
}

int
LogRotationTask::next(MUWorkItem *pItem)
{
    // sub ref cnt
    release();

    return dispatch(pItem);
}

int
LogRotationTask::doRotationByTime()
{
    uint64_t currentTime = ::time(NULL);

    if (currentTime - m_LastRotateTime >= m_RotateTime) {
        INFO_LOG("do log rotation for all buckets");
        m_LastRotateTime = currentTime;

        LogDispatcher *pLogDispatcher =
            MURegister::getInstance()->getLogDispatcher();

        std::list<uint64_t> bucketList;

        BucketManager::getInstance()->getAllBuckets(&bucketList);

        for (std::list<uint64_t>::iterator it = bucketList.begin();
             it != bucketList.end(); ++it) {
            pLogDispatcher->rotateLog(*it);
        }
    }

    return 0;
}

int
LogRotationTask::doRotationBySize()
{
    std::list<uint64_t> bucketList;

    BucketManager::getInstance()->getAllBuckets(&bucketList);

    LogRotationCheckItem *pItem = new LogRotationCheckItem(this);

    pItem->setItemType(LOG_ROTATION_CHECK_ITEM);
    pItem->setWorkType(LogRotationCheckItem::CHECK_LOG_FILE_SIZE);
    pItem->setBucketList(bucketList);

    sendItem(pItem);

    return 0;
}

int
LogRotationTask::dispatch(MUWorkItem *pItem)
{
    switch (pItem->getItemType()) {

    case LOG_ROTATION_CHECK_ITEM: {
            return dispatchLogRotationCheckItem(pItem);
            break;
        }

    default: {
            ERROR_LOG("unknown work item type %d.", pItem->getItemType());
            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}


int
LogRotationTask::dispatchLogRotationCheckItem(MUWorkItem *pItem)
{
    switch (pItem->getWorkType()) {

    case LogRotationCheckItem::CHECK_LOG_FILE_SIZE: {
            return checkLogFileSize(pItem);
            break;
        }

    default: {
            ERROR_LOG("unknown work type %d.", pItem->getWorkType());
            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}

int
LogRotationTask::checkLogFileSize(MUWorkItem *pItem)
{
    std::auto_ptr<LogRotationCheckItem> pRItem =
        std::auto_ptr<LogRotationCheckItem>(
            dynamic_cast<LogRotationCheckItem *>(pItem));

    ReturnStatus rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        ERROR_LOG("check log file size failed.");
        return -1;
    }

    LogDispatcher *pLogDispatcher =
        MURegister::getInstance()->getLogDispatcher();

    const std::map<uint64_t, uint64_t> &logMap = pRItem->getLogMap();

    for (std::map<uint64_t, uint64_t>::const_iterator it = logMap.begin();
         it != logMap.end(); ++it) {
        if (it->second >= m_RotateSize) {
            //INFO_LOG("do rotation for bucket " PRIu64, it->first);
            pLogDispatcher->rotateLog(it->first);
        }
    }

    return 0;
}


