/*
 * @file MigrationServerTask.cpp
 * @brief Server task for data migration.
 *
 * @version 1.0
 * @date Mon Jul 16 01:17:32 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MigrationServerTask.h"

#include "frame/PassiveTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"
#include "protocol/protocol.h"
#include "item/MigrationServerItem.h"
#include "item/BucketItem.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"

#include "log/log.h"

#include <string>

MigrationServerTask::~MigrationServerTask()
{
    if (TASK_FINISH != m_CurrentState) {
        ERROR_LOG("migrate bucket %" PRIu64 " failed", m_BucketId);
    }

    if (NULL != m_pItem) {
        delete m_pItem;
        m_pItem = NULL;
    }

    if (NULL != m_pStopBucketTask) {
        recycle(m_pStopBucketTask);
        m_pStopBucketTask = NULL;
    }

    if (NULL != m_pStartBucketTask) {
        recycle(m_pStartBucketTask);
        m_pStartBucketTask = NULL;
    }
}

MigrationServerTask::MigrationServerTask(MUTask *pParent) :
    PassiveTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_BucketId = 0;
    m_pItem = NULL;
    m_pStopBucketTask = NULL;
    m_pStartBucketTask = NULL;
}

void
MigrationServerTask::destroy(MUTCPAgent *pChannel)
{
    m_pOwner = NULL;

    switch (m_CurrentState) {

    case TASK_INIT: {
            recycle();
            break;
        }

    case TASK_STOP_BUCKET: {
            m_CurrentState = TASK_ERROR;
            break;
        }

    case TASK_STOP_BUCKET_COMPLETED:
    case TASK_TAR_BUCKET:
    case TASK_TAR_BUCKET_COMPLETED:
    case TASK_MIGRATE_DATA:
    case TASK_MIGRATE_DATA_COMPLETED: {
            recovery();
            break;
        }

    case TASK_FINISH:
    case TASK_ERROR: {
            break;
        }

    default: {
            DEBUG_LOG("unexpected task state %" PRIi32, m_CurrentState);

            assert(0);
            recycle();

            break;
        }

    }
}

void
MigrationServerTask::destroy(MUTask *pTask)
{
    if (pTask == m_pStopBucketTask) {
        destroyStopBucketTask(pTask);

    } else if (pTask == m_pStartBucketTask) {
        destroyStartBucketTask(pTask);
    }
}

void
MigrationServerTask::destroyStopBucketTask(MUTask *pTask)
{
    m_pStopBucketTask = NULL;

    ERROR_LOG("try to stop bucket %" PRIu64 " failed", m_BucketId);

    recycle();
}

void
MigrationServerTask::destroyStartBucketTask(MUTask *pTask)
{
    m_pStartBucketTask = NULL;

    ERROR_LOG("try to start bucket %" PRIu64 " failed", m_BucketId);

    recycle();
}

int
MigrationServerTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_INIT: {
            rt = handshake(req);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    case TASK_MIGRATE_DATA: {
            rt = migrate(req);

            if (-1 == rt) {
                recovery();
                //recycle();
                return -1;
            }

            break;
        }

    case TASK_MIGRATE_DATA_COMPLETED: {
            rt = deleteBucket(req);

            if (-1 == rt) {
                //recycle();
                recovery();
                return -1;
            }

            break;
        }

    default: {
            // never reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);

            assert(0);
            recycle();

            return -1;
            break;
        }

    }

    return 0;
}

int
MigrationServerTask::next(MUTask *pTask)
{
    int rt = 0;

    if (pTask == m_pStopBucketTask) {
        rt = stopBucket(pTask);

        if (-1 == rt) {
            recovery();
        }

    } else if (pTask == m_pStartBucketTask) {
        rt = startBucket(pTask);

        if (-1 == rt) {
            recycle();
        }

    } else {
        DEBUG_LOG("unexpeced task pointer");
        recycle();
    }

    return 0;
}

int
MigrationServerTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    if (MIGRATION_SERVER_ITEM == pItem->getItemType()) {
        rt = dispatchMigrationServerItem(pItem);

        if (-1 == rt) {
            //recycle();
            recovery();
            return -1;
        }

    }  else {
        // never reach here
        DEBUG_LOG("unexpected item type %" PRIi32, pItem->getItemType());

        assert(0);
        recycle();

        return -1;
    }

    return 0;
}

int
MigrationServerTask::dispatchMigrationServerItem(MUWorkItem *pItem)
{
    m_pItem = dynamic_cast<MigrationServerItem *>(pItem);

    switch (pItem->getWorkType()) {

    case MigrationServerItem::TAR_BUCKET_DATA: {
            return tarBucket(pItem);
            break;
        }

    case MigrationServerItem::READ_BUCKET_DATA: {
            return migrate(pItem);
            break;
        }

    default: {
            DEBUG_LOG("unexpect work type %" PRIi32, pItem->getWorkType());

            return -1;
            break;
        }

    }
}


int
MigrationServerTask::handshake(const InReq &req)
{
    if (MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command 0x%x.",
                  req.m_msgHeader.cmd);
        return -1;
    }

    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE handshake;

    if (!handshake.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse error");

        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK,
            MU_UNKNOWN_ERROR);

        return 0;
    }

    m_BucketId = handshake.bucket_index();

    INFO_LOG("start migrating bucket %" PRIu64, m_BucketId);

    Bucket *pBucket =
        BucketManager::getInstance()->get(m_BucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("no such bucket %" PRIu64, m_BucketId);

        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK,
            MU_UNKNOWN_ERROR);

        return 0;
    }

    return stopBucket();
}

int
MigrationServerTask::migrate(const InReq &req)
{
    if (MSG_MU_MU_MIGRATE_BUCKET_DATA != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command 0x%x.",
                  req.m_msgHeader.cmd);
        return -1;
    }

    m_pItem->setWorkType(MigrationServerItem::READ_BUCKET_DATA);
    m_pItem->postRequest();
    m_pItem = NULL;

    return 0;
}

int
MigrationServerTask::migrate(MUWorkItem *pItem)
{
    ReturnStatus rs;

    rs = m_pItem->getReturnStatus();

    if (!rs.success()) {
        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_DATA_ACK,
            MU_UNKNOWN_ERROR);

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_MU_MU_MIGRATE_BUCKET_DATA_ACK;

    if (m_pItem->moreData()) {
        msg.para1 = MU_MORE_DATA;
        TRACE_LOG("still have more data");

    } else {
        msg.para1 = MU_NO_MORE_DATA;

        m_CurrentState = TASK_MIGRATE_DATA_COMPLETED;

        TRACE_LOG("no more data now");
    }

    const std::string &data = m_pItem->getBucketData();

    msg.length = data.length();

    m_pOwner->sendPacket(msg, data.c_str());

    if (!m_pItem->moreData()) {
        m_CurrentState = TASK_FINISH;

        delete m_pItem;
        m_pItem = NULL;
    }

    return 0;
}

int
MigrationServerTask::stopBucket()
{
    int rt = 0;

    m_pStopBucketTask = new StopBucketTask(this);
    m_pStopBucketTask->setBucketId(m_BucketId);

    rt = m_pStopBucketTask->start();

    if (-1 == rt) {
        INFO_LOG("try to stop bucket %" PRIu64 " failed", m_BucketId);

        recycle(m_pStopBucketTask);
        m_pStopBucketTask = NULL;

        return -1;
    }

    m_CurrentState = TASK_STOP_BUCKET;

    return 0;
}

int
MigrationServerTask::stopBucket(MUTask *pTask)
{
    if (TASK_ERROR == m_CurrentState) {
        return -1;
    }

    // stop bucket successfully

    m_pStopBucketTask->setParent(NULL);
    m_pStopBucketTask = NULL;

    m_CurrentState = TASK_STOP_BUCKET_COMPLETED;

    return tarBucket();
}

int
MigrationServerTask::tarBucket()
{
    m_pItem = new MigrationServerItem(this);

    m_pItem->setItemType(MIGRATION_SERVER_ITEM);
    m_pItem->setWorkType(MigrationServerItem::TAR_BUCKET_DATA);
    m_pItem->setBucketId(m_BucketId);

    m_pItem->postRequest();
    m_pItem = NULL;

    m_CurrentState = TASK_TAR_BUCKET;

    return 0;
}

int
MigrationServerTask::tarBucket(MUWorkItem *pItem)
{
    ReturnStatus rs;

    rs = m_pItem->getReturnStatus();

    if (!rs.success()) {
        INFO_LOG("try to tar bucket %" PRIu64 " failed", m_BucketId);

        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK,
            MU_UNKNOWN_ERROR);

        return 0;
    }

    Bucket *pBucket =
        BucketManager::getInstance()->get(m_BucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("no such bucket %" PRIu64, m_BucketId);

        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK,
            MU_UNKNOWN_ERROR);

        return 0;
    }

    MsgHeader msg;
    msg.cmd = MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK;

    cstore::pb_MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK ack;
    ack.set_seq_nr(pBucket->m_LogSeq);

    std::string data;

    if (!ack.SerializeToString(&data)) {

        DEBUG_LOG("protobuf serialize failed.");

        sendSimplePacket(
            m_pOwner, MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK,
            MU_UNKNOWN_ERROR);

        return -1;
    }

    msg.length = data.length();
    m_pOwner->sendPacket(msg, data.c_str());

    m_CurrentState = TASK_TAR_BUCKET_COMPLETED;

    return 0;
}

int
MigrationServerTask::deleteBucket(const InReq &req)
{
    if (MSG_CS_MU_DELETE_BUCKET != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command 0x%x",
                  req.m_msgHeader.cmd);
        return -1;
    }

    INFO_LOG("try to delete bucket %" PRIu64, m_BucketId);

    BucketItem *pItem = new BucketItem(this);

    pItem->setItemType(BUCKET_ITEM);
    pItem->setWorkType(BucketItem::DELETE_BUCKET);
    pItem->setBucketId(m_BucketId);

    pItem->postRequest();

    m_CurrentState = TASK_DELETE_BUCKET;

    return 0;
}

int
MigrationServerTask::deleteBucket(MUWorkItem *pItem)
{
    std::auto_ptr<BucketItem> pRItem =
        std::auto_ptr<BucketItem>(dynamic_cast<BucketItem *>(pItem));

    ReturnStatus rs;

    rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        INFO_LOG("delete bucket %" PRIu64 " failed", m_BucketId);
        return 0;
    }

    Bucket *pBucket = BucketManager::getInstance()->get(m_BucketId);

    if (NULL == pBucket) {
        DEBUG_LOG("no such bucket %" PRIu64, m_BucketId);
        return 0;
    }

    BucketManager::getInstance()->remove(m_BucketId);

    delete pBucket;
    pBucket = NULL;

    INFO_LOG("delete bucket %" PRIu64 " successfully", m_BucketId);

    INFO_LOG("migrate bucket %" PRIu64 " successfully", m_BucketId);

    m_CurrentState = TASK_FINISH;

    recycle();

    return 0;
}

int
MigrationServerTask::startBucket()
{
    m_pStartBucketTask = new StartBucketTask(this);
    m_pStartBucketTask->setBucketId(m_BucketId);

    int rt = 0;

    rt = m_pStartBucketTask->start();

    if (-1 == rt) {
        recycle(m_pStartBucketTask);
        m_pStartBucketTask = NULL;
        return -1;
    }

    m_CurrentState = TASK_RECOVERY;

    return 0;
}

int
MigrationServerTask::startBucket(MUTask *pTask)
{
    m_pStartBucketTask->setParent(NULL);

    m_CurrentState = TASK_RECOVERY_COMPLETED;
    INFO_LOG("recovery bucket %" PRIu64 " successfully",
             m_BucketId);

    recycle();

    return 0;
}

void
MigrationServerTask::recovery()
{
    INFO_LOG("migrate bucket %" PRIu64 " failed, try to recovery now",
             m_BucketId);

    int rt = 0;

    rt = startBucket();

    if (-1 == rt) {
        ERROR_LOG("try to start bucket %" PRIu64 " failed",
                  m_BucketId);
        recycle();
    }
}




