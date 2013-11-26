/*
 * @file MUTCPAgent.h
 * @brief Communication agent of MU module.
 *
 * @version 1.0
 * @date Wed Jun 27 11:06:27 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MUTCPAgent_H_
#define MUTCPAgent_H_

#include "comm/comm.h"

class MUTask;

class MUTCPAgent : public TCPAgent
{
public:
    virtual ~MUTCPAgent();

    MUTCPAgent(Epoll *pEpoll);

    MUTCPAgent(Epoll *pEpoll, MUTask *pTask);

    /**
     * @brief constructor
     *
     * @param socket An established socket object.
     * @param addr Address of opposition.
     * @param pEpoll The Epoll driver.
     */
    MUTCPAgent(const TCPSocket &socket, const SocketAddress &addr,
               Epoll *pEpoll);

    /**
     * @brief called by AgentManager in comm
     *
     * @return 
     */
    int recycler();

    /**
     * @brief called by MUAgentManager
     *
     * @param 
     *
     * @return 
     */
    int recycler(bool bymu);

    /**
     * @brief called after data sent out
     *
     * @param result
     */
    void writeBack(bool result);

    /**
     * @brief close this agent, if there's still some data in buffer,
     *          send it first.
     */
    void close();

    /**
     * @brief Set associated task
     *
     * @param pTask
     */
    void setTask(MUTask *pTask);

    /**
     * @brief called by recvData() when read event ocurred
     *
     * @param req data packet in
     */
    void readBack(InReq &req);

    /**
     * @brief called by epoll when connection established or connect error.
     *
     * @param bConn
     *
     * @return
     */
    int connectAfter(bool bConn);

    /**
     * @brief Package data and send it out.
     *
     * @param msg Header
     * @param pData Data
     */
    void sendPacket(const MsgHeader &msg, const char *pData);

private:
    MUTask *m_pTask;
    // should be closed?
    bool m_Close;
};

#endif  // MUTCPAgent_H_

