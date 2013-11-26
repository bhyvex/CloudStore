/*
 * @file StartBucketTask.cpp
 * @brief  start a bucket
 *
 * @version 1.0
 * @date Sun Jul 15 16:57:08 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "StartBucketTask.h"

#include <assert.h>

#include "frame/MUTask.h"
#include "frame/MUWorkItem.h"
#include "frame/MUTaskManager.h"
#include "item/BucketItem.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"

#include "log/log.h"

StartBucketTask::~StartBucketTask()
{
    if (TASK_FINISH != m_CurrentState) {
        DEBUG_LOG("start bucket %llu failed", m_BucketId);
    }
}

StartBucketTask::StartBucketTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_BucketId = 0;
}

int
StartBucketTask::start()
{
    switch (m_CurrentState) {

    case TASK_INIT: {
            return startBucket();
            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}

int
StartBucketTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    if (pItem->getItemType() == BUCKET_ITEM) {
        rt = dispatchBucketItem(pItem);

        if (-1 == rt) {
            recycle();
            return -1;
        }

    } else {
        // never going here
        DEBUG_LOG("unexpected item type %d", pItem->getItemType());
        assert(0);

        return -1;
    }

    return 0;
}

int
StartBucketTask::dispatchBucketItem(MUWorkItem *pItem)
{
    switch (pItem->getWorkType()) {

    case BucketItem::START_BUCKET: {
            return startBucket(pItem);
            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected work type %d.", pItem->getWorkType());
            assert(0);
            break;
        }

    }

    return 0;
}

int
StartBucketTask::startBucket()
{
    DEBUG_LOG("try to start bucket %llu", m_BucketId);

    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL == pBucket) {
        pBucket = new Bucket();

        pBucket->m_BucketId = m_BucketId;
        pBucket->m_BucketState = MU_BUCKET_START;

        BucketManager::getInstance()->put(pBucket->m_BucketId, pBucket);
    }

    BucketItem *pItem = new BucketItem(this);

    pItem->setItemType(BUCKET_ITEM);
    pItem->setWorkType(BucketItem::START_BUCKET);
    pItem->setBucketId(m_BucketId);

    pItem->postRequest();

    m_CurrentState = TASK_READ_BUCKET_INFO;

    return 0;
}

int
StartBucketTask::startBucket(MUWorkItem *pItem)
{
    std::auto_ptr<BucketItem> pRItem =
        std::auto_ptr<BucketItem>(dynamic_cast<BucketItem *>(pItem));

    ReturnStatus rs;

    rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        DEBUG_LOG("start bucket %ld failed", pRItem->getBucketId());
        return -1;
    }

    Bucket *pBucket = BucketManager::getInstance()->get(pRItem->getBucketId());

    if (NULL == pBucket) {
        pBucket = new Bucket();

        pBucket->m_BucketId = pRItem->getBucketId();
        pBucket->m_LogSeq = pRItem->getLogSeqNr();
        pBucket->m_BucketState = MU_BUCKET_MASTER;

        BucketManager::getInstance()->put(pBucket->m_BucketId, pBucket);

    } else if (MU_BUCKET_STOP == pBucket->m_BucketState
            || MU_BUCKET_START == pBucket->m_BucketState) {
        pBucket->m_LogSeq = pRItem->getLogSeqNr();
        pBucket->m_BucketState = MU_BUCKET_MASTER;
    }

    DEBUG_LOG("start bucket %" PRIu64 " successfully", m_BucketId);

    if (NULL != m_pParent) {
        m_pParent->next(this);
        m_pParent = NULL;
    }

    m_CurrentState = TASK_READ_BUCKET_INFO_COMPLETED;

    // to do

    m_CurrentState = TASK_FINISH;

    recycle();

    return 0;
}


