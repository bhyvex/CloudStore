/*
 * @file MUTCPListenAgent.h
 * @brief Agent for TCP listen socket in MU.
 *
 * @version 1.0
 * @date Thu Jun 28 21:15:54 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MUTCPListenAgent_H_
#define MUTCPListenAgent_H_

#include "comm/comm.h"
#include "log/log.h"

#include "MUTCPAgent.h"
#include "PassiveTask.h"
#include "MUTaskManager.h"
#include "MUAgentManager.h"

template <typename ConcreteTask>
class MUTCPListenAgent : public TCPListenAgent<MUTCPAgent>
{
public:
    virtual ~MUTCPListenAgent();

    MUTCPListenAgent(Epoll *pEpoll);

    /**
     * @brief called when a new connection comes
     *
     * @return  0 if a success, or -1
     */
    int recvData();
};

template <typename ConcreteTask>
MUTCPListenAgent<ConcreteTask>::~MUTCPListenAgent()
{

}

template <typename ConcreteTask>
MUTCPListenAgent<ConcreteTask>::MUTCPListenAgent(Epoll *pEpoll) :
    TCPListenAgent<MUTCPAgent>(pEpoll)
{

}

template <typename ConcreteTask>
int
MUTCPListenAgent<ConcreteTask>::recvData()
{
    SocketAddress oppoAddr;

    int connFd = 0;

    connFd = m_TCPListenSocket.acceptConnection(oppoAddr);

    if (connFd < 0) {
        if (!((errno == EINTR) || (errno == EWOULDBLOCK))) {
            DEBUG_LOG("In MUTCPListenAgent::recvData, accept error, %s.",
                      strerror(errno));
            return -1;
        }
    }

    TCPSocket connSock(connFd);

    if (connSock.setNonblock() < 0 || connSock.disableLinger() < 0
        || connSock.disableNagle() < 0) {

        DEBUG_LOG("In MUTCPListenAgent::recvData, set socket options error, "
                  "%s.", strerror(errno));
        connSock.closeSocket();
        return -1;
    }

    MUTCPAgent *pAgent =
        MUAgentManager::getInstance()->createAgent<MUTCPAgent>(
            connSock, oppoAddr, m_epoll);

    PassiveTask *pTask =
        MUTaskManager::getInstance()->createTask<ConcreteTask>(NULL);

    pTask->setOwner(pAgent);

    pAgent->setTask(pTask);
    pAgent->init();

    return 0;
}

#endif  // MUTCPListenAgent_H_

