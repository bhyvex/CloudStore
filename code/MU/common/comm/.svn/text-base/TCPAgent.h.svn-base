#ifndef TCPAGENT_H_
#define TCPAGENT_H_

#include "Agent.h"
#include "BaseTask.h"
#include "BaseHeader.h"
#include "EpollEvent.h"
#include "BufferManager.h"
#include "BaseReq.h"
#include "Epoll.h"
const SocketAddress InitAddr("0", 0);
class TCPSocket;
//class Epoll;
class TCPAgent:public Agent
{
public:
    TCPAgent():m_Addr(InitAddr) {}
    TCPAgent(Epoll *epl);
    TCPAgent(const SocketAddress&,Epoll*);
    TCPAgent(const TCPSocket&,const SocketAddress&,Epoll*);
    virtual ~TCPAgent();
    int connect(const SocketAddress & );
    virtual int init();
//    virtual int agentRecycle(void){return SUCCESSFUL;}
    virtual int recycler(void);
    virtual int sendData(void);// {return 0;}
    virtual int recvData(void);// {return 0;}
    virtual int doParse(void)
    {
        return 0;
    }
    virtual int doPackage(void)
    {
        return 0;
    }

    virtual void readBack(InReq& req) {}
    virtual void writeBack(bool result) {}
    virtual int connectAfter(bool);

    int writeDynData(char *buf ,unsigned int len,BaseTask *task=NULL);
    int writeData(void);
    //virtual int readHead(void);

    //int expectRead(BaseTask *);
    int readData(void);

    virtual int getOppAddr(SocketAddress& sockAddr)
    {
        if(InitAddr == m_Addr)
        {
            return FAILED;
        }
        sockAddr = m_Addr;
        return SUCCESSFUL;
    }

    const EpollEvent& getEpollEvent() const
    {
        return mEpollEvent;
    }
    void setState(int st);
    int getErrno();
//	int getErrno();
//	bool allowReconnect();

    TCPSocket getSocket()const
    {
        return m_Socket;
    }

protected:
    TCPSocket m_Socket;
    SocketAddress m_Addr;
    MsgHeader m_msgHeader;//maybe useless
//add by fb 0407
    EpollEvent mEpollEvent;
    int m_iLen;
    BufferManager m_Bufv;
    bool m_isActive;
};
#endif
