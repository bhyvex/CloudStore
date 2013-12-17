#include <string>
#include "Agent.h"
#include <errno.h>
#include "Epoll.h"
//#include "Header.h"


Agent::Agent():
    m_nRefCount(0),m_bIsRecycler(false),m_iConnect(CONNECTING),
    m_iConnectTimes(-1)
{
}
//the base class which is inherited by other agents such as class DHTUdpAgent
Agent::~Agent()
{
}

/*int Agent::init(SocketAddress &addr)
{
	return 0;
}*/
int Agent::getErrno()
{
    return errno;
}
int Agent::allowReconnect()const
{
    if(m_iConnectTimes < MAXRECONNECTTIMES)
        return true;
    else
        return false;
}

int Agent::recvData(void)
{
    return SUCCESSFUL;
}

int Agent::sendData(void)
{
    return SUCCESSFUL;
}

int Agent::recvReq(BaseRequest* req)
{
    return SUCCESSFUL;
}

int Agent::connectAfter(bool bConnect)
{
    return SUCCESSFUL;
}
int Agent::connect(const SocketAddress &addr)
{
    return SUCCESSFUL;
}

int Agent::connect()
{
    return SUCCESSFUL;
}

int Agent::recycler(void)
{
    m_bIsRecycler = true;
    return SUCCESSFUL;
}
bool Agent::isRecycler(void)
{
    return m_bIsRecycler;
}

long Agent::getRefCount()const
{
    return m_nRefCount;
}

void Agent::release()
{
    --m_nRefCount;
}

int Agent::getState()const
{
    return m_iConnect;
}

void Agent::setState(int st)
{
    m_iConnect = st;
}

Agent* Agent::ref()
{
    ++m_nRefCount;
    return this;
}
