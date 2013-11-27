/*
 * @file ExtentTask.cpp
 * @brief  for bucket extent
 *
 * @version 1.0
 * @date Mon Jul 16 10:01:14 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "ExtentTask.h"

#include <assert.h>

#include <list>
#include <memory>

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MURegister.h"
#include "frame/MUTaskManager.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "state/RuleManager.h"
#include "item/ExtentItem.h"
#include "oplog/LogDispatcher.h"

ExtentTask::~ExtentTask()
{
    if (TASK_FINISH != m_CurrentState) {
        INFO_LOG("do bucket extent failed");
    }

    // open big lock when finished
    if (!MURegister::getInstance()->bigLock()) {
        openBigLock();
    }

    if (NULL != m_pTimer) {
        TRACE_LOG("m_pTimer != NULL");
        recycle(m_pTimer);
        m_pTimer = NULL;
    }
}

ExtentTask::ExtentTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_pTimer = NULL;
    m_Failed = false;
}

void
ExtentTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    ERROR_LOG("extent task, timer error");

    m_CurrentState = TASK_ERROR;
    recycle();
}

int
ExtentTask::start()
{
    INFO_LOG("try to do bucket extent");

    int rt = 0;

    // set big lock to prevent new requests from dispatching to thread pool
    MURegister::getInstance()->setBigLock(true);

    // wait current requests to complete
    Epoll *pEpoll = MURegister::getInstance()->getEpoll();
    m_pTimer = new MUTimer(pEpoll, this);

    rt = m_pTimer->create();

    if (-1 == rt) {
        DEBUG_LOG("create timer failed");
        return -1;
    }

    rt = m_pTimer->setTime(
             REQUEST_QUEUE_CHECK_PERIOD,
             REQUEST_QUEUE_CHECK_PERIOD);

    if (-1 == rt) {
        DEBUG_LOG("set time of timer failed");
        return -1;
    }

    INFO_LOG("wait received client requests to be done");

    m_CurrentState = TASK_PREPARE;

    return 0;
}

int
ExtentTask::checkRequestQueue()
{
    MUWorkItemManager *pItemManager =
        MURegister::getInstance()->getCurrentItemManager();

    if (pItemManager->noItems()) {
        return noRequests();
    }

    // still have some requests to do
    INFO_LOG("still have some client requests to do");

    return 0;
}

int
ExtentTask::noRequests()
{
    // close timer
    m_pTimer->disarm();

    m_CurrentState = TASK_PREPARE_COMPLETED;

    INFO_LOG("all received client requests have done");

    return extent();
}

int
ExtentTask::extent()
{
    // mod nr now
    uint64_t nowModNr = RuleManager::getInstance()->getModNr();

    // update rule
    if (nowModNr != m_ModNr) {
        RuleManager::getInstance()->setModNr(m_ModNr);
    }

    uint64_t lastModNr = RuleManager::getInstance()->getLastModNr();
    uint64_t lastTotalBuckets =
        RuleManager::getInstance()->getLastTotalBuckets();
    int modDiff = m_ModNr - lastModNr;
    int newBuckets = ::pow(2, modDiff) - 1;
    uint64_t firstNewBuckets = 0;

    if (lastModNr == m_ModNr) {
        // do not need to extent
        INFO_LOG("need not to do bucket extent, now mod %" PRIu64
                 ", new mod %" PRIu64,
                 lastModNr, m_ModNr);

        m_CurrentState = TASK_FINISH;

        if (NULL != m_pParent) {
            m_pParent->next(this);
            m_pParent = NULL;
        }

        // open system big lock
        openBigLock();

        INFO_LOG("do bucket extent successfully");

        recycle();
        return 0;
    }

    // get all buckets
    BucketManager::getInstance()->getAllBuckets(&m_BucketList);

    Bucket *pBucket = NULL;

    if (nowModNr == m_ModNr) {
        // a redo-task, remove succeeded bucket
        for (std::list<uint64_t>::iterator it = m_BucketList.begin();
             it != m_BucketList.end();) {
            if (*it >= lastTotalBuckets) {
                // new buckets
                ++it;
                continue;
            }

            firstNewBuckets = *it + lastTotalBuckets;

            if (inBucketList(firstNewBuckets, m_BucketList)) {
                for (int i = 0; i < newBuckets; ++i) {
                    m_BucketList.remove(*it + lastTotalBuckets * (i + 1));
                }

                m_BucketList.erase(it++);
                continue;
            }

            ++it;
        }
    }

    if (m_BucketList.size() == 0) {
        // a redo-task and no failed bucket, or no buckets on this MU
        m_CurrentState = TASK_FINISH;

        if (NULL != m_pParent) {
            m_pParent->next(this);
            m_pParent = NULL;
        }

        // open system big lock
        openBigLock();

        INFO_LOG("do bucket extent successfully");

        recycle();
        return 0;
    }

    // a redo-task and have some failed buckets, or not a redo-task

    ExtentItem *pItem = NULL;

    for (std::list<uint64_t>::iterator it = m_BucketList.begin();
         it != m_BucketList.end();) {
        pItem = new ExtentItem(this);
        pItem->setItemType(EXTENT_ITEM);
        pItem->setWorkType(ExtentItem::EXTENT_BUCKET);
        pItem->setBucketId(*it);
        pItem->setNewModNr(m_ModNr);
        pItem->setLastModNr(lastModNr);

        pItem->postRequest();

        // !!! add to ref cnt so we can still do something
        // even after this task was put into recycle list
        ref();

        ++it;
    }

    m_CurrentState = TASK_EXTENT;

    INFO_LOG("now doing bucket extent, now mod number %" PRIu64 ", "
             "new mod number %" PRIu64,
             lastModNr,
             m_ModNr);

    return 0;
}

int
ExtentTask::dispatch(MUWorkItem *pItem)
{
    switch (pItem->getItemType()) {

    case EXTENT_ITEM: {
            return dispatchExtentItem(pItem);
            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unexpected item type %" PRIi32, pItem->getItemType());
            assert(0);

            return -1;
            break;
        }

    }
}

int
ExtentTask::dispatchExtentItem(MUWorkItem *pItem)
{
    switch (pItem->getWorkType()) {

    case ExtentItem::EXTENT_BUCKET: {
            return extent(pItem);
            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unexpected work type %" PRIi32, pItem->getWorkType());
            assert(0);

            return -1;
            break;
        }

    }
}

int
ExtentTask::next(MUWorkItem *pItem)
{
    release();  // !!!

    int rt = 0;

    switch (m_CurrentState) {

    case TASK_EXTENT:
    case TASK_ERROR: {
            // try to complete bucket-extent task even something goes wrong
            rt = dispatch(pItem);

            if (-1 == rt) {
                m_CurrentState = TASK_ERROR;
                recycle();
                return -1;
            }

            break;
        }

    default: {
            DEBUG_LOG("unexpected task state %" PRIi32, m_CurrentState);

            delete pItem;
            pItem = NULL;

            m_CurrentState = TASK_ERROR;
            recycle();

            return -1;
            break;
        }

    }

    return 0;
}

int
ExtentTask::extent(MUWorkItem *pItem)
{
    std::auto_ptr<ExtentItem> pRItem =
        std::auto_ptr<ExtentItem>(dynamic_cast<ExtentItem *>(pItem));

    Bucket *pBucket = NULL;
    uint64_t bucketId = pRItem->getBucketId();
    pBucket = BucketManager::getInstance()->get(bucketId);

    // extent bucket 'bucketId' finish
    m_BucketList.remove(bucketId);

    ReturnStatus rs;

    rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        INFO_LOG("extent bucket %" PRIu64 " failed", bucketId);
        m_Failed = true;

        if (0 != m_BucketList.size()) {
            return 0;  // wait all items to finish

        } else {
            openBigLock();

            return -1;
        }
    }

    // rotate log
    LogDispatcher *pLD = MURegister::getInstance()->getLogDispatcher();
    pLD->rotateLog(bucketId);

    // send pending request

    std::list<uint64_t> itemList;
    MUWorkItem *pNItem = NULL;
    MUWorkItemManager *pIM = pBucket->m_pItemManager;

    pIM->getAllItems(&itemList);

    for (std::list<uint64_t>::iterator it = itemList.begin();
         it != itemList.end(); ++it) {
        pNItem = pIM->get(*it);
        pIM->remove(*it);

        if (NULL != pNItem) {
            pNItem->postRequest();
        }
    }

    // for new buckets

    const std::list<uint64_t> &newBucketList =
        pRItem->getNewBucketList();

    for (std::list<uint64_t>::const_iterator it = newBucketList.begin();
         it != newBucketList.end(); ++it) {
        Bucket *pNBucket = new Bucket();
        pNBucket->m_BucketState = MU_BUCKET_MASTER;
        pNBucket->m_BucketId = *it;
        pNBucket->m_LogSeq = pBucket->m_LogSeq;

        BucketManager::getInstance()->put(*it, pNBucket);

        pLD->rotateLog(*it);
    }

    INFO_LOG("extent bucket %" PRIu64 " successfully", bucketId);

    // extent finish?

    if (m_BucketList.size() == 0) {
        openBigLock();

        if (m_Failed) {
            ERROR_LOG("do bucket extent failed");
            return -1;
        }

        m_CurrentState = TASK_FINISH;

        if (NULL != m_pParent) {
            m_pParent->next(this);
            m_pParent = NULL;
        }


        INFO_LOG("do bucket extent successfully");

        recycle();
        return 0;
    }
}

void
ExtentTask::openBigLock()
{
    MUWorkItemManager *pIM = MURegister::getInstance()->getDelayItemManager();

    std::list<uint64_t> itemList;

    pIM->getAllItems(&itemList);

    MUWorkItem *pItem = NULL;

    for (std::list<uint64_t>::iterator it = itemList.begin();
         it != itemList.end(); ++it) {
        pItem = pIM->get(*it);
        pIM->remove(*it);

        if (NULL != pItem) {
            pItem->postRequest();
        }
    }

    Bucket *pBucket = NULL;
    std::list<Bucket *> bucketList;
    BucketManager::getInstance()->getAllBuckets(&bucketList);

    for (std::list<Bucket *>::iterator it = bucketList.begin();
         it != bucketList.end(); ++it) {
        pBucket = *it;
        pIM = pBucket->m_pItemManager;
        itemList.clear();
        pIM->getAllItems(&itemList);

        for (std::list<uint64_t>::iterator nit = itemList.begin();
             nit != itemList.end(); ++nit) {
            pItem = pIM->get(*nit);
            pIM->remove(*nit);

            if (NULL != pItem) {
                pItem->postRequest();
            }
        }
    }

    MURegister::getInstance()->setBigLock(false);
}

int
ExtentTask::next(MUTimer *pTimer, uint64_t times)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_PREPARE: {
            rt = checkRequestQueue();

            if (-1 == rt) {
                m_CurrentState = TASK_ERROR;
                recycle();
                return -1;
            }
        }

    default: {
            // should not reach here
            DEBUG_LOG("have prepared, no timer should open");

            m_CurrentState = TASK_ERROR;

            return -1;
            break;
        }

    }

    return 0;
}

bool
ExtentTask::inBucketList(
    uint64_t bucketId,
    const std::list<uint64_t> &bucketList)
{
    for (std::list<uint64_t>::const_iterator it = bucketList.begin();
         it != bucketList.end(); ++it) {
        if (bucketId == *it) {
            return true;
        }
    }

    return false;
}


