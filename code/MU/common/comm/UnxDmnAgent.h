/*
 * =========================================================================
 *
 *       Filename:  UnxDmnAgent.h
 *
 *    Description:  使用Unix域套接字通信的Agent
 *
 *        Version:  1.0
 *        Created:  2012-01-13 01:11:16
 *  Last Modified:  2012-01-13 01:11:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNAGENT_H_
#define UNXDMNAGENT_H_

#include "Agent.h"
#include "BaseTask.h"
#include "BaseHeader.h"
#include "EpollEvent.h"
#include "BaseReq.h"
#include "Epoll.h"
#include "UnxDmnSocketAddress.h"
#include "UnxDmnSocket.h"
#include "Buffer.h"

#include "log/log.h"

/**
 * @brief 使用Unix域套接字通信
 */
class UnxDmnAgent : public Agent
{
public:
    // pSpec必须是new出来的，并且在析构函数中不能释放，
    // 它被传递到m_Buffer中，将被m_Buffer释放
    UnxDmnAgent(Epoll *pEpoll, AppProtoSpec *pSpec,
                const UnxDmnSocketAddress &oppoAddr);

    UnxDmnAgent(Epoll *pEpoll, AppProtoSpec *pSpec,
                const UnxDmnSocket &socket,
                const UnxDmnSocketAddress &oppoAddr);

    virtual ~UnxDmnAgent();

    int connect();

    virtual int init();

    virtual int sendData();

    virtual int recvData();

    virtual int recvReq(BaseRequest *req) {
        return 0;
    }

    virtual void readBack(InReq &req) {

    }

    virtual int readCallback(const char *pHeader, int headerLen,
                             char *pContent, int contentLen) {
        return 0;
    }

    virtual void writeBack(bool result) {
        if (!result) {
            ERROR_LOG("In UnxDmnAgent::writeBack, write error");
        }
    }

    virtual int writeCallback(bool bResult, 
            const DataContext *pContext) {
        return 0;
    }

    virtual int connectAfter(bool result) {
        return 0;
    }

    virtual void setState(int state);

    /**
     * @brief 
     *
     * @param pBuf 必须是new出来的内存，并且不能释放，
     *              数据发送后由m_Buffer释放
     * @param len
     * @param pContext
     */
    virtual void writeToBuffer(char *pBuf, int len, 
            DataContext *pContext = NULL);

protected:
    UnxDmnSocket m_Socket;
    UnxDmnSocketAddress m_OppoAddress;
    EpollEvent m_EpollEvent;
    Buffer m_Buffer;
};

#endif  // UNXDMNAGENT_H_

