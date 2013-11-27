/*
 * @file CSHeartbeatTask.cpp
 * @brief Task to do operation log rotation.
 *
 * @version 1.0
 * @date Tue Jul 10 16:08:53 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "CSHeartbeatTask.h"

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUConfiguration.h"
#include "frame/MURegister.h"
#include "frame/MUTaskManager.h"
#include "frame/MUAgentManager.h"
#include "protocol/protocol.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"

#include "comm/comm.h"

#include <assert.h>
#include <sys/statvfs.h>

#include <string>
#include <list>

CSHeartbeatTask::~CSHeartbeatTask()
{
    if (NULL != m_pTimer) {
        recycle(m_pTimer);
        m_pTimer = NULL;
    }

    if (NULL != m_pAgent) {
        recycle(m_pAgent);
        m_pAgent = NULL;
    }
}

CSHeartbeatTask::CSHeartbeatTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_CSTimeOut = 0;
    m_pTimer = NULL;
    m_pAgent = NULL;
}

void
CSHeartbeatTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    ERROR_LOG("cs heartbeat task, timer error");
    recycle();
}

void
CSHeartbeatTask::destroy(MUTCPAgent *pChannel)
{
    m_pAgent = NULL;

    DEBUG_LOG("agent error while doing heartbeating with cs");

    m_CurrentState = TASK_CSAGENT_DESTROYED;
}

int
CSHeartbeatTask::start()
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_INIT: {
            rt = registerTimer();
            if (-1 == rt) {
                DEBUG_LOG("registerTimer() error");
                return -1;
            }

            break;
        }

    default: {
            // should not reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);

            break;
        }

    }

    return 0;
}

int
CSHeartbeatTask::next(MUTimer *pTimer, uint64_t times)
{
    checkTimeOut();

    switch (m_CurrentState) {

    case TASK_TIMER_REGISTERED: {
            return connectToCS();
            break;
        }

    case TASK_CSAGENT_CONNECTING: {
            DEBUG_LOG("agent still connecting");
            break;
        }

    case TASK_CSAGENT_DESTROYED: {
            m_CurrentState = TASK_TIMER_REGISTERED;
            break;
        }

    case TASK_HANDSHAKE: {
            DEBUG_LOG("still doing handshake with cs");
            break;
        }

    case TASK_HEARTBEAT: {
            return sendHeartbeat();
            break;
        }

    default: {
            // should not reach here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
CSHeartbeatTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    // reset time out flag
    m_CSTimeOut = 0;

    switch (m_CurrentState) {

    case TASK_HANDSHAKE: {
            return parseHandshakeAck(req);
            break;
        }

    case TASK_HEARTBEAT: {
            return parseHeartbeatAck(req);
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
CSHeartbeatTask::connectCallback(MUTCPAgent *pAgent, bool bConn)
{
    if (!bConn) {
        ERROR_LOG("can't connect to cs");
        m_CurrentState = TASK_ERROR;

        return -1;  // release this agent
    }

    return sendHandshake();
}

int
CSHeartbeatTask::connectToCS()
{
    int rt = 0;

    Epoll *pEpoll = MURegister::getInstance()->getEpoll();

    assert(pEpoll);

    m_pAgent = MUAgentManager::getInstance()->createAgent<MUTCPAgent>(pEpoll);
    m_pAgent->setTask(this);

    SocketAddress csAddr;
    csAddr.setAddress(
        MUConfiguration::getInstance()->m_CSIP.c_str(),
        MUConfiguration::getInstance()->m_CSPort
    );

    rt = m_pAgent->init();

    if (-1 == rt) {
        DEBUG_LOG("init cs agent failed");

        m_pAgent->setTask(NULL);
        delete m_pAgent;
        m_pAgent = NULL;

        m_CurrentState = TASK_CSAGENT_DESTROYED;
        return -1;
    }

    rt = m_pAgent->connect(csAddr);

    if (-1 == rt) {
        DEBUG_LOG("connect to cs failed");

        m_pAgent->setTask(NULL);
        delete m_pAgent;
        m_pAgent = NULL;

        m_CurrentState = TASK_CSAGENT_DESTROYED;
        return -1;
    }

    m_CurrentState = TASK_CSAGENT_CONNECTING;

    return 0;
}

int
CSHeartbeatTask::sendHandshake()
{
    int rt = 0;

    MsgHeader msg;
    msg.cmd = MSG_MU_CS_HEARTBEAT_HANDSHAKE;

    struct statvfs st;

    rt = ::statvfs(MUConfiguration::getInstance()->m_FileSystemRoot.c_str(),
                   &st);

    if (-1 == rt) {
        DEBUG_LOG("statvfs() error, %s.", strerror(errno));
        return -1;
    }

    uint32_t sizeInMB = ((uint64_t) st.f_frsize) * st.f_blocks / 1024 / 1024;

    cstore::pb_MSG_MU_CS_HEARTBEAT_HANDSHAKE handshake;
    handshake.set_storage_load_limit(sizeInMB);

    std::string data;

    if (!handshake.SerializeToString(&data)) {
        DEBUG_LOG("protobuf serialize failed");
        return -1;
    }

    msg.length = data.length();

    m_pAgent->sendPacket(msg, data.c_str());

    m_CurrentState = TASK_HANDSHAKE;

    return 0;
}

int
CSHeartbeatTask::sendHeartbeat()
{
    MsgHeader msg;
    msg.cmd = MSG_MU_CS_HEARTBEAT;

    cstore::pb_MSG_MU_CS_HEARTBEAT heartbeat;
    cstore::Bucket_Item *pItem = NULL;

    std::list<Bucket *> bucketList;
    BucketManager::getInstance()->getAllBuckets(&bucketList);

    for (std::list<Bucket *>::iterator it = bucketList.begin();
         it != bucketList.end(); ++it) {
        pItem = heartbeat.add_bucket_item();
        pItem->set_bucket((*it)->m_BucketId);
        // cannot get bucket load now
        pItem->set_bucket_load(0);
        TRACE_LOG("bucket %" PRIu64 " heartbeat, current log seq %" PRIu64, 
                (*it)->m_BucketId, (*it)->m_LogSeq);
    }

    std::string data;

    if (!heartbeat.SerializeToString(&data)) {
        DEBUG_LOG("protobuf serialize failed");
        return -1;
    }

    msg.length = data.length();

    m_pAgent->sendPacket(msg, data.c_str());

    return 0;
}


int
CSHeartbeatTask::parseHandshakeAck(const InReq &req)
{
    if (MSG_MU_CS_HEARTBEAT_HANDSHAKE_ACK != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command %d.", req.m_msgHeader.cmd);
        return -1;
    }

    if (CS_OK != req.m_msgHeader.error) {
        DEBUG_LOG("handshake ack error, error code %d.",
                  req.m_msgHeader.error);
        return -1;
    }

    m_CurrentState = TASK_HEARTBEAT;

    return sendHeartbeat();
}

int
CSHeartbeatTask::parseHeartbeatAck(const InReq &req)
{
    if (MSG_MU_CS_HEARTBEAT_ACK != req.m_msgHeader.cmd) {
        DEBUG_LOG("unexpected protocol command %d.", req.m_msgHeader.cmd);
        return -1;
    }

    if (CS_OK != req.m_msgHeader.error) {
        DEBUG_LOG("Heartbeat ack error, error code %d.",
                  req.m_msgHeader.error);
        return -1;
    }

    return 0;
}

int
CSHeartbeatTask::registerTimer()
{
    int rt = 0;

    m_pTimer = new MUTimer(MURegister::getInstance()->getEpoll(), this);

    rt = m_pTimer->create();

    if (-1 == rt) {
        DEBUG_LOG("create timer failed");

        m_pTimer->setTask(NULL);
        delete m_pTimer;
        m_pTimer = NULL;

        return -1;
    }

    m_pTimer->setTime(
        MUConfiguration::getInstance()->m_CSHeartbeatKeepAlive,
        MUConfiguration::getInstance()->m_CSHeartbeatKeepAlive
    );

    m_CurrentState = TASK_TIMER_REGISTERED;

    return 0;
}

void
CSHeartbeatTask::checkTimeOut()
{
    if (m_CSTimeOut++ > MUConfiguration::getInstance()->m_CSHeartbeatTimeOut) {
        WARN_LOG("heartbeat with cs time out, cs maybe down");
    }
}


