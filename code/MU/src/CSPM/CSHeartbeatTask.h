/*
 * @file CSHeartbeatTask.h
 * @brief Maintain a heartbeat connection with CS.
 *
 * @version 1.0
 * @date Tue Jul 10 15:36:00 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef CSPM_CSHeartbeatTask_H_
#define CSPM_CSHeartbeatTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUTCPAgent.h"

#include <inttypes.h>

class CSHeartbeatTask : public MUTask
{
public:
    virtual ~CSHeartbeatTask();

    CSHeartbeatTask(MUTask *pParent);

    virtual void destroy(MUTimer *pChannel);

    virtual void destroy(MUTCPAgent *pChannel);

    virtual int next(MUTimer *pTimer, uint64_t times);

    virtual int next(MUTCPAgent *pAgent, const InReq &req);

    virtual int connectCallback(MUTCPAgent *pAgent, bool bConn);

    int start();

private:
    int registerTimer();

    int connectToCS();

    int sendHandshake();

    int sendHeartbeat();

    int parseHandshakeAck(const InReq &req);

    int parseHeartbeatAck(const InReq &req);

    void checkTimeOut();

private:
    enum _CSHeartbeatTaskState {
        TASK_INIT,
        TASK_TIMER_REGISTERED,
        TASK_CSAGENT_CONNECTING,
        TASK_CSAGENT_DESTROYED,
        TASK_HANDSHAKE,
        TASK_HEARTBEAT,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    MUTimer *m_pTimer;
    MUTCPAgent *m_pAgent;
    int m_CSTimeOut;
};

#endif  // CSPM_CSHeartbeatTask_H_

