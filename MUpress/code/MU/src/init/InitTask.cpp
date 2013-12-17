/*
 * @file InitTask.cpp
 * @brief System Init.
 *
 * @version 1.0
 * @date Fri Jul 20 16:27:03 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "InitTask.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>
#include <list>
#include <memory>

#include "comm/comm.h"
#include "log/log.h"

#include "frame/MURegister.h"
#include "frame/MUConfiguration.h"
#include "frame/MUTCPListenAgent.h"
#include "frame/MUTaskManager.h"
#include "frame/MUAgentManager.h"
#include "protocol/protocol.h"
#include "state/RuleManager.h"
#include "state/Bucket.h"
#include "state/BucketManager.h"
#include "item/InitItem.h"
#include "CSPM/StartBucketTask.h"
#include "ClientPM/ClientTask.h"

InitTask::~InitTask()
{
    if (TASK_FINISH != m_CurrentState) {
        ERROR_LOG("Initialize system failed. Program will exit.");
        exit(1);
    }

    if (NULL != m_pRSAgent) {
        TRACE_LOG("rs agent not null");
        recycle(m_pRSAgent);
        m_pRSAgent = NULL;
    }

    for (std::map<uint64_t, MUTask *>::iterator it =
             m_StartBucketTaskMap.begin();
         it != m_StartBucketTaskMap.end(); ++it) {
        recycle(it->second);
        it->second = NULL;
    }
}

InitTask::InitTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_pRSAgent = NULL;
}

int
InitTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_GET_RULE:
        rt = getRule(req);

        if (-1 == rt) {
            m_CurrentState = TASK_ERROR;
            recycle();
            return -1;
        }

        break;

    default: {
            // never reach here
            ERROR_LOG("unexpected task state %" PRIi32, m_CurrentState);

            assert(0);

            m_CurrentState = TASK_ERROR;
            recycle();

            return -1;
            break;
        }

    }

    return 0;
}

int
InitTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_CREATE_BUCKETS: {
            rt = dispatch(pItem);

            if (-1 == rt) {
                m_CurrentState = TASK_ERROR;
                recycle();
                return -1;
            }

            break;
        }

    default: {
            ERROR_LOG("unexpected task state %" PRIi32, m_CurrentState);

            assert(0);

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
InitTask::next(MUTask *pTask)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_START_BUCKETS: {
            rt = startBuckets(pTask);

            if (-1 == rt) {
                m_CurrentState = TASK_ERROR;
                recycle();
                return -1;
            }

            break;
        }

    case TASK_ERROR: {
            // remove task from task map
            pTask->setParent(NULL);

            StartBucketTask *pRTask = dynamic_cast<StartBucketTask *>(pTask);

            m_BucketList.remove(pRTask->getBucketId());
            m_StartBucketTaskMap.erase(pRTask->getBucketId());

            break;
        }

    default: {
            // never reach here
            ERROR_LOG("unexpected state %" PRIi32, m_CurrentState);

            assert(0);

            m_CurrentState = TASK_ERROR;
            recycle();

            return -1;
            break;
        }

    }

    return 0;
}

void
InitTask::destroy(MUTCPAgent *pAgent)
{
    m_pRSAgent = NULL;

    if (TASK_GET_RULE == m_CurrentState) {
        ERROR_LOG("rs agent error when get mu rules from rs");

        m_CurrentState = TASK_ERROR;
        recycle();
    }
}

void
InitTask::destroy(MUTask *pTask)
{
    ERROR_LOG("error occurrs while starting buckets");

    m_CurrentState = TASK_ERROR;
    recycle();
}

int
InitTask::start()
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_INIT: {
            // connect to rs to get mu rules
            rt = connectToRS();

            if (-1 == rt) {
                m_CurrentState = TASK_ERROR;
                return -1;
            }

            break;
        }

    default: {
            // should not reach here
            ERROR_LOG("unexpected state %" PRIi32, m_CurrentState);

            assert(0);

            m_CurrentState = TASK_ERROR;

            return -1;
            break;
        }

    }

    return 0;
}

int
InitTask::connectToRS()
{
    Epoll *pEpoll = MURegister::getInstance()->getEpoll();

    m_pRSAgent = MUAgentManager::getInstance()->createAgent<MUTCPAgent>(pEpoll);
    m_pRSAgent->setTask(this);

    SocketAddress rsAddr;

    rsAddr.setAddress(
        MUConfiguration::getInstance()->m_RSIP.c_str(),
        MUConfiguration::getInstance()->m_RSPort
    );


    int rt = 0;

    rt = m_pRSAgent->init();

    if (-1 == rt) {
        ERROR_LOG("init rs agent failed.");

        m_pRSAgent->setTask(NULL);
        delete m_pRSAgent;
        m_pRSAgent = NULL;

        return -1;
    }

    rt = m_pRSAgent->connect(rsAddr);

    if (-1 == rt) {
        ERROR_LOG("connect to rs failed.");

        m_pRSAgent->setTask(NULL);
        delete m_pRSAgent;
        m_pRSAgent = NULL;

        return -1;
    }

    m_CurrentState = TASK_GET_RULE;

    return 0;
}

int
InitTask::connectCallback(MUTCPAgent *pAgent, bool bConn)
{
    if (!bConn) {
        ERROR_LOG("can't connect to rs");
        return -1;  // release this agent, then agent will destroy this task
    }

    return getRule();
}

int
InitTask::getRule()
{
    MsgHeader msg;
    msg.cmd = MSG_SYS_RS_UPDATE_ALL_MU_HASH;

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH req;
    req.set_local_version(0);

    std::string data;

    if (!req.SerializeToString(&data)) {
        ERROR_LOG("protobuf serialize failed.");
        return -1;
    }

    msg.length = data.length();

    m_pRSAgent->sendPacket(msg, data.c_str());

    return 0;
}

int
InitTask::getRule(const InReq &req)
{
    // do not need rs agent any more
    recycle(m_pRSAgent);
    m_pRSAgent = NULL;

    if (RULER_OK != req.m_msgHeader.error) {
        ERROR_LOG("get rule from rs failed");
        return -1;
    }

    std::string data(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK ack;

    if (!ack.ParseFromString(data)) {
        ERROR_LOG("protobuf parse failed.");
        return -1;
    }

    // Get buckets which will store on current MU

    std::string localIP =
        MUConfiguration::getInstance()->m_ClientPMListenIP;

    int rt = 0;
    struct in_addr localAddr;

    rt = ::inet_pton(AF_INET, localIP.c_str(), &localAddr);

    if (-1 == rt) {
        ERROR_LOG("inet_pton() error, %s", strerror(errno));
        return -1;
    }

    uint32_t ip;

    cstore::Hash_Version hash = ack.hash_version();

    ERROR_LOG("local ip %" PRIu32, localAddr.s_addr);

    for (int i = 0; i < hash.map_list().size(); ++i) {
        cstore::Map_Item item  = hash.map_list(i);

        ERROR_LOG("in rule bucket %" PRIi32, i);

        for (int j = 0; j < item.module_ip().size(); ++j) {
            ip = item.module_ip(j);

            ERROR_LOG("ip %" PRIi32 " %" PRIu32, j, ip);

            if (ip == localAddr.s_addr) {
                m_BucketList.push_back(i);
                ERROR_LOG("get bucket %" PRIi32, i);
                break;
            }
        }
    }

    // set rule

    uint64_t modNr = log(hash.map_list().size()) / log(2);
    RuleManager::getInstance()->initModNr(modNr);
    TRACE_LOG("Rule mod nr %" PRIu64, modNr);

    if (0 == m_BucketList.size()) {
        // serve no buckets
        return startSystem();

    } else {
        return createBuckets();
    }

    return 0;
}

int
InitTask::dispatch(MUWorkItem *pItem)
{
    int rt = 0;

    switch (pItem->getItemType()) {

    case INIT_ITEM: {
            rt = dispatchInitItem(pItem);

            if (-1 == rt) {
                return -1;
            }

            break;
        }

    default: {
            // never reach here
            ERROR_LOG("unexpected item type %" PRIi32, pItem->getItemType());

            assert(0);

            return -1;
            break;
        }
    }

    return 0;
}

int
InitTask::dispatchInitItem(MUWorkItem *pItem)
{
    switch (pItem->getWorkType()) {

    case InitItem::CREATE_BUCKETS: {
            return createBuckets(pItem);

            break;
        }

    default: {
            ERROR_LOG("unexpected work type %" PRIi32, pItem->getWorkType());

            return -1;
            break;
        }

    }

    return 0;
}

int
InitTask::createBuckets()
{
    InitItem *pItem = new InitItem(this);

    pItem->setItemType(INIT_ITEM);
    pItem->setWorkType(InitItem::CREATE_BUCKETS);

    pItem->setBucketList(m_BucketList);

    pItem->postRequest();

    m_CurrentState = TASK_CREATE_BUCKETS;

    return 0;
}

int
InitTask::createBuckets(MUWorkItem *pItem)
{
    std::auto_ptr<InitItem> pRItem =
        std::auto_ptr<InitItem>(dynamic_cast<InitItem *>(pItem));

    ReturnStatus rs;

    rs = pRItem->getReturnStatus();

    if (!rs.success()) {
        ERROR_LOG("create buckets failed");
        return -1;
    }

    return startBuckets();
}

int
InitTask::startBuckets()
{
    int rt = 0;

    for (std::list<uint64_t>::iterator it = m_BucketList.begin();
         it != m_BucketList.end(); ++it) {
        StartBucketTask *pTask =
            MUTaskManager::getInstance()->createTask<StartBucketTask>(this);

        pTask->setBucketId(*it);

        rt = pTask->start();

        if (-1 == rt) {
            ERROR_LOG("init StartBucketTask error");

            recycle(pTask);

            return -1;
        }
    }

    m_CurrentState = TASK_START_BUCKETS;

    return 0;
}

int
InitTask::startBuckets(MUTask *pTask)
{
    // do not need this task any more
    pTask->setParent(NULL);

    StartBucketTask *pRTask = dynamic_cast<StartBucketTask *>(pTask);

    m_BucketList.remove(pRTask->getBucketId());
    m_StartBucketTaskMap.erase(pRTask->getBucketId());

    if (0 == m_BucketList.size()) {
        return startSystem();
    }

    return 0;
}

int
InitTask::startSystem()
{
    m_CurrentState = TASK_START_SYSTEM;

    Epoll *pEpoll = MURegister::getInstance()->getEpoll();

    MUTCPListenAgent<ClientTask> *pClientListenAgent =
        new MUTCPListenAgent<ClientTask>(pEpoll);

    SocketAddress addr;
    addr.setAddress(
        MUConfiguration::getInstance()->m_ClientPMListenIP.c_str(),
        MUConfiguration::getInstance()->m_ClientPMListenPort
    );

    int rt = 0;

    rt = pClientListenAgent->init(addr);

    if (-1 == rt) {
        ERROR_LOG("Initialize client listen socket failed.");
        return -1;
    }

    INFO_LOG("Initialize client listen socket, "
             "waiting for client connections.");

    m_CurrentState = TASK_FINISH;
    recycle();

    return 0;
}


