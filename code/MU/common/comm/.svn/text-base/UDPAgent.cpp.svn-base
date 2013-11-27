#include "EpollEvent.h"
#include <sys/types.h>
#include "UDPAgent.h"
#include "Error.h"
#include "DevLog/DevLog.h"

using namespace std;
extern DevLog *g_pDevLog;

#define PASSIVE false
#define ACTIVE true

UDPAgent::UDPAgent()
{
    m_nRefCount = 1;
    m_iLen = 0;
    m_Bufv.setAgent(this);
    m_bAgentType=PASSIVE;
}

UDPAgent::UDPAgent(Epoll *epl)
{
    m_nRefCount = 1;
    m_iLen = 0;
    m_Bufv.setAgent(this);
    mEpollEvent.setEpoll(epl);
    m_bAgentType=PASSIVE;
}
//the parameter is the opposit end 's SocketAddress
UDPAgent::UDPAgent(const SocketAddress &oppoAddr,Epoll *epl)
    :m_OppoAddr(oppoAddr)
{
    DEV_LOG(LEVENT, OUT_BOTH, "UDPAgent::ACTIVE");
    m_bAgentType = ACTIVE;
    m_nRefCount = 1;
    m_iLen = 0;
    m_Bufv.setAgent(this);
    mEpollEvent.setEpoll(epl);
}

UDPAgent::~UDPAgent(void)
{
    try
    {
        if(this->recycler() != SUCCESSFUL)
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::~UDPAgent Error");
        }
    }
    catch(...)
    {
        DEV_LOG(LERROR, OUT_BOTH, "catch exception in ~UDPAgent");
    }
}

int UDPAgent::recycler(void)
{
    if ( m_bIsRecycler )
    {
        return 0;
    }
    m_bIsRecycler = true;

    if(mEpollEvent.unregisterRWEvents()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.unregisterRWEvents() error");
    }
    mEpollEvent.setFd(-1);
    mEpollEvent.setHandler( NULL );
    if(m_Sock.closeSocket()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "m_Sock.closeSocket() error");
    }

//	printf("UDPAgent::Recycler\n");

    return SUCCESSFUL;
}
//the parameter is the Server's SocketAddress
int UDPAgent::init(const SocketAddress & addr)
{
    if( m_Sock.generateSocket() < 0
            || m_Sock.enableReuseaddr() < 0
            || m_Sock.setNonblock() < 0
            || m_Sock.enableBroadcast() < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent:initialize");
        return -1;
    }
    if(!m_bAgentType)
    {
        if(m_Sock.bindAddr( addr)<0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::init():bindAddr()");
            return FAILED;
        }
    }
    mEpollEvent.setFd(m_Sock.getSockFd());
    mEpollEvent.setHandler(this);
    if(mEpollEvent.registerREvent()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.registerREvent error");
        return FAILED;
    }
    return SUCCESSFUL;
}

int UDPAgent::recvData(void)
{
    if(readData()<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::recvData(void):readData()");
        return FAILED;
    }
//	DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::recvData(void)");
    return SUCCESSFUL;
}

int UDPAgent::sendData(void)
{
    if(writeData()<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::sendData(void):writeData()");
        return FAILED;
    }
//	DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::sendData(void)");
    return SUCCESSFUL;
}

int UDPAgent::writeData(void)
{
    if(m_Bufv.write(m_Sock)<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::writeData(void):m_Bufv.write()");
        return FAILED;
    }
    if(this->m_Bufv.getLength() == 0)
    {
        if(mEpollEvent.closeWevent()<0)
        {
            DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.closeWevent error");
        }
    }
    return SUCCESSFUL;
}

int UDPAgent::readData(void)
{
    if(m_Bufv.read(m_Sock)<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::readData(void):m_Bufv.read()");
        return FAILED;
    }
    return SUCCESSFUL;
}

int
UDPAgent::writeDynData(char *buf ,unsigned int len, BaseTask *task)
{
    if(this->m_Bufv.writeDynamic(buf,len,m_OppoAddr,task) !=SUCCESSFUL)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::writeDynData:m_Bufv.WriteDynamic()");
        return FAILED;
    }
    if(this->mEpollEvent.openWevent()<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPAgent::writeDynData():openWevent()");
        return FAILED;
    }
    return SUCCESSFUL;
}
