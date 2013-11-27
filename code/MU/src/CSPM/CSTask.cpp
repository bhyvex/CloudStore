/*
 * @file CSTask.cpp
 * @brief Process CS commands.
 *
 * @version 1.0
 * @date Wed Jul 11 14:19:31 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "CSTask.h"

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <string>
#include <memory>

#include "frame/MUTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"
#include "frame/MURegister.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "frame/MUTaskManager.h"
#include "protocol/protocol.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "item/BucketItem.h"

#include "comm/comm.h"
#include "util/util.h"
#include "log/log.h"

#include "MigrationClientTask.h"

#define IP_ADDRESS_STR_LEN  16

CSTask::~CSTask()
{
    if (NULL != m_pMigrationTask) {
        DEBUG_LOG("m_pMigrationTask != NULL");
        recycle(m_pMigrationTask);
        m_pMigrationTask = NULL;
    }

    if (NULL != m_pExtentTask) {
        DEBUG_LOG("m_pExtentTask != NULL");
        recycle(m_pExtentTask);
        m_pExtentTask = NULL;
    }

    if (NULL != m_pStartBucketTask) {
        DEBUG_LOG("m_pStartBucketTask != NULL");
        recycle(m_pStartBucketTask);
        m_pStartBucketTask = NULL;
    }

    if (NULL != m_pStopBucketTask) {
        DEBUG_LOG("m_pStopBucketTask != NULL");
        recycle(m_pStopBucketTask);
        m_pStopBucketTask = NULL;
    }
}

CSTask::CSTask(MUTask *pParent) :
    PassiveTask(pParent)
{
    m_CurrentState = TASK_INIT;

    m_pMigrationTask = NULL;
    m_pExtentTask = NULL;
    m_pStartBucketTask = NULL;
    m_pStopBucketTask = NULL;
}

void
CSTask::destroy(MUTCPAgent *pChannel)
{
    m_pOwner = NULL;

    DEBUG_LOG("cs task, agent error");

    recycle();
}

void
CSTask::destroy(MUTask *pChannel)
{
    if (m_pMigrationTask == pChannel) {
        destroyMigrationTask(pChannel);

    } else if (m_pExtentTask == pChannel) {
        destroyExtentTask(pChannel);

    } else if (m_pStartBucketTask == pChannel) {
        destroyStartBucketTask(pChannel);

    } else if (m_pStopBucketTask == pChannel) {
        destroyStopBucketTask(pChannel);

    } else {
        // never reach here
        assert(0);
    }
}

int
CSTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_INIT: {
            m_CurrentState = TASK_WORKING;
            return next(pAgent, req);

            break;
        }

    case TASK_WORKING:
    case TASK_START_BUCKET_COMPLETED:
    case TASK_STOP_BUCKET_COMPLETED:
    case TASK_MIGRATION_COMPLETED:
    case TASK_EXTENT_COMPLETED: {
            //case TASK_EXTENT_PREPARED: {
            rt = dispatch(pAgent, req);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    case TASK_START_BUCKET:
    case TASK_STOP_BUCKET:
    case TASK_MIGRATION_STARTED:
    case TASK_EXTENT_STARTED: {
            // should not reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);

            recycle();
            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);

            recycle();
            break;
        }

    }

    return 0;
}

int
CSTask::dispatch(MUTCPAgent *pAgent, const InReq &req)
{
    switch (req.m_msgHeader.cmd) {

    case MSG_CS_MU_SET_BUCKET_STATE: {
            return setBucketState(req);
            break;
        }

    case MSG_CS_MU_DELETE_BUCKET: {
            return deleteBucket(req);
            break;
        }

    case MSG_CS_MU_MIGRATE_BUCKET: {
            return migrateBucket(req);
            break;
        }

        //case MSG_CS_MU_EXTENT_BUCKET_PREPARE: {
        //return prepareExtentBucket(req);
        //break;
        //}

    case MSG_CS_MU_EXTENT_BUCKET: {
            return extentBucket(req);
            break;
        }

    default: {
            // should not reach here
            DEBUG_LOG("unknown protocol command, %d.",
                      req.m_msgHeader.cmd);

            return -1;
            break;
        }

    }

    return 0;
}

int
CSTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    switch (pItem->getItemType()) {

    case BUCKET_ITEM: {
            rt = dispatchBucketItem(pItem);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    default: {
            DEBUG_LOG("unexpected item type %d.", pItem->getItemType());
            assert(0);
            recycle();
            break;
        }

    }

    return 0;
}

int
CSTask::next(MUTask *pTask)
{
    int rt = 0;

    if (m_pMigrationTask == pTask) {
        rt = completeMigration(pTask);

        if (-1 == rt) {
            recycle();
            return -1;
        }

    } else if (m_pExtentTask == pTask) {
        rt = completeExtent(pTask);

        if (-1 == rt) {
            recycle();
            return -1;
        }

    } else if (m_pStartBucketTask == pTask) {
        rt = completeStartBucket(pTask);

        if (-1 == rt) {
            recycle();
            return -1;
        }

    } else if (m_pStopBucketTask == pTask) {
        rt = completeStopBucket(pTask);

        if (-1 == rt) {
            recycle();
            return -1;
        }
    }

    return 0;
}

void
CSTask::errorResponse(uint32_t cmd)
{
    simpleResponse(cmd, MU_UNKNOWN_ERROR);
}

void
CSTask::simpleResponse(uint32_t cmd, uint32_t error)
{
    MsgHeader msg;

    msg.cmd = cmd;
    msg.error = error;
    msg.length = 0;

    m_pOwner->sendPacket(msg, NULL);
}

int
CSTask::setBucketState(const InReq &req)
{
    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_CS_MU_SET_BUCKET_STATE setBucketState;

    if (!setBucketState.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse failed");
        errorResponse(MSG_CS_MU_SET_BUCKET_STATE_ACK);
        return 0;
    }

    uint64_t bucketId = setBucketState.bucket_index();
    int bucketState = setBucketState.state();

    Bucket *pBucket = BucketManager::getInstance()->get(bucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("no such bucket %ld", bucketId);
        errorResponse(MSG_CS_MU_SET_BUCKET_STATE_ACK);
        return 0;
    }

    if (MU_BUCKET_START == bucketState) {
        return startBucket(bucketId);

    } else if (MU_BUCKET_STOP == bucketState) {
        return stopBucket(bucketId);

    } else {
        pBucket->m_BucketState = bucketState;
        simpleResponse(MSG_CS_MU_SET_BUCKET_STATE_ACK, MU_OK);
    }

    return 0;
}

int
CSTask::deleteBucket(const InReq &req)
{
    DEBUG_LOG("delete bucket start");

    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_CS_MU_DELETE_BUCKET deleteBucket;

    if (!deleteBucket.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse failed");
        errorResponse(MSG_CS_MU_DELETE_BUCKET_ACK);
        return 0;
    }

    uint64_t bucketId = deleteBucket.bucket_index();

    Bucket *pBucket = BucketManager::getInstance()->get(bucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("no such bucket %ld", bucketId);
        errorResponse(MSG_CS_MU_DELETE_BUCKET_ACK);
        return 0;
    }

    if (MU_BUCKET_STOP != pBucket->m_BucketState) {
        DEBUG_LOG("Bucket %ld busy.", bucketId);
        simpleResponse(MSG_CS_MU_DELETE_BUCKET_ACK, MU_BUCKET_BUSY);
        return 0;
    }

    BucketItem *pItem = new BucketItem(this);

    pItem->setItemType(BUCKET_ITEM);
    pItem->setWorkType(BucketItem::DELETE_BUCKET);
    pItem->setBucketId(bucketId);

    pItem->postRequest();

    return 0;
}

int
CSTask::migrateBucket(const InReq &req)
{
    DEBUG_LOG("migrate bucket start");

    int rt = 0;

    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_CS_MU_MIGRATE_BUCKET migrateBucket;

    if (!migrateBucket.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse failed");
        errorResponse(MSG_CS_MU_MIGRATE_BUCKET_ACK);
        return 0;
    }

    uint64_t bucketId = migrateBucket.bucket_index();

    Bucket *pBucket = BucketManager::getInstance()->get(bucketId);

    if (NULL != pBucket) {
        ERROR_LOG("bucket %ld already exists", bucketId);
        simpleResponse(MSG_CS_MU_MIGRATE_BUCKET_ACK, MU_BUCKET_EXIST);
        return 0;
    }

    uint32_t srcIp = migrateBucket.source_mu();

    char strIp[IP_ADDRESS_STR_LEN];
    memset(strIp, 0, IP_ADDRESS_STR_LEN);

    struct in_addr addr;
    addr.s_addr = srcIp;

    const char *pRc = NULL;
    pRc = inet_ntop(AF_INET, &addr, strIp, IP_ADDRESS_STR_LEN);

    if (NULL == pRc) {
        DEBUG_LOG("inet_ntop() error, %s", strerror(errno));
        return -1;
    }

    m_pMigrationTask =
        MUTaskManager::getInstance()->createTask<MigrationClientTask>(this);
    m_pMigrationTask->setBucketId(bucketId);
    m_pMigrationTask->setSrcMUIP(strIp);

    rt = m_pMigrationTask->start();

    if (-1 == rt) {
        recycle(m_pMigrationTask);
        m_pMigrationTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_MIGRATION_STARTED;
    return 0;
}

int
CSTask::prepareExtentBucket(const InReq &req)
{
    DEBUG_LOG("extent bucket prepare start");

    int rt = 0;

    m_pExtentTask =
        MUTaskManager::getInstance()->createTask<ExtentTask>(this);

    rt = m_pExtentTask->start();

    if (-1 == rt) {
        recycle(m_pExtentTask);
        m_pExtentTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_EXTENT_STARTED;

    return 0;
}

int
CSTask::extentBucket(const InReq &req)
{
    DEBUG_LOG("extent bucket start");

    int rt = 0;

    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_CS_MU_EXTENT_BUCKET extent;

    if (!extent.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse failed.");
        errorResponse(MSG_CS_MU_EXTENT_BUCKET_ACK);
        return 0;
    }

    m_pExtentTask =
        MUTaskManager::getInstance()->createTask<ExtentTask>(this);

    uint64_t modNr = extent.new_mod();
    m_pExtentTask->setModNr(modNr);
    DEBUG_LOG("new mod nr %llu", modNr);

    rt = m_pExtentTask->start();

    if (-1 == rt) {
        recycle(m_pExtentTask);
        m_pExtentTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_EXTENT_STARTED;

    return 0;
}

int
CSTask::dispatch(MUWorkItem *pItem)
{
    switch (pItem->getItemType()) {

    case BUCKET_ITEM: {
            return dispatchBucketItem(pItem);
            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unknown item type %d.", pItem->getItemType());
            assert(0);
            return -1;
            break;
        }

    }

    return 0;
}

int
CSTask::dispatchBucketItem(MUWorkItem *pItem)
{
    switch (pItem->getWorkType()) {

    case BucketItem::DELETE_BUCKET: {
            return deleteBucket(pItem);
            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unknown work type %d.", pItem->getWorkType());
            assert(0);
            return -1;
            break;
        }

    }

    return 0;
}

int
CSTask::deleteBucket(MUWorkItem *pItem)
{
    std::auto_ptr<BucketItem> pRItem =
        std::auto_ptr<BucketItem>(dynamic_cast<BucketItem *>(pItem));

    ReturnStatus rs;

    rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        errorResponse(MSG_CS_MU_DELETE_BUCKET_ACK);
        return 0;
    }

    uint64_t bucketId = pRItem->getBucketId();

    Bucket *pBucket = BucketManager::getInstance()->get(bucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("delete bucket error");
        DEBUG_LOG("no such bucket %ld.", bucketId);
        errorResponse(MSG_CS_MU_DELETE_BUCKET_ACK);
        return 0;
    }

    BucketManager::getInstance()->remove(bucketId);
    delete pBucket;

    simpleResponse(MSG_CS_MU_DELETE_BUCKET_ACK, MU_OK);

    DEBUG_LOG("delete bucket complete");

    return 0;
}

void
CSTask::destroyMigrationTask(MUTask *pTask)
{
    m_pMigrationTask = NULL;

    ERROR_LOG("cs task, migration task error");

    simpleResponse(MSG_CS_MU_MIGRATE_BUCKET_ACK, MU_UNKNOWN_ERROR);
    closeAgent(m_pOwner);
    m_pOwner = NULL;

    recycle();
}

int
CSTask::completeMigration(MUTask *pTask)
{
    DEBUG_LOG("migration complete");

    simpleResponse(MSG_CS_MU_MIGRATE_BUCKET_ACK, MU_OK);
    m_CurrentState = TASK_MIGRATION_COMPLETED;
    m_pMigrationTask = NULL;  // do not need it any more

    return 0;
}

int
CSTask::startBucket(uint64_t bucketId)
{
    DEBUG_LOG("try to start bucket");

    int rt = 0;

    m_pStartBucketTask =
        MUTaskManager::getInstance()->createTask<StartBucketTask>(this);

    m_pStartBucketTask->setBucketId(bucketId);

    rt = m_pStartBucketTask->start();

    if (-1 == rt) {
        recycle(m_pStartBucketTask);
        m_pStartBucketTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_START_BUCKET;

    return 0;
}


int
CSTask::completeStartBucket(MUTask *pTask)
{
    DEBUG_LOG("start bucket complete");
    simpleResponse(MSG_CS_MU_SET_BUCKET_STATE_ACK, MU_OK);
    m_CurrentState = TASK_START_BUCKET_COMPLETED;
    m_pStartBucketTask = NULL;

    return 0;
}

void
CSTask::destroyStartBucketTask(MUTask *pTask)
{
    m_pStartBucketTask = NULL;

    ERROR_LOG("cs task, start bucket task error");
    recycle();
}

int
CSTask::stopBucket(uint64_t bucketId)
{
    DEBUG_LOG("try to stop bucket");

    int rt = 0;

    m_pStopBucketTask =
        MUTaskManager::getInstance()->createTask<StopBucketTask>(this);

    m_pStopBucketTask->setBucketId(bucketId);

    rt = m_pStopBucketTask->start();

    if (-1 == rt) {
        recycle(m_pStopBucketTask);
        m_pStopBucketTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_STOP_BUCKET;

    return 0;
}

void
CSTask::destroyStopBucketTask(MUTask *pTask)
{
    m_pStopBucketTask = NULL;

    ERROR_LOG("cs task, stop bucket error");
    recycle();
}

int
CSTask::completeStopBucket(MUTask *pTask)
{
    TRACE_LOG("stop bucket complete");

    simpleResponse(MSG_CS_MU_SET_BUCKET_STATE_ACK, MU_OK);
    m_CurrentState = TASK_STOP_BUCKET_COMPLETED;
    m_pStopBucketTask = NULL;

    return 0;
}

int
CSTask::completeExtent(MUTask *pTask)
{
    switch (m_CurrentState) {

    case TASK_EXTENT_STARTED: {
            DEBUG_LOG("extent bucket complete");
            sendSimplePacket(m_pOwner, MSG_CS_MU_EXTENT_BUCKET_ACK,
                             MU_OK);
            m_CurrentState = TASK_EXTENT_COMPLETED;
            m_pExtentTask = NULL;  // do not need it any more

            break;
        }

    default: {
            // should not reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);
            return -1;
            break;
        }

    }

    return 0;
}

void
CSTask::destroyExtentTask(MUTask *pTask)
{
    m_pExtentTask = NULL;

    ERROR_LOG("cs task, extent task error");
    recycle();
}


