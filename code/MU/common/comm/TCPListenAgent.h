#ifndef _TCPLISTENAGENT_H
#define _TCPLISTENAGENT_H

#include "Error.h"
#include "Agent.h"
#include "AgentManager.h"
#include "TCPSocket.h"
#include "EpollEvent.h"
#include "Epoll.h"
#include "SocketAddress.h"
#include <netinet/tcp.h>
#include <iostream>
#include <cstring>
#include <errno.h>
#include "DevLog/DevLog.h"
#include "sys/Singleton.h"
#define LISTEN_NUM 50
extern DevLog *g_pDevLog;

using namespace std;
template <class ConcreteAgent>
class TCPListenAgent : public Agent
{
public:
    TCPListenAgent(Epoll* epl)
    {
        //m_iPort = port;
        m_epollEvent.setEpoll(epl);
        m_epoll =epl;
    }
    ~TCPListenAgent() {}
    virtual int init(SocketAddress &);
//	virtual int handleDestory(void);
    virtual int recvData(void);
    virtual int sendData(void);

//private:
protected:
    TCPSocket m_TCPListenSocket;
    SocketAddress m_Addr;
//	int m_iPort;
    EpollEvent m_epollEvent;
    Epoll *m_epoll;
};

//initialize the TCPListenAgent
template <class ConcreteAgent>
int TCPListenAgent<ConcreteAgent>::init(SocketAddress &addr)
{
    m_Addr = addr;
    //create sockfd and
    //if(m_TCPListenSocket.init() == FAILED)
    //	return FAILED;
    if(m_TCPListenSocket.generateSocket()<0
            || m_TCPListenSocket.disableLinger()<0
            || m_TCPListenSocket.disableNagle()<0
            || m_TCPListenSocket.enableReuseaddr()<0
            || m_TCPListenSocket.setNonblock()<0
            || m_TCPListenSocket.bindAddr(m_Addr)<0
            || m_TCPListenSocket.listenOn(LISTEN_NUM)<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPListenAgent::init()");
        return FAILED;
    }
    //m_iFD = m_TCPListenSocket.getFD();
    //set the IP address
    //the following opetation is insteaded by above call
    /*  sockaddr_in listenAddr;
    	bzero(&listenAddr, sizeof(listenAddr));
    	listenAddr.sin_family = AF_INET;
    	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	listenAddr.sin_port = htons(m_iPort);
    	int on = 1;
    	//set the option of SO_REUSEADDR
    	if( setsockopt(m_iFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
    		ERROR_PRINT("%s\n", "set reuseaddr ERROR!");
    		handleDestory();
    		return FAILED;
    	}
    	//bind the  IP address
    	if(bind(m_iFD, (sockaddr*)&listenAddr, sizeof(listenAddr)) < 0){
    		ERROR_PRINT("%s\n", "TCPListenAgent::bind  ERROR!");
    		std::cout<<"port:"<<m_iPort<<std::endl;
    		handleDestory();
    		return FAILED;
    	}
    	m_TCPListenSocket.setNonBlock();  	//set the sockfd to noblock
    	m_TCPListenSocket.ListenOn(LISTEN_NUM);	 //listen on the sockfd
     */
    //Singleton<Epoll>::getInstancePtr()->registerRevent(m_iFD, this);
    //register the read I/O event
    m_epollEvent.setFd(m_TCPListenSocket.getSockFd());
    m_epollEvent.setHandler(this);
    m_epollEvent.registerREvent();
    return SUCCESSFUL;
}

//destory the TCPListenAgent
/*template <class ConcreteAgent>
int TCPListenAgent<ConcreteAgent>::handleDestory(void){
//	Singleton<Epoll>::GetInstancePtr()->unregisterFd(m_iFD);
//	unregister the read and write I/O event
	m_epollEvent.UnregisterRWEvents();
	m_TCPListenSocket.closeSocket();    		 //close the sockfd
	delete this;            //delete self
	return SUCCESSFUL;
}*/

//when the read event arrive, accept a connecton,
//create a ConcreteAgent and initialize it
template <class ConcreteAgent>
int TCPListenAgent<ConcreteAgent>::recvData(void)
{
    SocketAddress oppositeAddr;
    int connfd;
    connfd = m_TCPListenSocket.acceptConnection(oppositeAddr);
    if(connfd < 0)
    {
        if(!((errno == EINTR) || (errno == EWOULDBLOCK)) )
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPListenAgent listen");
            return FAILED;
        }
    }
    TCPSocket connSock(connfd);
    if(connSock.setNonblock()<0
            || connSock.disableLinger()<0
            || connSock.disableNagle()<0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPListenAgent::recvData");
        connSock.closeSocket();
        return FAILED;
    }
    //resigerREvent and initialize the Agent's member
    ConcreteAgent* agent = (AgentManager::getInstance())->createAgent<ConcreteAgent>(connSock,oppositeAddr,m_epoll);
    agent->init();
    return SUCCESSFUL;
}

//when the write event arrive, do nothing
template <class ConcreteAgent>
int TCPListenAgent<ConcreteAgent>::sendData(void)
{
    return FAILED;
}
//extern TCPListenAgent<RequestAgent> *pRequestListenAgent;
#endif
