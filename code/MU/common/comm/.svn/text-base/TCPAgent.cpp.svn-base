#include <errno.h>
#include "Error.h"
#include "Epoll.h"
#include "TCPAgent.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "DevLog/DevLog.h"
extern DevLog* g_pDevLog;
using namespace std;
//constructor
TCPAgent::TCPAgent(Epoll *epl):m_isActive(true)
{
    m_iLen = 0;
    m_iConnectTimes = -1;
    mEpollEvent.setEpoll(epl);
    this->m_Bufv.setAgent(this);
}

TCPAgent::TCPAgent(const SocketAddress& oppoAddr,Epoll *epl):
    m_Addr(oppoAddr),m_iLen(0),m_isActive(true)
{
    m_iConnectTimes = -1;
    mEpollEvent.setEpoll(epl);
    this->m_Bufv.setAgent(this);
}

//constructor
TCPAgent::TCPAgent(const TCPSocket& sock,const SocketAddress& oppoAddr,Epoll *epl):
    m_Socket(sock),m_Addr(oppoAddr),m_iLen(0),m_isActive(false)
{
    m_iConnectTimes = -1;
    TCPAgent::setState(CONNECTED);
    mEpollEvent.setEpoll(epl);
    this->m_Bufv.setAgent(this);
    this->mEpollEvent.setFd(m_Socket.getSockFd());
    mEpollEvent.setHandler(this);

    //std::cout << "In TCPAgent: " << &m_Bufv << std::endl;

    if(mEpollEvent.registerREvent()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.registerREvent Error");
    }
}
int TCPAgent::init()
{
    return SUCCESSFUL;
}
//destructor
TCPAgent::~TCPAgent()
{
    try
    {
        if(this->recycler() < 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::~TCPAgent : recycler error");
        }
    }
    catch(...)
    {
        DEV_LOG(LERROR, OUT_BOTH, "catch exception in ~TCPAgent");
    }
}
int TCPAgent::getErrno()
{
    if(m_Socket.connectSocket(m_Addr)<0)
    {
//	    DEV_LOG(LINFO, OUT_BOTH, "m_Socket.connectSocket(m_Addr) error");
    }
    return errno;
}

int TCPAgent::recycler()
{
    if(m_bIsRecycler)
    {
        return SUCCESSFUL;
    }
    m_bIsRecycler=true;
#ifdef DEBUG
    if(false == m_isActive)
    {
        string ip;
        ip.assign(m_Addr.getIP());
        string msg = "*** Client Closed, IP is " + ip + " ***";
        DEV_LOG(LEVENT, OUT_BOTH, msg);
    }
#endif
    if(mEpollEvent.unregisterRWEvents()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.unregisterRWEvents() error");
    }
    mEpollEvent.setFd(-1);
    mEpollEvent.setHandler(NULL);
    if(this->m_Socket.closeSocket()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "m_Socket.closeSocket error");
    }
//  cout<<"nothing need to be recycled"<<endl;
    return SUCCESSFUL;
}

//intialize the TCPAgent,set the nonblock socket
int
TCPAgent::connect(const SocketAddress &addr)
{
    m_iConnectTimes+=1;
    if(m_Socket.closeSocket()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "m_Socket.closeSocket error");
    }
    m_Addr = addr;
    if( m_Socket.generateSocket()<0
            ||m_Socket.disableNagle()<0
            ||m_Socket.disableLinger()<0
            ||m_Socket.setNonblock()<0
            ||m_Socket.setKeepAlive()<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::connect():set the socket option error ");
        return FAILED;
    }
    mEpollEvent.setFd(m_Socket.getSockFd());
    mEpollEvent.setHandler(this);
    if(mEpollEvent.registerRWEvents()<0)
    {
        return FAILED;
    }
    if(m_Socket.connectSocket(m_Addr)<0)
    {
//        DEV_LOG(LINFO, OUT_BOTH, "NonBlock Connect Error");
    }
    return SUCCESSFUL;
}

int TCPAgent::sendData(void)
{
    return writeData();
}
int TCPAgent::recvData(void)
{
    return readData();
}
//write messages
int
TCPAgent::writeData(void)
{
    if(this->m_Bufv.write(m_Socket) <0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::writeData():m_Bufv.write(TCPSocket)");
        return FAILED;
    }
    if(this->m_Bufv.getLength() == 0)
    {
//		cout<<"closeWevent"<<endl;
        if(this->mEpollEvent.closeWevent()<0)
        {
            DEV_LOG(LINFO, OUT_BOTH, "this->mEpollEvent.closeWevent error");
        }
    }
    return SUCCESSFUL;
}

//write dynamic messages
int
TCPAgent::writeDynData(char *buf ,unsigned int len,BaseTask *task)
{
    if(this->m_Bufv.writeDynamic(buf,len,task) !=SUCCESSFUL)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::writeDynData:m_Bufv.WriteDynamic()");
        return FAILED;
    }
    if(this->mEpollEvent.openWevent()<0)
    {
        DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.openWevent error");
        return FAILED;
    }
    return SUCCESSFUL;
}

int
TCPAgent::readData()
{
    if((this->m_Bufv.read(m_Socket))<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPAgent::readData():m_Bufv.read()");
        DEV_LOG(LERROR, OUT_BOTH, ("opposit: " + m_Addr.convertToString())); //pxy
        return FAILED;
    }
//   cout<<"readData in TCPAgent"<<endl;
    return SUCCESSFUL;
}

int TCPAgent::connectAfter(bool bConnect)
{
    return SUCCESSFUL;
}
void TCPAgent::setState(int st)
{
    m_iConnect = CONNECTED;
    if( st == CONNECTED)
    {
        if(mEpollEvent.closeWevent()<0)
        {
            DEV_LOG(LINFO, OUT_BOTH, "mEpollEvent.closeWevent error");
        }
    }
}
/*int TCPAgent::expectRead(BaseTask *task )
{
   return SUCCESSFUL;
}*/
