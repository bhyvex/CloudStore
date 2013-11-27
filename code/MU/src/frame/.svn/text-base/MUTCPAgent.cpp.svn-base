/*
 * @file MUTCPAgent.cpp
 * @brief TCP communication agent of MU.
 *
 * @version 1.0
 * @date Thu Jun 28 21:07:09 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTCPAgent.h"

#include "comm/comm.h"
#include "log/log.h"

#include "MUTask.h"
#include "MUAgentManager.h"

#include <assert.h>

MUTCPAgent::~MUTCPAgent()
{

}

MUTCPAgent::MUTCPAgent(Epoll *pEpoll) :
    TCPAgent(pEpoll)
{
    m_pTask = NULL;
    m_Close = false;
}

MUTCPAgent::MUTCPAgent(Epoll *pEpoll, MUTask *pTask) :
    TCPAgent(pEpoll)
{
    m_pTask = pTask;
    m_Close = false;
}

MUTCPAgent::MUTCPAgent(const TCPSocket &socket, const SocketAddress &addr,
                       Epoll *pEpoll) :
    TCPAgent(socket, addr, pEpoll)
{
    m_pTask = NULL;
}

int
MUTCPAgent::recycler()
{
    if (NULL != m_pTask) {
        m_pTask->destroy(this);
        m_pTask = NULL;
    }

    MUAgentManager::getInstance()->remove(m_AgentId);

    return 0;
}

int
MUTCPAgent::recycler(bool bymu)
{
    if (NULL != m_pTask) {
        m_pTask->destroy(this);
        m_pTask = NULL;
    }

    return 0;
}

void
MUTCPAgent::writeBack(bool result)
{
    if (m_Close &&
        0 == m_Bufv.getLength()) {
        // agent was set to be closed, and data in buffer have been sent out
        MUAgentManager::getInstance()->recycle(this);
    }
}

void
MUTCPAgent::close()
{
    m_Close = true;

    if (0 == m_Bufv.getLength()) {
        MUAgentManager::getInstance()->recycle(this);
    }
}

void
MUTCPAgent::setTask(MUTask *pTask)
{
    m_pTask = pTask;
}

void
MUTCPAgent::readBack(InReq &req)
{
    assert(m_pTask);

    int rc = 0;

    rc = m_pTask->next(this, req);

    if (-1 == rc) {
        DEBUG_LOG("In MUTCPAgent::readBack, m_pOwner->next() failed.");
    }
}

int
MUTCPAgent::connectAfter(bool bConn)
{
    assert(m_pTask);

    int rc = 0;

    rc = m_pTask->connectCallback(this, bConn);

    return rc;
}

void
MUTCPAgent::sendPacket(const MsgHeader &msg, const char *pData)
{
    char *pBuf = new char[sizeof(msg) + msg.length];

    memcpy(pBuf, &msg, sizeof(msg));

    if (NULL != pData) {
        memcpy(pBuf + sizeof(msg), pData, msg.length);
    }

    writeDynData(pBuf, sizeof(msg) + msg.length);
}

