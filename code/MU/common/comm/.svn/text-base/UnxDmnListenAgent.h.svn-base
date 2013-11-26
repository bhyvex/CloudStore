/*
 * =========================================================================
 *
 *       Filename:  UnxDmnListenAgent.h
 *
 *    Description:  Unix域套接字监听套接字
 *
 *        Version:  1.0
 *        Created:  2012-02-06 00:33:59
 *  Last Modified:  2012-02-06 00:33:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UnxDmnListenAgent_H_
#define UnxDmnListenAgent_H_

#include "Agent.h"
#include "AgentManager.h"
#include "UnxDmnSocket.h"
#include "EpollEvent.h"
#include "Epoll.h"
#include "UnxDmnSocketAddress.h"

#include "sys/Singleton.h"
#include "log/log.h"

template <typename ConcreteAgent, typename ConcreteProtocol>
class UnxDmnListenAgent : public Agent
{
public:
    UnxDmnListenAgent(Epoll *pEpoll) {
        m_EpollEvent.setEpoll(pEpoll);
        m_pEpoll = pEpoll;
    }

    virtual ~UnxDmnListenAgent() {}

    virtual int init(UnxDmnSocketAddress &localAddr, int backlog);

    virtual int recvData();

    virtual int sendData();

private:
    UnxDmnSocket m_Socket;
    UnxDmnSocketAddress m_LocalAddress;
    EpollEvent m_EpollEvent;
    Epoll *m_pEpoll;
};

template <typename ConcreteAgent, typename ConcreteProtocol>
int
UnxDmnListenAgent<ConcreteAgent, ConcreteProtocol>::init(
    UnxDmnSocketAddress &localAddr,
    int backlog)
{
    m_LocalAddress = localAddr;

    int rt = 0;

    rt = m_Socket.socket();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnListenAgent::init, m_Socket.socket() error");
        return FAILED;
    }

    rt = m_Socket.setNonBlock();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnListenAgent::init, "
                  "m_Socket.setNonBlock() error");
        return FAILED;
    }

    rt = m_Socket.bind(m_LocalAddress);

    if (rt < 0) {
        ERROR_LOG("In UnxDmnListenAgent::init, "
                  "m_Socket.bind() error");
        return FAILED;
    }

    rt = m_Socket.listen(backlog);

    if (rt < 0) {
        ERROR_LOG("In UnxDmnListenAgent::init, "
                  "m_Socket.listen() error");
        return FAILED;
    }

    m_EpollEvent.setFd(m_Socket.getFd());
    m_EpollEvent.setHandler(this);
    m_EpollEvent.registerREvent();

    return SUCCESSFUL;
}

template <typename ConcreteAgent, typename ConcreteProtocol>
int
UnxDmnListenAgent<ConcreteAgent, ConcreteProtocol>::recvData()
{
    UnxDmnSocketAddress oppoAddr;
    int connFd;
    connFd = m_Socket.accept(&oppoAddr);

    if (connFd < 0) {
        if (!((errno == EINTR) || (errno == EWOULDBLOCK))) {
            ERROR_LOG("In UnxDmnListenAgent::recvData, "
                      "m_Socket.accept() error");
            return FAILED;
        }
    }

    UnxDmnSocket connSocket(connFd);

    // set nonblock
    if (connSocket.setNonBlock() < 0) {
        ERROR_LOG("In UnxDmnListenAgent::recvData, "
                  "connSocket.setNonBlock() error");
        return FAILED;
    }

    ConcreteAgent *pAgent = new ConcreteAgent(m_pEpoll,
            new ConcreteProtocol(), connSocket, oppoAddr);
    return pAgent->init();
}

template <typename ConcreteAgent, typename ConcreteProtocol>
int
UnxDmnListenAgent<ConcreteAgent, ConcreteProtocol>::sendData()
{
    return FAILED;
}

#endif  // UnxDmnListenAgent_H_

