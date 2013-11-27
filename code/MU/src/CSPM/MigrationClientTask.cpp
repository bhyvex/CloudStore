/*
 * @file MigrationClientTask.cpp
 * @brief data migration client.
 *
 * @version 1.0
 * @date Thu Jul 12 15:51:43 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MigrationClientTask.h"

#include <inttypes.h>
#include <assert.h>

#include <string>
#include <memory>

#include "frame/MUTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUConfiguration.h"
#include "frame/MURegister.h"
#include "frame/MUTaskManager.h"
#include "frame/MUAgentManager.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "protocol/protocol.h"

#include "log/log.h"

MigrationClientTask::~MigrationClientTask()
{
    if (TASK_FINISH != m_CurrentState
        && TASK_DELETE_SRC_BUCKET != m_CurrentState
        && TASK_DELETE_SRC_BUCKET_COMPLETED != m_CurrentState) {
        INFO_LOG("migrate bucket %" PRIu64 " from %s failed",
                 m_BucketId, m_SrcMUIP.c_str());
    }

    if (NULL != m_pAgent) {
        DEBUG_LOG("m_pAgent != NULL");
        recycle(m_pAgent);
        m_pAgent = NULL;
    }

    if (NULL != m_pItem) {
        DEBUG_LOG("m_pItem != NULL");
        delete m_pItem;
        m_pItem = NULL;
    }

    if (NULL != m_pStartBucketTask) {
        DEBUG_LOG("m_pStartBucketTask != NULL");
        recycle(m_pStartBucketTask);
        m_pStartBucketTask = NULL;
    }
}

MigrationClientTask::MigrationClientTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;

    m_pAgent = NULL;
    m_pItem = NULL;
    m_pStartBucketTask = NULL;
    m_BucketId = 0;
    m_MoreData = false;
}

int
MigrationClientTask::start()
{
    DEBUG_LOG("try to migrate bucket %" PRIu64 " to myself", m_BucketId);

    int rt = 0;

    Epoll *pEpoll = MURegister::getInstance()->getEpoll();

    assert(pEpoll);

    //m_pAgent = new MUTCPAgent(pEpoll, this);
    m_pAgent = MUAgentManager::getInstance()->createAgent<MUTCPAgent>(pEpoll);
    m_pAgent->setTask(this);

    SocketAddress muAddr;
    muAddr.setAddress(
        m_SrcMUIP.c_str(),
        MUConfiguration::getInstance()->m_MigrationPort
    );

    rt = m_pAgent->init();

    if (-1 == rt) {
        DEBUG_LOG("init MUTCPAgent failed");

        recycle(m_pAgent);
        m_pAgent = NULL;

        return -1;
    }

    rt = m_pAgent->connect(muAddr);

    if (-1 == rt) {
        DEBUG_LOG("connect to src mu failed");

        recycle(m_pAgent);
        m_pAgent = NULL;

        return -1;
    }

    return 0;
}

void
MigrationClientTask::destroy(MUTCPAgent *pChannel)
{
    m_pAgent = NULL;

    if (TASK_FINISH == m_CurrentState
        || TASK_DELETE_SRC_BUCKET_COMPLETED == m_CurrentState) {

    } else if (TASK_DELETE_SRC_BUCKET == m_CurrentState) {
        WARN_LOG("agent error while delete bucket %" PRIu64 " on source mu",
                 m_BucketId);

    } else {
        ERROR_LOG("agent error while doing migration");
    }

    recycle();
}

void
MigrationClientTask::destroy(MUTask *pTask)
{
    m_pStartBucketTask = NULL;

    recycle();
}

int
MigrationClientTask::next(MUTask *pTask)
{
    int rt = 0;

    pTask->setParent(NULL);
    m_pStartBucketTask = NULL;

    rt = startBucket(pTask);

    if (-1 == rt) {
        recycle();
        return -1;
    }

    return 0;
}

int
MigrationClientTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_HANDSHAKE: {
            rt = handshake(pAgent, req);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    case TASK_MIGRATE_DATA: {
            rt = migrate(pAgent, req);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    case TASK_DELETE_SRC_BUCKET: {
            rt = deleteBucket(req);

            if (-1 == rt) {
                recycle();
                return -1;
            }

            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %" PRIi32, m_CurrentState);

            assert(0);
            recycle();

            return -1;
            break;
        }

    }

    return 0;
}

int
MigrationClientTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    if (pItem->getItemType() == MIGRATION_CLIENT_ITEM) {
        rt = dispatchMigrationClientItem(pItem);

        if (-1 == rt) {
            recycle();
            return -1;
        }

    } else {
        // never going here
        DEBUG_LOG("unexpected item type %d", pItem->getItemType());
        assert(0);

        delete pItem;
        pItem = NULL;
    }

    return 0;
}

int
MigrationClientTask::handshake()
{
    MsgHeader msg;
    msg.cmd = MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE;

    cstore::pb_MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE handshake;

    handshake.set_bucket_index(m_BucketId);

    std::string data;

    if (!handshake.SerializeToString(&data)) {
        DEBUG_LOG("protobuf serialize failed.");
        return -1;
    }

    msg.length = data.length();
    m_pAgent->sendPacket(msg, data.c_str());

    m_CurrentState = TASK_HANDSHAKE;

    return 0;
}

int
MigrationClientTask::connectCallback(MUTCPAgent *pAgent, bool bConn)
{
    if (!bConn) {
        ERROR_LOG("cannot connect source mu at %s", m_SrcMUIP.c_str());
        return -1;  // this will release agent
    }

    switch (m_CurrentState) {

    case TASK_INIT: {
            return handshake();
            break;
        }

    default: {
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}

int
MigrationClientTask::handshake(MUTCPAgent *pAgent, const InReq &req)
{
    if (MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command %d.", req.m_msgHeader.cmd);
        return -1;
    }

    if (MU_OK != req.m_msgHeader.error) {
        DEBUG_LOG("migration handshake error");
        return -1;
    }

    std::string data(req.ioBuf, req.m_msgHeader.length);
    cstore::pb_MSG_MU_MU_MIGRATE_BUCKET_HANDSHAKE_ACK ack;

    if (!ack.ParseFromString(data)) {
        DEBUG_LOG("protobuf parse error.");
        return -1;
    }

    uint64_t seqNr = 0;
    seqNr = ack.seq_nr();

    m_CurrentState = TASK_MIGRATE_DATA;

    return migrate();;
}

int
MigrationClientTask::migrate()
{
    sendSimplePacket(m_pAgent, MSG_MU_MU_MIGRATE_BUCKET_DATA);
    return 0;
}

int
MigrationClientTask::migrate(MUTCPAgent *pAgent, const InReq &req)
{
    if (MSG_MU_MU_MIGRATE_BUCKET_DATA_ACK != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command 0x%x.", req.m_msgHeader.cmd);
        return -1;
    }

    if (MU_OK != req.m_msgHeader.error) {
        DEBUG_LOG("receive data from dest mu failed");
    }

    if (MU_MORE_DATA == req.m_msgHeader.para1) {
        m_MoreData = true;
        TRACE_LOG("still have more data");

    } else {
        m_MoreData = false;
        TRACE_LOG("no data any more");
    }

    return flush(req);
}

int
MigrationClientTask::dispatchMigrationClientItem(MUWorkItem *pItem)
{
    m_pItem = dynamic_cast<MigrationClientItem *>(pItem);

    int rt = 0;

    ReturnStatus rs;
    rs = m_pItem->getReturnStatus();

    if (!rs.success()) {
        ERROR_LOG("thread pool work item error while doing migration");
        return -1;
    }

    switch (m_pItem->getWorkType()) {

    case MigrationClientItem::WRITE_BUCKET_DATA: {
            rt = flush(pItem);

            if (-1 == rt) {
                return -1;
            }

            break;
        }

    case MigrationClientItem::EXTRACT_BUCKET_DATA: {
            rt = extract(pItem);

            if (-1 == rt) {
                return -1;
            }

            break;
        }

    default: {
            DEBUG_LOG("unexpected work type %" PRIi32,
                      m_pItem->getWorkType());

            assert(0);

            return -1;
            break;
        }

    }

    return 0;
}

int
MigrationClientTask::startBucket()
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

    m_CurrentState = TASK_START_BUCKET;

    return 0;
}

int
MigrationClientTask::startBucket(MUTask *pTask)
{
    pTask->setParent(NULL);
    m_pStartBucketTask = NULL;

    m_CurrentState = TASK_START_BUCKET_COMPLETED;

    // response to cs

    if (NULL != m_pParent) {
        m_pParent->next(this);
        m_pParent = NULL;
    }

    return deleteBucket();
}

int
MigrationClientTask::deleteBucket()
{
    MsgHeader msg;
    msg.cmd = MSG_CS_MU_DELETE_BUCKET;

    cstore::pb_MSG_CS_MU_DELETE_BUCKET delBucket;

    delBucket.set_bucket_index(m_BucketId);

    std::string data;

    if (!delBucket.SerializeToString(&data)) {
        DEBUG_LOG("protobuf serialize failed");
        return -1;
    }

    msg.length = data.length();
    m_pAgent->sendPacket(msg, data.c_str());

    m_CurrentState = TASK_DELETE_SRC_BUCKET;

    return 0;
}

int
MigrationClientTask::deleteBucket(const InReq &req)
{
    if (MSG_CS_MU_DELETE_BUCKET_ACK != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command 0x%x", req.m_msgHeader.cmd);
        return -1;
    }

    if (MU_OK != req.m_msgHeader.error) {
        WARN_LOG("delete migrated bucket %" PRIu64 " on source mu failed",
                 m_BucketId);
        return -1;
    }

    m_CurrentState = TASK_FINISH;

    recycle();

    return 0;
}

int
MigrationClientTask::flush(const InReq &req)
{
    if (NULL == m_pItem) {
        m_pItem = new MigrationClientItem(this);
        m_pItem->setItemType(MIGRATION_CLIENT_ITEM);
        m_pItem->setBucketId(m_BucketId);
    }

    m_pItem->setWorkType(MigrationClientItem::WRITE_BUCKET_DATA);
    m_pItem->setBucketData(std::string(req.ioBuf, req.m_msgHeader.length));

    m_pItem->postRequest();
    m_pItem = NULL;

    return 0;
}

int
MigrationClientTask::flush(MUWorkItem *pItem)
{
    if (m_MoreData) {
        // still more data to transfer
        return migrate();

    } else {
        // close connection
        recycle(m_pAgent);
        m_pAgent = NULL;

        m_CurrentState = TASK_MIGRATE_DATA_COMPLETED;

        return extract();
    }

    return 0;
}

int
MigrationClientTask::extract()
{
    m_pItem->setWorkType(MigrationClientItem::EXTRACT_BUCKET_DATA);
    m_pItem->postRequest();
    m_pItem = NULL;

    m_CurrentState = TASK_EXTRACT_DATA;

    return 0;
}

int
MigrationClientTask::extract(MUWorkItem *pItem)
{
    Bucket *pBucket = new Bucket();
    pBucket->m_BucketId = m_BucketId;
    pBucket->m_BucketState = MU_BUCKET_STOP;

    BucketManager::getInstance()->put(pBucket->m_BucketId, pBucket);

    delete m_pItem;
    m_pItem = NULL;

    m_CurrentState = TASK_EXTRACT_DATA_COMPLETED;

    return startBucket();
}

