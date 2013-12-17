/*
 * =========================================================================
 *
 *       Filename:  UnxDmnAgent.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2012-02-05 18:57:36
 *  Last Modified:  2012-02-05 18:57:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "UnxDmnAgent.h"

#include "log/log.h"

#include "UnxDmnSocketAddress.h"
#include "UnxDmnSocket.h"
#include "Buffer.h"
#include "UnxDmnReader.h"
#include "UnxDmnWriter.h"
#include "UnxDmnAgentReadCallback.h"
#include "UnxDmnAgentWriteCallback.h"

UnxDmnAgent::UnxDmnAgent(Epoll *pEpoll, AppProtoSpec *pSpec,
                         const UnxDmnSocketAddress &oppoAddr)
{
    m_iConnectTimes = -1;
    m_EpollEvent.setEpoll(pEpoll);
    m_EpollEvent.setHandler(this);
    m_OppoAddress = oppoAddr;

    m_Buffer.setAppProtoSpec(pSpec);
}

UnxDmnAgent::UnxDmnAgent(Epoll *pEpoll, AppProtoSpec *pSpec,
                         const UnxDmnSocket &socket,
                         const UnxDmnSocketAddress &oppoAddr)
{
    m_iConnectTimes = -1;
    m_EpollEvent.setEpoll(pEpoll);
    m_EpollEvent.setHandler(this);

    m_Socket = socket;
    m_OppoAddress = oppoAddr;
    m_EpollEvent.setFd(m_Socket.getFd());

    m_Buffer.setAppProtoSpec(pSpec);
    m_Buffer.setReader(new UnxDmnReader(m_Socket));
    m_Buffer.setWriter(new UnxDmnWriter(m_Socket));
    m_Buffer.setReadCallback(new UnxDmnAgentReadCallback(this));
    m_Buffer.setWriteCallback(new UnxDmnAgentWriteCallback(this));

    if (m_EpollEvent.registerREvent() < 0) {
        ERROR_LOG("In UnxDmnAgent::UnxDmnAgent, "
                  "m_EpollEvent.registerREvent() error");
    }

    setState(CONNECTED);
}

UnxDmnAgent::~UnxDmnAgent()
{
    if (m_EpollEvent.unregisterRWEvents() < 0) {
        ERROR_LOG("In UnxDmnAgent::~UnxDmnAgent, "
                  "m_EpollEvent.unregisterRWEvents() error");
    }

    m_EpollEvent.setFd(-1);
    m_EpollEvent.setHandler(NULL);

    if (m_Socket.close() < 0) {
        ERROR_LOG("In UnxDmnAgent::~UnxDmnAgent, m_Socket.close() error");
    }
}

int
UnxDmnAgent::init()
{
    return SUCCESSFUL;
}

int
UnxDmnAgent::connect()
{
    m_iConnectTimes += 1;

    m_Socket.close();

    int rt = 0;

    rt = m_Socket.socket();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnAgent::connect, m_Socket.socket() error");
        return FAILED;
    }

    rt = m_Socket.setNonBlock();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnAgent::connect, m_Socket.setNonBlock() error");
        return FAILED;
    }

    m_EpollEvent.setFd(m_Socket.getFd());
    m_Buffer.setReader(new UnxDmnReader(m_Socket));
    m_Buffer.setWriter(new UnxDmnWriter(m_Socket));
    m_Buffer.setReadCallback(new UnxDmnAgentReadCallback(this));
    m_Buffer.setWriteCallback(new UnxDmnAgentWriteCallback(this));

    if (m_EpollEvent.registerRWEvents() < 0) {
        ERROR_LOG("In UnxDmnAgent::connect, "
                  "m_EpollEvent.registerRWEvents() error");
        return FAILED;
    }

    if (m_Socket.connect(m_OppoAddress) < 0) {
        // NonBlock connect error
    }

    return SUCCESSFUL;
}

int
UnxDmnAgent::sendData()
{
    int rt = 0;

    rt = m_Buffer.write();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnAgent::sendData, m_Buffer.write() error");
        return FAILED;
    }

    if (0 == m_Buffer.getSendBufferLength()) {
        // 发送缓冲区无数据，关闲写事件
        rt = m_EpollEvent.closeWevent();

        if (rt < 0) {
            ERROR_LOG("In UnxDmnAgent::sendData, "
                      "m_EpollEvent.closeWevent() error");
            return FAILED;
        }
    }

    return SUCCESSFUL;
}

int
UnxDmnAgent::recvData()
{
    int rt = 0;

    rt = m_Buffer.read();

    if (rt < 0) {
        ERROR_LOG("In UnxDmnAgent::recvData, m_Buffer.read() error");
        return FAILED;
    }

    return SUCCESSFUL;
}

void
UnxDmnAgent::setState(int state)
{
    m_iConnect = state;

    if (state == CONNECTED) {
        if (m_EpollEvent.closeWevent() < 0) {
            ERROR_LOG("In UnxDmnAgent::setState, "
                      "m_EpollEvent.closeWevent() error");
        }
    }
}


void
UnxDmnAgent::writeToBuffer(char *pBuf, int len, DataContext *pContext)
{
    if (0 == m_Buffer.getSendBufferLength()) {
        if (m_EpollEvent.openWevent() < 0) {
            ERROR_LOG("In UnxDmnAgent::writeToBuffer, "
                    "m_EpollEvent.openWevent() error");
        }
    }

    m_Buffer.writeToBuffer(pBuf, len, pContext);
}

