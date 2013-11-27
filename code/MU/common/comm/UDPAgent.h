#ifndef __UDPAGENT_H__
#define __UDPAGENT_H__

#include "Agent.h"
#include "BaseTask.h"
#include "BaseReq.h"
#include "EpollEvent.h"
#include "BufferManager.h"
#include "UDPSocket.h"
#include "Epoll.h"
class SocketAddress;
class UDPSocket;
//class Epoll;
class UDPAgent: public Agent
{
public:
    UDPAgent();
    UDPAgent(Epoll*);
    UDPAgent(const SocketAddress &,Epoll*);
    virtual ~UDPAgent(void);
    virtual int init(const SocketAddress&);
    virtual int recvData(void);
    virtual int sendData(void);
//    virtual int agentRecycle(void){return SUCCESSFUL;}
    virtual int recycler(void);

    int readData(void);
    int writeData(void);
    int writeDynData(char* buf ,unsigned int len,BaseTask* task=NULL);
    virtual void readBack(InReq &req) {}
    virtual void writeBack(bool result) {}

protected:
    EpollEvent			mEpollEvent;
    UDPSocket			m_Sock;
    SocketAddress       m_OppoAddr;
    BufferManager     	m_Bufv;
    int		m_iLen;
    MsgHeader   	    m_msgHeader;
    bool m_bAgentType;
};
#endif
